// test_couleur.cpp : Seuille une image couleur

#include <stdio.h>
#include "image_ppm.h"
#include "fct_gyom.h"
#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nR, nG, nB, SR, SG, SB, Sr, Sg, Sb, sm, SM;

  if (argc > 1)
  {
    sscanf(argv[1], "%s", cNomImgLue);
  }

  bool color = strcmp(cNomImgLue + strlen(cNomImgLue) - 3, "ppm") == 0;
  std::vector<int> min, max;

  if (color)
  {
    if (argc != 9)
    {
      printf("Usage: ImageIn.ppm ImageOut.ppm SeuilminR G B SeuilmaxR G B\n");
      exit(1);
    }
    //Minima

    sscanf(argv[3], "%d", &Sr); //Composante Rouge
    min.push_back(Sr);
    sscanf(argv[4], "%d", &Sg); //Composante Verte
    min.push_back(Sg);
    sscanf(argv[5], "%d", &Sb); //Composante Bleue
    min.push_back(Sb);

    //Maxima
    sscanf(argv[6], "%d", &SR); //Composante Rouge
    max.push_back(SR);
    sscanf(argv[7], "%d", &SG); //Composante Verte
    max.push_back(SG);
    sscanf(argv[8], "%d", &SB); //Composante Bleue
    max.push_back(SB);
  }
  else
  {
    if (argc != 5)
    {
      printf("Usage: ImageIn.ppm ImageOut.ppm Seuilmin Seuilmax\n");
      exit(1);
    }
    min.push_back(atoi(argv[3]));
    max.push_back(atoi(argv[4]));
  }
  sscanf(argv[2], "%s", cNomImgEcrite);
  //Lecture
  OCTET *ImgIn, *ImgOut;
  if (color)
    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
  else
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);

  int taille = (color ? nW * nH * 3 : nH * nW);

  allocation_tableau(ImgIn, OCTET, taille);
  allocation_tableau(ImgOut, OCTET, taille);

  //Traitement et ecriture
  if (color)
  {
    lire_image_ppm(cNomImgLue, ImgIn, nW * nH);
    extrema_seuil(ImgIn, ImgOut, min, max, nH, nW, color);
    //expansion_ppm(ImgOut, ImgOut, nH, nW);
    ecrire_image_ppm(cNomImgEcrite, ImgOut, nH, nW);
  }

  else
  {
    lire_image_pgm(cNomImgLue, ImgIn, nW * nH);
    extrema_seuil(ImgIn, ImgOut, min, max, nH, nW, color);
    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
  }

  free(ImgIn);
  return 1;
}
