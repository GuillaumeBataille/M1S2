#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
  char cNomImgLue[250];
  int nH, nW, nTaille;

  if (argc != 2)
  {
    printf("Usage: ImageIn.pgm \n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImgLue);

  OCTET *ImgIn;
  int *Histo;
  int maxGris = 255;

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(Histo, int, maxGris);

  int maxiCoul = 0;
  // lecture - comptage des niveaux de gris
  for (int i = 0; i < nTaille; i++)
  {
    Histo[ImgIn[i]]++;
  }
  //enregistrement des résultats
  for (int i = 0; i <= maxGris; i++)
  {
    printf("%i %i\n", i, Histo[i]);
  }

  free(ImgIn);
  free(Histo);

  return 1;
}