CC=gcc
CFLAGS=`GraphicsMagickWand-config --cppflags --ldflags --libs`
SRCDIR=../src
OUTPUTDIR=../output

all: $(SRCDIR)/Filters.c $(SRCDIR)/ResizeAllImages.c $(SRCDIR)/RotateAllImages.c
	$(CC) -o Filters $(SRCDIR)/Filters.c -O $(CFLAGS)
	$(CC) -o ResizeAllImages $(SRCDIR)/ResizeAllImages.c -O $(CFLAGS)
	$(CC) -o RotateAllImages $(SRCDIR)/RotateAllImages.c -O $(CFLAGS)

simpleThread: SimpleThread.o libqueue.a
	$(CC) -o SimpleThread SimpleThread.o -L. -libqueue -lm

SimpleThread.o: $(SRCDIR)/SimpleThread.c 
	$(CC) -o SimpleThread $(SRCDIR)/SimpleThread.c -O $(CFLAGS)

libqueue.a: queue.o 
    ar rcs libqueue.a queue.o

queue.o: queue.c Queue.h
	$(CC) -O -c $(SRCDIR)/queue.c

sequential: $(SRCDIR)/Sequential.c
	$(CC) -o Sequential $(SRCDIR)/Sequential.c -O $(CFLAGS)

filters: $(SRCDIR)/Filters.c
	$(CC) -o Filters $(SRCDIR)/Filters.c -O $(CFLAGS)

resize: $(SRCDIR)/ResizeAllImages.c
	$(CC) -o ResizeAllImages $(SRCDIR)/ResizeAllImages.c -O $(CFLAGS)

rotate: $(SRCDIR)/RotateAllImages.c
	$(CC) -o RotateAllImages $(SRCDIR)/RotateAllImages.c -O $(CFLAGS)

clean:
	rm Filters ResizeAllImages RotateAllImages $(OUTPUTDIR)/*.jpg
