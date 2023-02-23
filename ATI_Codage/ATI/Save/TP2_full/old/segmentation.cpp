// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"
#include "fonctionutile.h"
#include <iostream>

int main(int argc, char *argv[])
{
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille;

    if (argc != 3)
    {
        printf("Usage: ImageInSeuilled.pgm ImgOut.pgm \n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);

    OCTET *ImgIn, *ImgOut, *ImgInter;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille);
    allocation_tableau(ImgInter, OCTET, nTaille);

    for (int i = 0; i < nH; i++)
        for (int j = 0; j < nW; j++) // Parcours de tout les pixels de l'image
        {
            if (pixel_blanc(ImgIn, i, j, nW))
                ImgInter[i * nW + j] = 255;
            else
                ImgInter[i * nW + j] = 0; // si l'int de gris du pixel a l'indice x est inf  au seuil il devient 0 noir sinon blanc
        }

    for (int i = 0; i < nH; i++)
        for (int j = 0; j < nW; j++) // Parcours de tout les pixels de l'image
        {
            if (ImgInter[i * nW + j] = !ImgIn[i * nW + j])
            {
                ImgOut[i * nW + j] = 0;
            }
            else
                ImgOut[i * nW + j] = 255;
        }
    

    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
    free(ImgOut);
    free(ImgInter);
    return 1;
}
