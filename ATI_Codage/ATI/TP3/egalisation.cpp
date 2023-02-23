#include <stdio.h>
#include "image_ppm.h"
#include "fct_gyom.h"
#include <iostream>

int main(int argc, char *argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille;

  if (argc != 3)
  {
    printf("Usage: ImageIn.pgm ImgOut.ppm \n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImgLue);
  sscanf(argv[2], "%s", cNomImgEcrite);

  OCTET *ImgIn, *ImgOut;
  int maxGris = 255;

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  allocation_tableau(ImgIn, OCTET, nTaille);
  allocation_tableau(ImgOut, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

  egalisation(ImgIn, ImgOut, nH, nW);

  // Affichage pour les courbes gnuplot
  //NiveauDeGris Occurence(Histo) DensitéDeProba(ddp) FonctionDeRepartition(repart)
  int *Histo = histogramme_pgm(ImgIn, nTaille);
  float *ddp = densiteproba_pgm(ImgIn, nTaille);
  float *FdR = Fonctionrepartion_pgm(ImgIn, nTaille);
  for (int i = 0; i <= 255; i++)
  {
    printf("%i %i %f %f\n", i, Histo[i], ddp[i], FdR[i]);
  }

  ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
  free(ImgIn);
  free(Histo);

  return 1;
}