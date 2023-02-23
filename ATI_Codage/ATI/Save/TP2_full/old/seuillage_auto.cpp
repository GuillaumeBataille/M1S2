// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, S;

  if (argc != 3)
  {
    printf("Usage: ImageIn.pgm ImageOut.pgm \n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImgLue);
  sscanf(argv[2], "%s", cNomImgEcrite);

  OCTET *ImgIn, *ImgOut;

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(ImgOut, OCTET, nTaille);

  /// Start
  int seuil, var_max, sum, sumB, q1, q2, u1, u2 = 0;

  /// Histo
  int max_intensity = 255;
  int histo[256]; // Initialisation tableau final
  for (int i = 0; i <= max_intensity; i++)
  {
    histo[i] = 0;
  }

  for (int i = 0; i < nTaille; i++)
  {
    int value = ImgIn[i];
    histo[value] += 1;
  }
  for (int j = 0; j <= max_intensity; j++)
  {
    sum += j * histo[j];
  }

  for (int t = 0; t <= max_intensity; t++)
  {
    q1 += histo[t];
    if (q1 == 0)
    {
      continue;
    }
    q2 = nTaille - q1;
    if (q2 == 0)
    {
      // printf("q est null : %d et t = %d et nTaille = %d et q1 = %d", q2, t, nTaille, q1);
      continue;
    }
    sumB += t * histo[t];
    u1 = sumB / q1;         // good
    u2 = (sum - sumB) / q2; // Not good
    int delta = q1 * q2 * ((u1 - u2) * (u1 - u2));
    if (delta > var_max)
    {

      seuil = t;
      var_max = delta;
    }
  }

  /// Histo

  for (int i = 0; i < nH; i++)
    for (int j = 0; j < nW; j++) // Parcours de tout les pixels de l'image
    {

      if (ImgIn[i * nW + j] < seuil)
        ImgOut[i * nW + j] = 0;
      else
        ImgOut[i * nW + j] = 255; // si l'int de gris du pixel a l'indice x est inf  au seuil il devient 0 noir sinon blanc
    }

  ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
  free(ImgIn);
  free(ImgOut);

  return 1;
}
