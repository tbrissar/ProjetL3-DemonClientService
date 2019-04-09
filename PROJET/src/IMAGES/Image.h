#ifndef IMAGE_H
#define IMAGE_H

/*
 * Les valeurs sont comprises potentiellement entre 0 et 65535, i.e. elles
 * sont stockées dans des unsigned short
 * Les données sont stockées dans un tableau monodimensionnel
 * alors qu'il est intrinsèquement bidimensionnel : les lignes
 * sont mises les unes à la suite des autres.
 * Format manipulé : pgm binaire
 */
typedef struct
{
    int nbLignes;
    int nbColonnes;
    int dynamique;
    unsigned short *pixels;
} Image;

// lecture d'un fichier
Image image_lire(const char *filename);

// écriture dans un fichier
void image_ecrire(const Image *image, const char *filename);
void image_ecrire255(const Image *image, const char *filename);

// libération des ressources mémoire
void image_detruire(Image *image);

#endif
