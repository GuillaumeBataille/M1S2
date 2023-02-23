// test_couleur.cpp : Seuille une image couleur

#include <stdio.h>
#include <iostream>
#include <vector>
#include "image_ppm.h"
#include "color.h"
#include "ImageAlgorithms.h"

int main(int argc, char *argv[])
{
    std::string inputName = argv[1], outpuName = argv[2];
    inputName = "../in/" + inputName;
    int nH, nW, nTaille, S;

    if (argc != 3)
    {
        printf("Usage: ImageIn.ppm ImageOut.ppm\n");
        exit(1);
    }

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(inputName, &nH, &nW);
    nTaille = nH * nW;

    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(inputName, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille3);

    Color c1, c2;
    ImageAlgorithms::findDistantColor(c1, c2, ImgIn, nTaille3, 200);

    std::cout << "c1 : " << c1.toString() << std::endl;
    std::cout << "c2 : " << c2.toString() << std::endl;
    std::vector<Color> classes = {c1, c2};

    // classement des pixel entre les deux classes

    for (int i = 0; i < nTaille3; i += 3)
    {
        Color c(ImgIn[i], ImgIn[i+1], ImgIn[i+2]);

        int colorClass = ImageAlgorithms::findClass(c, classes);

        ImgOut[i] = classes[colorClass].r;
        ImgOut[i+1] = classes[colorClass].g;
        ImgOut[i+2] = classes[colorClass].b;
    }

    ecrire_image_ppm("../out/ValeursDistantes_" + outpuName, ImgOut, nH, nW);

    ImageAlgorithms::k_mean(ImgIn, classes, nTaille3, 50);

    for (int i = 0; i < nTaille3; i += 3)
    {
        Color c(ImgIn[i], ImgIn[i+1], ImgIn[i+2]);

        int colorClass = ImageAlgorithms::findClass(c, classes);

        ImgOut[i] = classes[colorClass].r;
        ImgOut[i+1] = classes[colorClass].g;
        ImgOut[i+2] = classes[colorClass].b;
    }

    ecrire_image_ppm("../out/ValeursMoyenne_" + outpuName, ImgOut, nH, nW);

    std::cout << "PSNR = " << ImageAlgorithms::psnr(ImgIn, ImgOut, 255, nTaille3) << std::endl;

    free(ImgIn);
    return 1;
}
