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
int total;

typedef struct LoadArgs
{
    int argc;
    char **argv; 
    Queue *buffer1;
}loadArgs;

typedef struct FilterOneArgs
{
    Queue *buffer1;
    Queue *buffer2;
}filterOneArgs;

typedef struct SaveArgs
{
    Queue *buffer2;
    Queue *buffer3;
}saveArgs;

typedef struct ReleaseArgs
{
    int argc;
    Queue *buffer3;
}releaseArgs;

void dealWithError(MagickWand *magick_wand) 
{
    char *description;
    ExceptionType severity;

    description=MagickGetException(magick_wand, &severity);
    (void) fprintf(stderr, "%.1024s (severity %d)\n",
    description, severity);
}

void *loadFiles(void *lA) {
    int argc = ((loadArgs *)lA)->argc;
    char **argv = ((loadArgs *)lA)->argv;
    Queue *buffer1 = ((loadArgs *)lA)->buffer1; 

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

void *filterOne(void *fA) {
    Queue *buffer1 = ((filterOneArgs *)fA)->buffer1;
    Queue *buffer2 = ((filterOneArgs *)fA)->buffer2;  

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

void *saveFiles(void *sA) {
    Queue *buffer2 = ((saveArgs *)sA)->buffer2;
    Queue *buffer3 = ((saveArgs *)sA)->buffer3;  

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

void *releaseWands(void *rA){
    int argc = ((releaseArgs *)rA)->argc;
    Queue *buffer3 = ((releaseArgs *)rA)->buffer3;  

    MagickWand *magick_wand = NULL;
    
    while(1) {
        // Entra na região crítica
        sem_wait(&mutexBuffer3);
        if( !isQueueEmpty(buffer3) )
            dequeue(buffer3, &magick_wand);
        sem_post(&mutexBuffer3);
        
        if(magick_wand != NULL){
            DestroyMagickWand(magick_wand);
            total++;
        }
        if (total == argc)
            break;
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
    total = 0;

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

    // Preparing args
    loadArgs *lA = malloc(sizeof(loadArgs));
    lA->argc = argc;
    lA->argv = argv;
    lA->buffer1 = &buffer1;
    filterOneArgs *fA = malloc(sizeof(filterOneArgs));
    fA->buffer1 = &buffer1;
    fA->buffer2 = &buffer2;
    saveArgs *sA      = malloc(sizeof(saveArgs));
    sA->buffer2 = &buffer2;
    sA->buffer3 = &buffer3;
    releaseArgs *rA   = malloc(sizeof(releaseArgs));
    rA->buffer3 = &buffer3;

    // Initializing threads
    pthread_t load, filter1, save, release; 
    pthread_create(&load,    NULL, loadFiles, (void *)lA);
    pthread_create(&filter1, NULL, filterOne, (void *)fA);
    pthread_create(&save,    NULL, saveFiles, (void *)sA);
    pthread_create(&release, NULL, releaseWands, (void *)rA);

    // Wait fot the last thread to end and then exit the others
    pthread_join(release, NULL);
    int rc = pthread_cancel(load);
    if(rc) printf("failed to cancel the load thread\n");
    rc = pthread_cancel(filter1);
    if(rc) printf("failed to cancel the filter1 thread\n");
    rc = pthread_cancel(save);
    if(rc) printf("failed to cancel the save thread\n");
    
    
    // Clean queues
    clearQueue(&buffer1);
    clearQueue(&buffer2);
    clearQueue(&buffer3);

    // Destroy GraphicsMagick API
    DestroyMagick();

    return 0;
}