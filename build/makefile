CC=gcc
CFLAGS=`GraphicsMagickWand-config --cppflags --ldflags --libs`
SRCDIR=../src
OUTPUTDIR=../output

default: multiThreadN2 multiThreadN5 multiThreadN10 multiThreadN20 simpleThread sequential

libqueue.a: queue.o 
	ar rcs libqueue.a queue.o

queue.o: $(SRCDIR)/queue.c $(SRCDIR)/Queue.h
	$(CC) -O -c $(SRCDIR)/queue.c

multiThreadN2: $(SRCDIR)/MultiThread.c libqueue.a
	$(CC) -o MultiThreadN2 $(SRCDIR)/MultiThread.c -DNUM_PARALEL_THREADS=2 -O $(CFLAGS) -L. -lqueue -lm

multiThreadN5: $(SRCDIR)/MultiThread.c libqueue.a
	$(CC) -o MultiThreadN5 $(SRCDIR)/MultiThread.c -DNUM_PARALEL_THREADS=5 -O $(CFLAGS) -L. -lqueue -lm

multiThreadN10: $(SRCDIR)/MultiThread.c libqueue.a
	$(CC) -o MultiThreadN10 $(SRCDIR)/MultiThread.c -DNUM_PARALEL_THREADS=10 -O $(CFLAGS) -L. -lqueue -lm

multiThreadN20: $(SRCDIR)/MultiThread.c libqueue.a
	$(CC) -o MultiThreadN20 $(SRCDIR)/MultiThread.c -DNUM_PARALEL_THREADS=20 -O $(CFLAGS) -L. -lqueue -lm

simpleThread: $(SRCDIR)/SimpleThread.c libqueue.a
	$(CC) -o SimpleThread $(SRCDIR)/SimpleThread.c -O $(CFLAGS) -L. -lqueue -lm

sequential: $(SRCDIR)/Sequential.c
	$(CC) -o Sequential $(SRCDIR)/Sequential.c -O $(CFLAGS)

filters: $(SRCDIR)/Filters.c
	$(CC) -o Filters $(SRCDIR)/Filters.c -O $(CFLAGS)

resize: $(SRCDIR)/ResizeAllImages.c
	$(CC) -o ResizeAllImages $(SRCDIR)/ResizeAllImages.c -O $(CFLAGS)

rotate: $(SRCDIR)/RotateAllImages.c
	$(CC) -o RotateAllImages $(SRCDIR)/RotateAllImages.c -O $(CFLAGS)

clean:
	rm -f Filters ResizeAllImages RotateAllImages Sequential MultiThreadN2 MultiThreadN5 MultiThreadN10 MultiThreadN20 SimpleThread $(OUTPUTDIR)/*.jpg *.o *.a
