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
        for (int i = 0; i < nH; i++)
            for (int j = 0; j < nW; j++) // Parcours de tout les pixels de l'image
            {
                if (pixel_blanc(ImgIn, i, j, nW))
                    ImgOut[i * nW + j] = 255;
                else
                    ImgOut[i * nW + j] = 0; // si l'int de gris du pixel a l'indice x est inf  au seuil il devient 0 noir sinon blanc
            }
        //  ImgIn = ImgOut; // ecrase la valeur de l'image precedente car on veut utilise ImgIn comme image de base pour la prochaine iteration
        refill(ImgIn, ImgOut, nTaille);
    }

    for (int x = 0; x < 6; x++)
    {
        for (int i = 0; i < nH; i++)
            for (int j = 0; j < nW; j++) // Parcours de tout les pixels de l'image
            {
                if (pixel_noir(ImgIn, i, j, nW))
                    ImgOut[i * nW + j] = 0;
                else
                    ImgOut[i * nW + j] = 255; // si l'int de gris du pixel a l'indice x est inf  au seuil il devient 0 noir sinon blanc
            }
        refill(ImgIn, ImgOut, nTaille);
    }

    for (int x = 0; x < 3; x++) //repeter x3
    {
        for (int i = 0; i < nH; i++)
            for (int j = 0; j < nW; j++) // Parcours de tout les pixels de l'image
            {
                if (pixel_blanc(ImgIn, i, j, nW))
                    ImgOut[i * nW + j] = 255;
                else
                    ImgOut[i * nW + j] = 0; // si l'int de gris du pixel a l'indice x est inf  au seuil il devient 0 noir sinon blanc
            }
        refill(ImgIn, ImgOut, nTaille);
    }

    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
    free(ImgOut);

    return 1;
}
