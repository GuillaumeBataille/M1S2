// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"
#include "fct_gyom.h"
#include <iostream>

int main(int argc, char *argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille;
  bool isRGB;

  if (argc != 3)
  {
    printf("Usage: ImageIn.pgm ImageOut.pgm \n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImgLue);
  sscanf(argv[2], "%s", cNomImgEcrite);

  OCTET *ImgIn, *ImgOut;

  bool color = strcmp(cNomImgLue + strlen(cNomImgLue) - 3, "ppm") == 0;

  if (color)
    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
  else
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);

  int taille = (color ? nW * nH * 3 : nH * nW);

  allocation_tableau(ImgIn, OCTET, taille);
  allocation_tableau(ImgOut, OCTET, taille);

  //Expansion de l'image in vers out
  if (color)
  {
    lire_image_ppm(cNomImgLue, ImgIn, nW * nH);
    expansion_ppm(ImgIn, ImgOut, nH, nW);
    ecrire_image_ppm(cNomImgEcrite, ImgOut, nH, nW);
  }

  else
  {
    lire_image_pgm(cNomImgLue, ImgIn, nW * nH);
    expansion_pgm(ImgIn, ImgOut, nH, nW);
    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
  }

  free(ImgIn);
  free(ImgOut);
  return 1;
}
