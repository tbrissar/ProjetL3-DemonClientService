// il n'y a peu de vérifications sur la validité des fichiers images,
// ils sont supposés corrects

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <arpa/inet.h>

#include "Image.h"

// lecture d'un fichier
Image image_lire(const char *filename)
{
    #define MAX_LENGTH 100

    Image image;
    char ligne[MAX_LENGTH];
    FILE *fd;
    char *ret;

    fd = fopen(filename, "r");
    assert(fd != NULL);

    // lecture du P5
    ret = fgets(ligne, MAX_LENGTH, fd);
    assert(ret != NULL);
    assert(strcmp(ligne, "P5\n") == 0);

    // résolution
    ret = fgets(ligne, MAX_LENGTH, fd);
    assert(ret != NULL);
    sscanf(ligne, "%d %d", &(image.nbLignes), &(image.nbColonnes));
    assert(image.nbLignes > 0);
    assert(image.nbColonnes > 0);

    // dynamique (qu'on impose à entre 256 et 65535)
    ret = fgets(ligne, MAX_LENGTH, fd);
    assert(ret != NULL);
    sscanf(ligne, "%d", &(image.dynamique));
    assert((image.dynamique >= 256) && (image.dynamique <= 65535));

    // lecture des niveaux de gris
    image.pixels = malloc(image.nbLignes*image.nbColonnes*sizeof(unsigned short));
    assert(image.pixels != NULL);
    fread(image.pixels, sizeof(unsigned short),
          image.nbLignes*image.nbColonnes, fd);

    for (int i = 0 ; i < image.nbLignes*image.nbColonnes ; i++)
        image.pixels[i] = ntohs(image.pixels[i]);

    fclose(fd);

    return image;
}

// écriture dans un fichier
void image_ecrire(const Image *image, const char *filename)
{
    FILE *fd;
    
    fd = fopen(filename, "w");
    assert(fd != NULL);

    fprintf(fd, "P5\n");
    fprintf(fd, "%d %d\n", image->nbLignes, image->nbColonnes);
    fprintf(fd, "%d\n", image->dynamique);

    for (int i = 0 ; i < image->nbLignes*image->nbColonnes ; i++)
        image->pixels[i] = htons(image->pixels[i]);
    
    fwrite(image->pixels, sizeof(unsigned short),
           image->nbLignes*image->nbColonnes, fd);

    for (int i = 0 ; i < image->nbLignes*image->nbColonnes ; i++)
        image->pixels[i] = ntohs(image->pixels[i]);

    fclose(fd);
}

void image_ecrire255(const Image *image, const char *filename)
{
    FILE *fd;
    
    fd = fopen(filename, "w");
    assert(fd != NULL);

    fprintf(fd, "P5\n");
    fprintf(fd, "%d %d\n", image->nbLignes, image->nbColonnes);
    fprintf(fd, "255\n");

    for (int i = 0 ; i < image->nbLignes*image->nbColonnes ; i++)
    {
        unsigned char c;
        if (image->pixels[i] > 255)
            c = 255;
        else
            c = (unsigned char) image->pixels[i];
        fwrite(&c, sizeof(unsigned char), 1, fd);
    }

    fclose(fd);
}

// libération des ressources mémoire
void image_detruire(Image *image)
{
    free(image->pixels);
    image->pixels = NULL;
    image->nbLignes = -1;
    image->nbColonnes = -1;
}
