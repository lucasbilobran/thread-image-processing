CC=gcc
CFLAGS=`GraphicsMagickWand-config --cppflags --ldflags --libs`
SRCDIR=../src
OUTPUTDIR=../output

all: $(SRCDIR)/Filters.c $(SRCDIR)/ResizeAllImages.c $(SRCDIR)/RotateAllImages.c
	$(CC) -o Filters $(SRCDIR)/Filters.c -O $(CFLAGS)
	$(CC) -o ResizeAllImages $(SRCDIR)/ResizeAllImages.c -O $(CFLAGS)
	$(CC) -o RotateAllImages $(SRCDIR)/RotateAllImages.c -O $(CFLAGS)

libqueue.a: queue.o 
	ar rcs libqueue.a queue.o

queue.o: $(SRCDIR)/queue.c $(SRCDIR)/Queue.h
	$(CC) -O -c $(SRCDIR)/queue.c

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
	rm Filters ResizeAllImages RotateAllImages $(OUTPUTDIR)/*.jpg
