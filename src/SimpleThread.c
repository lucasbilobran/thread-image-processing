/* Laboratório de CES-33 */
/* Playground: Aplicação de filtros */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wand/magick_wand.h>
#include "Queue.h"

#define OUTPUTDIR "../output"

void dealWithError(MagickWand *magick_wand) 
{
    char *description;
    ExceptionType severity;

    description=MagickGetException(magick_wand, &severity);
    (void) fprintf(stderr, "%.1024s (severity %d)\n",
    description, severity);
}

int main(int argc, char *argv[])
{
    MagickWand *magick_wand;
    MagickPassFail status = MagickPass;
    const char *infile;
    char outfile[512];

    Queue buffer1, buffer2, buffer3;
    queueInitialize(&buffer1, sizeof(magick_wand));
    queueInitialize(&buffer2, sizeof(magick_wand));
    queueInitialize(&buffer3, sizeof(magick_wand));

    if (argc < 2)
    {
        fprintf(stderr,"Usage: %s: file1, file2, file3, ...\n", argv[0]);
        return 1;
    }

    // Initialize GraphicsMagick API
    InitializeMagick(*argv);

    // For each input image
    for (int i = 1; i < argc; i++) {
        infile=argv[i];

        // Allocate Wand handle 
        magick_wand = NewMagickWand();

        if(status == MagickPass)
        {
            status = MagickReadImage(magick_wand, infile);
        }

        // Populate buffer 1
        enqueue(&buffer1, &magick_wand);
    }

    while( !isQueueEmpty(&buffer1) )
    {
        // Consume buffer 1
        dequeue(&buffer1, &magick_wand);

        if(status == MagickPass)
        {
            status = MagickEmbossImage(magick_wand, 0, 2);
        }

        // Populate buffer 2
        enqueue(&buffer2, &magick_wand);
    }

    int i = 1;
    while( !isQueueEmpty(&buffer2) )
    {
        // Consume buffer 2
        dequeue(&buffer2, &magick_wand);

        sprintf(outfile, OUTPUTDIR"/img-%d.jpg", i);
        if (status == MagickPass)
        {
            status = MagickWriteImage(magick_wand, outfile);
        }
        else
        {
            dealWithError(magick_wand);
        }

        // Populate buffer 3
        enqueue(&buffer3, &magick_wand);
        i++;
    }
    
    // Release Wand handle
    while( !isQueueEmpty(&buffer3))
    {
        // Consume buffer 3
        dequeue(&buffer3, &magick_wand);
        DestroyMagickWand(magick_wand);
    }
    
    // Clean queues
    clearQueue(&buffer1);
    clearQueue(&buffer2);
    clearQueue(&buffer3);

    // Destroy GraphicsMagick API
    DestroyMagick();

    return ((status == MagickPass)? 0 : 1);
}