/* Laboratório de CES-33 */
/* Playground: Aplicação de filtros */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <wand/magick_wand.h>
#include "Queue.h"

#define OUTPUTDIR "../output"

sem_t mutexBuffer1;
sem_t mutexBuffer2;
sem_t mutexBuffer3;
// MagickPassFail status = MagickPass;

void dealWithError(MagickWand *magick_wand) 
{
    char *description;
    ExceptionType severity;

    description=MagickGetException(magick_wand, &severity);
    (void) fprintf(stderr, "%.1024s (severity %d)\n",
    description, severity);
}

void *loadFiles(int argc, char *argv[], Queue *buffer1) {
    MagickWand *magick_wand;
    const char *infile;

    // Populate buffer 1
    // For each input image
    for (int i = 1; i < argc; i++) {
        infile=argv[i];

        // Allocate Wand handle 
        magick_wand = NewMagickWand();

        if( MagickReadImage(magick_wand, infile) != MagickPass )
        {
            dealWithError(magick_wand);
        }
        else
        {
            // Entra na região crítica
            sem_wait(&mutexBuffer1);
            // Populate buffer 1
            enqueue(buffer1, &magick_wand);
            sem_post(&mutexBuffer1);
        }
    }
}

void *filterOne(Queue *buffer1, Queue *buffer2) {
    MagickWand *magick_wand = NULL;

    while(1) {
     
        // Entra na região crítica
        sem_wait(&mutexBuffer1);
        // Consume buffer 1
        dequeue(buffer1, &magick_wand);
        sem_post(&mutexBuffer1);
            
        if(magick_wand != NULL)
        {
            if( MagickEmbossImage(magick_wand, 0, 2) != MagickPass)
            {
                dealWithError(magick_wand);
            }
            else
            {
                // Entra na região crítica
                sem_wait(&mutexBuffer2);
                // Populate buffer 2
                enqueue(buffer2, &magick_wand);
                sem_post(&mutexBuffer2);
            } 
        } 
        magick_wand = NULL;
    }
}

void *saveFiles(Queue *buffer2, Queue *buffer3) {
    MagickWand *magick_wand = NULL;
    char outfile[512];

    int i = 1;
    while(1) {
        // Entra na região crítica
        sem_wait(&mutexBuffer2);
        // Consume buffer 2
        dequeue(buffer2, &magick_wand);
        sem_post(&mutexBuffer2);
            
        // TODO: pesar em como linkar com o nome original das files
        sprintf(outfile, OUTPUTDIR"/img-%d.jpg", i);
        if(magick_wand != NULL)
        {
            if (MagickWriteImage(magick_wand, outfile) != MagickPass)
            {
                dealWithError(magick_wand);
            }
            else
            {
                // Entra na região crítica
                sem_wait(&mutexBuffer3);
                // Populate buffer 3
                enqueue(buffer3, &magick_wand);
                sem_post(&mutexBuffer3);

                i++;
            }
        }

        magick_wand = NULL;
    }
}

void *releaseWands(Queue *buffer3){
    MagickWand *magick_wand = NULL;
    
    while(1) {
        // Entra na região crítica
        sem_wait(&mutexBuffer3);
        if( !isQueueEmpty(buffer3) )
            dequeue(buffer3, &magick_wand);
        sem_post(&mutexBuffer3);
        
        if(magick_wand != NULL)
            DestroyMagickWand(magick_wand);

        magick_wand = NULL;
    }
}


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr,"Usage: %s: file1, file2, file3, ...\n", argv[0]);
        return 1;
    }

    // Initializing semaphores
    sem_init(&mutexBuffer1, 0, 1);
    sem_init(&mutexBuffer2, 0, 1);
    sem_init(&mutexBuffer3, 0, 1);

    // Initializing buffers
    Queue buffer1, buffer2, buffer3;
    MagickWand *magick_wand;
    queueInitialize(&buffer1, sizeof(magick_wand));
    queueInitialize(&buffer2, sizeof(magick_wand));
    queueInitialize(&buffer3, sizeof(magick_wand));

    // Initialize GraphicsMagick API
    InitializeMagick(*argv);

    // Initializing threads
    // pthread_create(&pombo, NULL, pomboFunction, NULL);

    
    // Clean queues
    clearQueue(&buffer1);
    clearQueue(&buffer2);
    clearQueue(&buffer3);

    // Destroy GraphicsMagick API
    DestroyMagick();

    return 0;
}