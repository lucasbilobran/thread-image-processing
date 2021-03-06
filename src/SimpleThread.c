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
sem_t mutexbufferSave;
sem_t mutexbufferWand;
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

typedef struct FilterTwoArgs
{
    Queue *buffer2;
    Queue *buffer3;
}filterTwoArgs;

typedef struct FilterThreeArgs
{
    Queue *buffer3;
    Queue *bufferSave;
}filterThreeArgs;

typedef struct SaveArgs
{
    Queue *bufferSave;
    Queue *bufferWand;
}saveArgs;

typedef struct ReleaseArgs
{
    int argc;
    Queue *bufferWand;
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

void *filterOne(void *f1A) {
    Queue *buffer1 = ((filterOneArgs *)f1A)->buffer1;
    Queue *buffer2 = ((filterOneArgs *)f1A)->buffer2;  

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

void *filterTwo(void *f2A) {
    Queue *buffer2 = ((filterTwoArgs *)f2A)->buffer2;
    Queue *buffer3 = ((filterTwoArgs *)f2A)->buffer3;  

    MagickWand *magick_wand = NULL;

    while(1) {
     
        // Entra na região crítica
        sem_wait(&mutexBuffer2);
        // Consume buffer 2
        dequeue(buffer2, &magick_wand);
        sem_post(&mutexBuffer2);
            
        if(magick_wand != NULL)
        {
            if( MagickSolarizeImage(magick_wand, 0.2) != MagickPass)
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
            } 
        } 
        magick_wand = NULL;
    }
}

void *filterThree(void *f3A) {
    Queue *buffer3 = ((filterThreeArgs *)f3A)->buffer3;
    Queue *bufferSave = ((filterThreeArgs *)f3A)->bufferSave;  

    MagickWand *magick_wand = NULL;
    PixelWand *background;
    background=NewPixelWand();
    PixelSetColor(background,"#000000");

    while(1) {
     
        // Entra na região crítica
        sem_wait(&mutexBuffer3);
        // Consume buffer 1
        dequeue(buffer3, &magick_wand);
        sem_post(&mutexBuffer3);
            
        if(magick_wand != NULL)
        {
            if( MagickRotateImage(magick_wand,background,30) != MagickPass)
            {
                dealWithError(magick_wand);
            }
            else
            {
                // Entra na região crítica
                sem_wait(&mutexbufferSave);
                // Populate buffer 2
                enqueue(bufferSave, &magick_wand);
                sem_post(&mutexbufferSave);
            } 
        } 

        magick_wand = NULL;
    }

    DestroyPixelWand(background);
}

void *saveFiles(void *sA) {
    Queue *bufferSave = ((saveArgs *)sA)->bufferSave;
    Queue *bufferWand = ((saveArgs *)sA)->bufferWand;  

    MagickWand *magick_wand = NULL;
    char outfile[512];

    int i = 1;
    while(1) {
        // Entra na região crítica
        sem_wait(&mutexbufferSave);
        // Consume buffer 2
        dequeue(bufferSave, &magick_wand);
        sem_post(&mutexbufferSave);
            
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
                sem_wait(&mutexbufferWand);
                // Populate last buffer 
                enqueue(bufferWand, &magick_wand);
                sem_post(&mutexbufferWand);

                i++;
            }
        }

        magick_wand = NULL;
    }
}

void *releaseWands(void *rA){
    int argc = ((releaseArgs *)rA)->argc;
    Queue *bufferWand = ((releaseArgs *)rA)->bufferWand;  

    MagickWand *magick_wand = NULL;
    
    while(1) {
        // Entra na região crítica
        sem_wait(&mutexbufferWand);
        if( !isQueueEmpty(bufferWand) )
            dequeue(bufferWand, &magick_wand);
        sem_post(&mutexbufferWand);
        
        if(magick_wand != NULL){
            DestroyMagickWand(magick_wand);
            total++;
        }
        if (total == argc){
            break;
        }
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
    total = 1;

    // Initializing semaphores
    sem_init(&mutexBuffer1, 0, 1);
    sem_init(&mutexBuffer2, 0, 1);
    sem_init(&mutexBuffer3, 0, 1);
    sem_init(&mutexbufferSave, 0, 1);
    sem_init(&mutexbufferWand, 0, 1);

    // Initializing buffers
    Queue buffer1, buffer2, buffer3, bufferSave, bufferWand;
    MagickWand *magick_wand;
    queueInitialize(&buffer1, sizeof(magick_wand));
    queueInitialize(&buffer2, sizeof(magick_wand));
    queueInitialize(&buffer3, sizeof(magick_wand));
    queueInitialize(&bufferSave, sizeof(magick_wand));
    queueInitialize(&bufferWand, sizeof(magick_wand));

    // Initialize GraphicsMagick API
    InitializeMagick(*argv);

    // Preparing args
    loadArgs *lA = malloc(sizeof(loadArgs));
    lA->argc = argc;
    lA->argv = argv;
    lA->buffer1 = &buffer1;
    filterOneArgs *f1A = malloc(sizeof(filterOneArgs));
    f1A->buffer1 = &buffer1;
    f1A->buffer2 = &buffer2;
    filterTwoArgs *f2A = malloc(sizeof(filterOneArgs));
    f2A->buffer2 = &buffer2;
    f2A->buffer3 = &buffer3;
    filterThreeArgs *f3A = malloc(sizeof(filterOneArgs));
    f3A->buffer3 = &buffer3;
    f3A->bufferSave = &bufferSave;
    saveArgs *sA      = malloc(sizeof(saveArgs));
    sA->bufferSave = &bufferSave;
    sA->bufferWand = &bufferWand;
    releaseArgs *rA   = malloc(sizeof(releaseArgs));
    rA->argc = argc;
    rA->bufferWand = &bufferWand;

    // Initializing threads
    pthread_t load, filter1, filter2, filter3 ,save, release; 
    pthread_create(&load,    NULL, loadFiles,   (void *)lA);
    pthread_create(&filter1, NULL, filterOne,   (void *)f1A);
    pthread_create(&filter2, NULL, filterTwo,   (void *)f2A);
    pthread_create(&filter3, NULL, filterThree, (void *)f3A);
    pthread_create(&save,    NULL, saveFiles,   (void *)sA);
    pthread_create(&release, NULL, releaseWands,(void *)rA);

    // Wait fot the last thread to end and then exit the others
    pthread_join(release, NULL);
    int rc = pthread_cancel(load);
    if(rc) printf("failed to cancel the load thread\n");
    rc = pthread_cancel(filter1);
    if(rc) printf("failed to cancel the filter1 thread\n");
    rc = pthread_cancel(filter2);
    if(rc) printf("failed to cancel the filter2 thread\n");
    rc = pthread_cancel(filter3);
    if(rc) printf("failed to cancel the filter3 thread\n");
    rc = pthread_cancel(save);
    if(rc) printf("failed to cancel the save thread\n");
    
    
    // Clean queues
    clearQueue(&buffer1);
    clearQueue(&buffer2);
    clearQueue(&buffer3);
    clearQueue(&bufferSave);
    clearQueue(&bufferWand);

    // Destroy GraphicsMagick API
    DestroyMagick();

    return 0;
}