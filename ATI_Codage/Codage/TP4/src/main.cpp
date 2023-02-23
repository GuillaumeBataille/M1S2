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

    allocation_tableau(ImgIn, OCTET, nTaille);
    allocation_tableau(ImgOut, OCTET, nTaille);
    allocation_tableau(reconstruction, OCTET, nTaille);


    lire_image_pgm(inputName, ImgIn, nH * nW);

    // -------------------- Traitement --------------------

    // --------------------------- SANS QUANTIFICATION ---------------------------
    // Sans compression, mais en fait si un peu. La méthode compresse toute seule alors qu'on a pas toucher aux tailles des sous images
    // 👆 jsp si c'est normal. C'est une méthode avec perte donc bon ...

    OCTET *ImgBF, *ImgHV, *ImgHF, *ImgTHF;
    int nW2 = nW/2;
    int nH2 = nH/2;
    allocation_tableau(ImgBF, OCTET, nW2*nH2);
    allocation_tableau(ImgHV, OCTET, nW2*nH2);
    allocation_tableau(ImgHF, OCTET, nW2*nH2);
    allocation_tableau(ImgTHF, OCTET, nW2*nH2);

    ImageAlgorithms::haarWaveletDecompose(ImgIn, ImgBF, ImgHF, ImgHV, ImgTHF, nH, nW);

    ecrire_image_pgm("../out/ImgBF.pgm", ImgBF, nH2, nW2);
    ecrire_image_pgm("../out/ImgHF.pgm", ImgHF, nH2, nW2);
    ecrire_image_pgm("../out/ImgHV.pgm", ImgHV, nH2, nW2);
    ecrire_image_pgm("../out/ImgTHF.pgm", ImgTHF, nH2, nW2);

    ImageAlgorithms::reconstruct_haarWavelet(reconstruction, ImgBF, ImgHF, ImgHV, ImgTHF, nH, nW);

    ecrire_image_pgm("../out/reconstruction.pgm", reconstruction, nH, nW);

    std::cout << "psnr sans compression : " << ImageAlgorithms::psnr(ImgIn, reconstruction, 255, nTaille);

    free(ImgIn);
    free(ImgOut);
    free(reconstruction);



    // --------------------------- AVEC QUANTIFICATION ---------------------------



    /*OCTET *ImgBF, *ImgHV, *ImgHF, *ImgTHF, *temp1, *temp2;
    int nW2 = nW/2;
    int nH2 = nH/2;

    allocation_tableau(ImgBF, OCTET, nW2*nH2);
    allocation_tableau(ImgHV, OCTET, nW2*nH2);
    allocation_tableau(ImgHF, OCTET, nW2*nH2);
    allocation_tableau(ImgTHF, OCTET, nW2*nH2);

    ImageAlgorithms::haarWaveletDecompose(ImgIn, ImgBF, ImgHF, ImgHV, ImgTHF, nH, nW);


    ecrire_image_pgm("../out/ImgBF.pgm", ImgBF, nH/2, nW/2);
    ecrire_image_pgm("../out/ImgHF.pgm", ImgHF, nH/2, nW/2);
    ecrire_image_pgm("../out/ImgHV.pgm", ImgHV, nH/2, nW/2);
    ecrire_image_pgm("../out/ImgTHF.pgm", ImgTHF, nH/2, nW/2);

    //ImageAlgorithms::quantisizeColor(ImgHV, ImgHV, nH2, nW2, 4, 50, false);
    //ImageAlgorithms::quantisizeColor(ImgHF, ImgHF, nH2, nW2, 4, 50, false);
    //ImageAlgorithms::quantisizeColor(ImgTHF, ImgTHF, nH2, nW2, 2, 50, false);

    ImageAlgorithms::reconstruct_haarWavelet(reconstruction, ImgBF, ImgHF, ImgHV, ImgTHF, nH, nW);

    ecrire_image_pgm("../out/reconstruction.pgm", reconstruction, nH, nW);

    std::cout << "psnr avec compression : " << ImageAlgorithms::psnr(ImgIn, reconstruction, 255, nTaille);


    free(ImgIn);
    free(ImgOut);
    free(reconstruction);*/

    return 1;
}


