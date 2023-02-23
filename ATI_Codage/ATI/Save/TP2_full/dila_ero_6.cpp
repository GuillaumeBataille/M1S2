// test_couleur.cpp : Seuille une image en niveau de gris

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
        printf("Usage: ImageIn.pgm ImageOut.pgm \n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);

    OCTET *ImgIn, *ImgOut, *ImgA, *ImgB;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille);

    for (int x = 0; x < 3; x++) //repeter x3
    {
        //dilatation
        dilationnb(ImgIn, ImgOut, nH, nW);
        refill(ImgIn, ImgOut, nTaille);
    }

    for (int x = 0; x < 6; x++) //repeter x6
    {
        //dilatation
        erosionnb(ImgIn, ImgOut, nH, nW);
        refill(ImgIn, ImgOut, nTaille);
    }

    for (int x = 0; x < 3; x++) //repeter x3
    {
        //dilatation
        dilationnb(ImgIn, ImgOut, nH, nW);
        refill(ImgIn, ImgOut, nTaille);
    }

    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
    free(ImgOut);

    return 1;
}
