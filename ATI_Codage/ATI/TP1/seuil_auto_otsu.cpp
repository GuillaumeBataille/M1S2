#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille;

  if (argc != 3)
  {
    printf("Usage: ImageIn.pgm ImageOut.pgm \n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImgLue);
  sscanf(argv[2], "%s", cNomImgEcrite);

  OCTET *ImgIn, *ImgOut;
  int *Histo;
  int maxGris = 256;

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(Histo, int, maxGris);
  allocation_tableau(ImgOut, OCTET, nTaille); // allocation de l'image out

  int maxiCoul = 0;
  // lecture - comptage des niveaux de gris
  for (int i = 0; i < nTaille; i++)
  {
    Histo[ImgIn[i]]++;
  }

  //enregistrement des résultats
  /*for (int i = 0; i <= maxiCoul; i++)
  {
    printf("%i %i\n", i, Histo[i]);
  }*/

  // PARTIE OTSU

  //Somme des grey_level utilisé par l'algorithme d'otsu
  int sum = 0;
  int seuil;
  for (int i = 0; i < maxGris; i++)
  {
    //calculate the sum of all gray levels used by the algorithm
    sum += i * Histo[i];
  }

  //Variable pour somme partielles
  int sumB = 0;
  int wB = 0;
  int wF = 0;

  int varMax = 0; // Variance Max
  seuil = 0;      // Le seuil qui va être compute

  //Parcours de chaque niveau de gris[0,255] pour trouver le meilleur seuil
  for (int i = 0; i < maxGris; i++)
  {
    //increment the weight of black pixels
    wB += Histo[i];

    if (wB == 0) // Si y'a pas de pixel noir [0]
    {
      //if there are no black pixels, skip this iteration
      continue;
    }
    //calculate the weight of white pixels
    wF = nTaille - wB;
    if (wF == 0) // Si aucun pixel blanc [255]
    {
      //if there are no white pixels, skip the rest of the loop
      break;
    }

    //Maj de la somme partielle
    sumB += i * Histo[i];

    //Moyenne des grey_lvl de chaque grp
    int mB = sumB / wB;
    int mF = (sum - sumB) / wF;

    //Calcul de la variance entre les classes - formule internet
    int var_mid = wB * wF * (mB - mF) * (mB - mF);

    //Update si necessaire
    if (var_mid > varMax)
    {
      varMax = var_mid;
      seuil = i;
    }
  }
  // FIN OTSU - seuil final automatique dans "seuil";

  printf("Valeur de seuil final Otsu : %i\n", seuil);

  // Ecriture dans ImgOut
  for (int i = 0; i < nH; i++)
    for (int j = 0; j < nW; j++)
    {
      if (ImgIn[i * nW + j] < seuil) // utilisation du seuil calculé via algo d'otsu
        ImgOut[i * nW + j] = 0;
      else
        ImgOut[i * nW + j] = 255;
    }

  ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
  free(ImgIn);
  free(Histo);

  return 1;
}