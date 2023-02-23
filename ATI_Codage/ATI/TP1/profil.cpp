#include <stdio.h>
#include "image_ppm.h"
#include <iostream>

int main(int argc, char *argv[])
{
  char cNomImgLue[250];
  char cColLine[15];
  int nH, nW, nTaille, indice;

  if (argc != 4)
  {
    printf("Usage: ImageIn.pgm (l ou c) indice\n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImgLue);
  sscanf(argv[2], "%s", cColLine);
  sscanf(argv[3], "%d", &indice);

  if (strcmp(cColLine, "l") && strcmp(cColLine, "c"))
  {
    printf("c ou l en 3eme arg\n");
    exit(1);
  }

  OCTET *ImgIn;
  // Gestion des données pour le profil
  int *Profil;    // Tab de profil
  int sizeProfil; // Taille du tab de profil

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

  // lecture colonne
  if (strcmp(cColLine, "c") == 0)
  {
    allocation_tableau(Profil, int, nH); // allocation de la taille du tab de
    for (int y = 0; y < nH; y++)
    {
      Profil[y] = ImgIn[indice + y * nW];
    }
  }
  else
  {
    // lecture ligne
    allocation_tableau(Profil, int, nW); // allocation de la taille du tab de
    for (int x = 0; x < nH; x++)
    {
      Profil[x] = ImgIn[indice * nW + x];
    }
  }

  //enregistrement des résultats
  for (int i = 0; i <= nW; i++) // On itère uniquement jusqu'au maxiCoul pour le plot
  {
    printf("%i %i\n", i, Profil[i]);
  }

  free(ImgIn);
  free(Profil);

  return 1;
}