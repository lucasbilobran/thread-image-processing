CC=gcc
CFLAGS=`GraphicsMagickWand-config --cppflags --ldflags --libs`
SRCDIR=../src
OUTPUTDIR=../output

all: $(SRCDIR)/Filters.c $(SRCDIR)/ResizeAllImages.c $(SRCDIR)/RotateAllImages.c
	$(CC) -o Filters $(SRCDIR)/Filters.c -O $(CFLAGS)
	$(CC) -o ResizeAllImages $(SRCDIR)/ResizeAllImages.c -O $(CFLAGS)
	$(CC) -o RotateAllImages $(SRCDIR)/RotateAllImages.c -O $(CFLAGS)

simpleThread: $(SRCDIR)/SimpleThread.c queue
	$(CC) -o SimpleThread $(SRCDIR)/SimpleThread.c -O $(CFLAGS)

queue: 
	$(CC) -o $(SRCDIR)/queue.o -c $(SRCDIR)/queue.c
	$(CC) $(SRCDIR)/SimpleThread.c $(SRCDIR)/queue.o

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
