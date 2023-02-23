// test_couleur.cpp : Seuille une image couleur

#include <stdio.h>
#include <iostream>
#include <vector>
#include "image_ppm.h"
#include "ImageAlgorithms.h"
#include "color.h"

int main(int argc, char *argv[])
{
    char cNomImgLue[250], cNomImgEcrite[250], cNomImg2[250];
    int nH, nW, nTaille, nbcouleur;

    if (argc != 4)
    {
        printf("Usage: ImageIn.ppm ImgOut.ppm Nbcouleur\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);

    nbcouleur = std::stoi(argv[3]);

    std::string imglue = cNomImgLue;
    std::string imgecrite = cNomImgEcrite;

    OCTET *ImgIn, *ImgOut, *ImgOut_final;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille3);

    //Trouver nbCouleur classes - random

    std::vector<Color> classes = ImageAlgorithms::findRandomColors(ImgIn, nbcouleur, nW, nH);

    // classement des pixel entre les nbcouleurs classes

    for (int i = 0; i < nTaille3; i += 3)
    {
        Color c(ImgIn[i], ImgIn[i + 1], ImgIn[i + 2]);

        int colorClass = ImageAlgorithms::findClass(c, classes);

        ImgOut[i] = classes[colorClass].r;
        ImgOut[i + 1] = classes[colorClass].g;
        ImgOut[i + 2] = classes[colorClass].b;
    }

    ecrire_image_ppm("out/2/nbcouleurclasse.ppm", ImgOut, nH, nW);

    ImageAlgorithms::k_mean(ImgIn, classes, nTaille3, 10);

    for (int i = 0; i < nTaille3; i += 3)
    {
        Color c(ImgIn[i], ImgIn[i + 1], ImgIn[i + 2]);

        int colorClass = ImageAlgorithms::findClass(c, classes);
        //std::cout << "colorClass id " << colorClass << std::endl;
        ImgOut[i] = classes[colorClass].r;
        ImgOut[i + 1] = classes[colorClass].g;
        ImgOut[i + 2] = classes[colorClass].b;
    }

    ecrire_image_ppm("out/2/valeurmoyenne2_out.ppm", ImgOut, nH, nW);

    std::cout << "PSNR = " << ImageAlgorithms::psnr(ImgIn, ImgOut, 255, nTaille3) << std::endl;

    OCTET *Palette;
    int l, L;
    ImageAlgorithms::createColorPalette_rgb(Palette, ImgOut, nTaille3, l, L);
    ecrire_image_ppm("out/2/palette2_out.ppm", Palette, l, L);

    //Conversion du ppm 256 classe-palette ----> pgm avec comme greylvl l'index de la palette
    OCTET *Imggrey;
    allocation_tableau(Imggrey, OCTET, nTaille);
    for (int i = 0; i < nTaille3; i += 3)
    {
        Color c(ImgIn[i], ImgIn[i + 1], ImgIn[i + 2]);

        int colorClass = ImageAlgorithms::findClass(c, classes);

        Imggrey[i / 3] = colorClass;
    }
    ecrire_image_pgm("out/2/greymod_out.ppm", Imggrey, nH, nW);
    std::cout << "PSNR = " << ImageAlgorithms::psnr(ImgIn, ImgOut, 255, nTaille3) << std::endl;

    free(ImgIn);
    return 1;
}
