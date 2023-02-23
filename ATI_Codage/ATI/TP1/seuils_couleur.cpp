// test_couleur.cpp : Seuille une image couleur

#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, nR, nG, nB, SR, SG, SB;

  if (argc != 6)
  {
    printf("Usage: ImageIn.ppm ImageOut.ppm SeuilRed SeuilGreen SeuilBlue \n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImgLue);
  sscanf(argv[2], "%s", cNomImgEcrite);
  sscanf(argv[3], "%d", &SR); //Composante Rouge
  sscanf(argv[4], "%d", &SG); //Composante Verte
  sscanf(argv[5], "%d", &SB); //Composante Bleue

  OCTET *ImgIn, *ImgOut;

  lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  int nTaille3 = nTaille * 3; // Taille x 3 car 3 composantes
  allocation_tableau(ImgIn, OCTET, nTaille3);
  lire_image_ppm(cNomImgLue, ImgIn, nH * nW);  // Ecriture de l'image dans ImgIn via librairie du prof
  allocation_tableau(ImgOut, OCTET, nTaille3); // Allocation de l'image de sortie

  for (int i = 0; i < nTaille3; i += 3) //Pour tout les pixel (en sautant 3 par 3)
  {
    nR = ImgIn[i];
    nG = ImgIn[i + 1];
    nB = ImgIn[i + 2];
    if (nR < SR)
      ImgOut[i] = 0;
    else
      ImgOut[i] = 255;
    if (nG < SG)
      ImgOut[i + 1] = 0;
    else
      ImgOut[i + 1] = 255;
    if (nB < SB)
      ImgOut[i + 2] = 0;
    else
      ImgOut[i + 2] = 255;
  }

  ecrire_image_ppm(cNomImgEcrite, ImgOut, nH, nW);
  free(ImgIn);
  return 1;
}
