#include <stdio.h>
#include <vector>
#include <iostream>
#include "image_ppm.h"

std::vector<int> global_thresholding(const std::vector<int> &hist, int level)
{
  std::vector<int> thresholds;
  int n = hist.size();

  //Calculate the total sum of the histogram
  int sum = 0;
  for (int i = 0; i < n; i++)
  {
    sum += i * hist[i];
  }

  //Calculate the threshold
  int threshold = sum / level;
  thresholds.push_back(threshold);

  //Calculate the next thresholds
  for (int i = 1; i < level - 1; i++)
  {
    threshold += sum / level;
    thresholds.push_back(threshold);
  }

  return thresholds;
}

int main(int argc, char *argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, level;

  if (argc != 4)
  {
    printf("Usage: ImageIn.pgm ImageOut.pgm  NbrSeuil \n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImgLue);
  sscanf(argv[2], "%s", cNomImgEcrite);
  sscanf(argv[2], "%d", level); // le nombre de seuil qu'on souhaite
  OCTET *ImgIn, *ImgOut;
  int *Histo;
  int maxGris = 256;
  // New
  std::vector<int> Hist;
  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(Histo, int, maxGris);
  Hist.resize(256, 0);
  allocation_tableau(ImgOut, OCTET, nTaille); // allocation de l'image out

  int maxiCoul = 0;
  // lecture - comptage des niveaux de gris
  for (int i = 0; i < nTaille; i++)
  {
    Hist[ImgIn[i]]++;
    Histo[ImgIn[i]]++;
    if (ImgIn[i] > maxiCoul)
    {
      maxiCoul = ImgIn[i];
    }
  }

  //enregistrement des résultats
  for (int i = 0; i <= maxiCoul; i++)
  {
    //  printf("%i %i %i\n", i, Hist[i], Histo[i]);
  }

  std::vector<int> thresholds = global_thresholding(Hist, 3);
  std::cout << "Threshold " << thresholds.size() << std::endl;
  ;

  /* // Ecriture dans ImgOut
  for (int i = 0; i < nH; i++)
    for (int j = 0; j < nW; j++)
    {
      if (ImgIn[i * nW + j] < seuil) // utilisation du seuil calculé via algo d'otsu
        ImgOut[i * nW + j] = 0;
      else
        ImgOut[i * nW + j] = 255;
    }
    */

  ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
  free(ImgIn);
  free(Histo);

  return 1;
}