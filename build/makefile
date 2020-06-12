CC=gcc
CFLAGS=`GraphicsMagickWand-config --cppflags --ldflags --libs`
SRCDIR=../src
OUTPUTDIR=../output

default: multiThread simpleThread sequential

libqueue.a: queue.o 
	ar rcs libqueue.a queue.o

queue.o: $(SRCDIR)/queue.c $(SRCDIR)/Queue.h
	$(CC) -O -c $(SRCDIR)/queue.c

multiThread: $(SRCDIR)/MultiThread.c libqueue.a
	$(CC) -o MultiThread $(SRCDIR)/MultiThread.c -O $(CFLAGS) -L. -lqueue -lm

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
	rm -f Filters ResizeAllImages RotateAllImages Sequential MultiThread SimpleThread $(OUTPUTDIR)/*.jpg *.o *.a
