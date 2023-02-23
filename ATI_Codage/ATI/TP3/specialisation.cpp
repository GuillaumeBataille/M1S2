#include <stdio.h>
#include "image_ppm.h"
#include "fct_gyom.h"
#include <iostream>

int main(int argc, char *argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250], cNomImgRefe[250];
  int nH, nW, nHref, nWref, nTaille;

  if (argc != 4)
  {
    printf("Usage: ImageIn_Referene.pgm  ImageIn_A_specialiser ImgOut.ppm \n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImgRefe);
  sscanf(argv[2], "%s", cNomImgLue);
  sscanf(argv[3], "%s", cNomImgEcrite);

  std::cout << cNomImgEcrite << std::endl;
  std::cout << cNomImgLue << std::endl;
  std::cout << cNomImgRefe << std::endl;

  OCTET *ImgIn, *ImgOut, *ImgRefe;
  int maxGris = 255;

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  lire_nb_lignes_colonnes_image_pgm(cNomImgRefe, &nHref, &nWref);
  nTaille = nH * nW;

  allocation_tableau(ImgIn, OCTET, nTaille);
  allocation_tableau(ImgOut, OCTET, nTaille);
  allocation_tableau(ImgRefe, OCTET, nHref * nWref);

  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  lire_image_pgm(cNomImgRefe, ImgRefe, nHref * nWref);

  specialisation(ImgIn, ImgRefe, ImgOut, nH, nW, nHref, nWref);

  // Affichage pour les courbes gnuplot
  //NiveauDeGris Occurence(Histo) DensitéDeProba(ddp) FonctionDeRepartition(repart)
  int *Histo_ref = histogramme_pgm(ImgRefe, nWref * nHref); // histo de lena (reference)
  int *Histo_in = histogramme_pgm(ImgIn, nW * nH);
  int *Histo_out = histogramme_pgm(ImgOut, nW * nH);
  for (int i = 0; i <= 255; i++)
  {
    printf("%i %i %i %i\n", i, Histo_in[i], Histo_ref[i], Histo_out[i]);
  }

  ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
  free(ImgIn);
  free(Histo_in);

  return 1;
}