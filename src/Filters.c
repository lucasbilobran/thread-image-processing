/* Laboratório de CES-33 */
/* Playground: Aplicação de filtros */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wand/magick_wand.h>

int main(int argc, char *argv[])
{
    MagickWand *magick_wand1, *magick_wand2, *magick_wand3;
    MagickPassFail status1 = MagickPass, status2 = MagickPass, status3 = MagickPass;
    const char *infile;
    char outfile[512];

    if (argc < 2)
    {
        fprintf(stderr,"Usage: %s: file1, file2, file3, ...\n", argv[0]);
        return 1;
    }

    // Initialize GraphicsMagick API
    InitializeMagick(*argv);

    // Allocate Wand handle (one for each filter)
    magick_wand1 = NewMagickWand();
    magick_wand2 = NewMagickWand();
    magick_wand3 = NewMagickWand();

    // For each input image
    for (int i = 1; i < argc; i++) {
        infile=argv[i];

        // Read input image file
        if (status1 == MagickPass && status2 == MagickPass && status3 == MagickPass)
        {
            status1 = MagickReadImage(magick_wand1, infile);
            status2 = MagickReadImage(magick_wand2, infile);
            status3 = MagickReadImage(magick_wand3, infile);
        }

        // Rezise image
        if (status1 == MagickPass && status2 == MagickPass && status3 == MagickPass)
        {
            status1 = MagickThresholdImage(magick_wand1, 15000);
            status2 = MagickEmbossImage(magick_wand2, 0, 2);
            status3 = MagickSolarizeImage(magick_wand3, 0.2);
        }


        // Write output files
        sprintf(outfile, "output/tipo1-%d.jpg", i);
        if (status1 == MagickPass)
        {
            status1 = MagickWriteImage(magick_wand1, outfile);
        }

        sprintf(outfile, "output/tipo2-%d.jpg", i);
        if (status2 == MagickPass)
        {
            status2 = MagickWriteImage(magick_wand2, outfile);
        }

        sprintf(outfile, "output/tipo3-%d.jpg", i);
        if (status3 == MagickPass)
        {
            status3 = MagickWriteImage(magick_wand3, outfile);
        }

        // Diagnose any error
        if (status1 != MagickPass)
        {
            char *description;
            ExceptionType severity;

            description=MagickGetException(magick_wand1, &severity);
            (void) fprintf(stderr, "%.1024s (severity %d)\n",
                            description, severity);
        }
        if (status2 != MagickPass)
        {
            char *description;
            ExceptionType severity;

            description=MagickGetException(magick_wand2, &severity);
            (void) fprintf(stderr, "%.1024s (severity %d)\n",
                            description, severity);
        }
        if (status3 != MagickPass)
        {
            char *description;
            ExceptionType severity;

            description=MagickGetException(magick_wand3, &severity);
            (void) fprintf(stderr, "%.1024s (severity %d)\n",
                            description, severity);
        }

    }
    
    // Release Wand handle
    DestroyMagickWand(magick_wand1);
    DestroyMagickWand(magick_wand2);
    DestroyMagickWand(magick_wand3);

    // Destroy GraphicsMagick API
    DestroyMagick();

    return ((status1 == MagickPass && status2 == MagickPass && status3 == MagickPass)? 0 : 1);
}