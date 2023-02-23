// test_couleur.cpp : Seuille une image couleur

#include <stdio.h>
#include <iostream>
#include <vector>
#include <limits>
#include <random>
#include "image_ppm.h"
#include "color.h"
#include "ImageAlgorithms.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ImageIn.ppm\n");
        exit(1);
    }

    std::string inputName = argv[1];
    inputName = "../in/" + inputName;
    int nH, nW, nTaille, S;

    OCTET *ImgIn, *ImgOut, *reconstruction;

    lire_nb_lignes_colonnes_image_pgm(inputName, &nH, &nW);
    nTaille = nH * nW;

    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille);
    allocation_tableau(ImgOut, OCTET, nTaille);
    allocation_tableau(reconstruction, OCTET, nTaille);


    lire_image_pgm(inputName, ImgIn, nH * nW);

    // -------------------- Traitement --------------------

    int x,y;

    ImageAlgorithms::writeHistoDatFile(ImgIn, nTaille, "../histo.dat", false);
    ImageAlgorithms::diffMapAB2(ImgIn, ImgOut, nH, nW);
    ImageAlgorithms::writeDistibutionDatFile(ImgOut, nTaille, "../distribAB2.dat", false);

    ecrire_image_pgm("../out/diffMapAB2.pgm", ImgOut, nH, nW);

    ImageAlgorithms::diffMapMED(ImgIn, ImgOut, nH, nW);
    ecrire_image_pgm("../out/diffMapMED.pgm", ImgOut, nH, nW);

    ImageAlgorithms::writeDistibutionDatFile(ImgOut, nTaille, "../distribMED.dat", false);

    free(ImgIn);
    free(ImgOut);

    return 1;
}
