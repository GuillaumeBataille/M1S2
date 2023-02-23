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
    std::string inputName = argv[1], outpuName = argv[2];
    inputName = "../in/" + inputName;
    int nH, nW, nTaille, S;
    int nbColor = std::stoi(argv[3]);

    if (argc != 4)
    {
        printf("Usage: ImageIn.ppm ImageOut.ppm nbColor\n");
        exit(1);
    }

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(inputName, &nH, &nW);
    nTaille = nH * nW;

    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(inputName, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille3);

    // -------------------- Traitement --------------------

    std::vector<Color> classes = ImageAlgorithms::findRandomColors(ImgIn, nbColor, nW, nH);

    for (int i = 0; i < nTaille3; i += 3)
    {
        Color c(ImgIn[i], ImgIn[i+1], ImgIn[i+2]);

        int colorClass = ImageAlgorithms::findClass(c, classes);

        ImgOut[i] = classes[colorClass].r;
        ImgOut[i+1] = classes[colorClass].g;
        ImgOut[i+2] = classes[colorClass].b;
    }

    ecrire_image_ppm("../out/NoKmean_" + std::to_string(nbColor) + "_" + outpuName, ImgOut, nH, nW);

    ImageAlgorithms::k_mean(ImgIn, classes, nTaille3, 10);

    for (int i = 0; i < nTaille3; i += 3)
    {
        Color c(ImgIn[i], ImgIn[i+1], ImgIn[i+2]);

        int colorClass = ImageAlgorithms::findClass(c, classes);

        ImgOut[i] = classes[colorClass].r;
        ImgOut[i+1] = classes[colorClass].g;
        ImgOut[i+2] = classes[colorClass].b;
    }

    ecrire_image_ppm("../out/Kmean_" + std::to_string(nbColor) + "_" + outpuName, ImgOut, nH, nW);

    std::cout << "PSNR = " << ImageAlgorithms::psnr(ImgIn, ImgOut, 255, nTaille3) << std::endl;

    free(ImgIn);
    return 1;
}
