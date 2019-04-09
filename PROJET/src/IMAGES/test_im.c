#include <stdio.h>
#include <stdlib.h>

#include "Image.h"

static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s<image in> <image out>\n", exeName);
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}

int main(int argc, char * argv[])
{
    if (argc != 3)
        usage(argv[0], NULL);

    Image im;

    im = image_lire(argv[1]);
    //image_ecrire(&im, argv[2]);
    image_ecrire255(&im, argv[2]);
    image_detruire(&im);

    return EXIT_SUCCESS;
}
