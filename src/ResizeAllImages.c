/* Laboratório de CES-33 */
/* Playground: Redimensionar multiplas imagens */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wand/magick_wand.h>

int main(int argc, char *argv[])
{
    MagickWand *magick_wand;
    MagickPassFail status = MagickPass;
    const char *infile;
    char outfile[512];

    if (argc < 2)
    {
        fprintf(stderr,"Usage: %s: file1, file2, file3, ...\n", argv[0]);
        return 1;
    }

    // Initialize GraphicsMagick API
    InitializeMagick(*argv);

    // Allocate Wand handle
    magick_wand = NewMagickWand();

    // For each input image
    for (int i = 1; i < argc; i++) {
        infile=argv[i];
        sprintf(outfile, "output/redimensionada%d.jpg", i);

        // Read input image file
        if (status == MagickPass)
        {
            status = MagickReadImage(magick_wand, infile);
        }

        // Rezise image
        if (status == MagickPass)
        {
            status = MagickResizeImage(magick_wand, 1280, 720, LanczosFilter, 0);
        }

        // Write output file
        if (status == MagickPass)
        {
            status = MagickWriteImage(magick_wand, outfile);
        }

        // Diagnose any error
        if (status != MagickPass)
        {
            char *description;
            ExceptionType severity;

            description=MagickGetException(magick_wand, &severity);
            (void) fprintf(stderr, "%.1024s (severity %d)\n",
                            description, severity);
        }

    }
    
    // Release Wand handle
    DestroyMagickWand(magick_wand);

    // Destroy GraphicsMagick API
    DestroyMagick();

    return (status == MagickPass ? 0 : 1);
}