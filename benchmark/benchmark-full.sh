#!/bin/bash

# Benchmark Configuration
EXEC_PER_BENCH=3

# Benchmark Function
benchmark () {
  i=0
  sum=0
  while test $i -lt $EXEC_PER_BENCH
  do
    echo -ne "  #$((i+1)) execution: "
    exectime=0
    SEQ_TIME=$({ time $script $benchsuite/*.jpg >/dev/null; } |& grep real | cut -f2)
    MIN=$(echo "$SEQ_TIME" | cut -d'm' -f1)
    SEC=$(echo "$SEQ_TIME" | cut -d'm' -f2 | cut -d's' -f1)
    exectime=$(echo "$MIN*60 + $SEC" | bc -l)
    echo "$exectime seconds"
    sum=$(echo "$sum + $exectime" | bc -l)
    i=$((i+1))
  done
  avgtime=$(echo "scale=3; $sum / $EXEC_PER_BENCH" | bc -l)
  echo -e "  Average exection time: \e[32m\e[1m$avgtime\e[0m seconds"
  rm ../output/*.jpg
}

benchmarksuite() {
  echo -e "\n\n\e[1m----- Benchmark Suite: $title -----\e[0m"

  # ----------------------------------------------------------
  echo -e '\n\e[32m* Benchmarking Sequential Model (No threads): \e[0m'
  script='./Sequential'
  benchmark
  # ----------------------------------------------------------
  echo -e '\n\e[32m* Benchmarking Simple Threads Model (Single Thread per Filter): \e[0m'
  script='./SimpleThread'
  benchmark
  # ----------------------------------------------------------
  echo -e '\n\e[32m* Benchmarking Multi Threads Model (2 Threads per Filter): \e[0m'
  script='./MultiThreadN2'
  benchmark
  # ----------------------------------------------------------
  echo -e '\n\e[32m* Benchmarking Multi Threads Model (5 Threads per Filter): \e[0m'
  script='./MultiThreadN5'
  benchmark
  # ----------------------------------------------------------
  echo -e '\n\e[32m* Benchmarking Multi Threads Model (10 Threads per Filter): \e[0m'
  script='./MultiThreadN10'
  benchmark
  # ----------------------------------------------------------
  echo -e '\n\e[32m* Benchmarking Multi Threads Model (20 Threads per Filter): \e[0m'
  script='./MultiThreadN20'
  benchmark
}

# Start benchmark
clear

echo -e '\e[1m----- Starting benchmark script -----\e[0m'
cd ../build

echo -e '\n\n\e[1m----- Cleaning and Compiling -----\e[0m'
make clean && make

title="Dummy Set (34 images in 1080p) [1/7]"
benchsuite="../images/dummyset"
benchmarksuite

title="[1080p Test] Small Set (50 images in 1080p) [2/7]"
benchsuite="../images/1080p/smallset"
benchmarksuite

title="[1080p Test] Medium Set (150 images in 1080p) [3/7]"
benchsuite="../images/1080p/mediumset"
benchmarksuite

title="[1080p Test] Big Set (500 images in 1080p) [4/7]"
benchsuite="../images/1080p/bigset"
benchmarksuite

title="[4K Test] Small Set (34 images) [5/7]"
benchsuite="../images/4K/smallset"
benchmarksuite

title="[4K Test] Medium Set (150 images in 4K) [6/7]"
benchsuite="../images/4K/mediumset"
benchmarksuite

title="[4K Test] Big Set (500 images in 4K) [7/7]"
benchsuite="../images/4K/bigset"
benchmarksuite

echo -e '\n\n\e[1m----- Benchmark Completed -----\e[0m\n'
