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
    int nH, nW, nTaille, nR, nG, nB, id1, id2;

    if (argc != 3)
    {
        printf("Usage: ImageIn.ppm ImgOut.ppm\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);

    std::string imglue = cNomImgLue;
    std::string imgecrite = cNomImgEcrite;

    OCTET *ImgIn, *ImgOut, *ImgOut_final;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille3);

    //Trouver deux couleurs distantes
    Color c1, c2;
    ImageAlgorithms::findDistantColor(c1, c2, ImgIn, nTaille3, 200);

    std::cout << "c1 : " << c1.toString() << std::endl;
    std::cout << "c2 : " << c2.toString() << std::endl;
    std::vector<Color> classes = {c1, c2};

    // classement des pixel entre les deux classes

    for (int i = 0; i < nTaille3; i += 3)
    {
        Color c(ImgIn[i], ImgIn[i + 1], ImgIn[i + 2]);

        int colorClass = ImageAlgorithms::findClass(c, classes);

        ImgOut[i] = classes[colorClass].r;
        ImgOut[i + 1] = classes[colorClass].g;
        ImgOut[i + 2] = classes[colorClass].b;
    }

    ecrire_image_ppm("out/1/valeur_distante_out.ppm", ImgOut, nH, nW);

    ImageAlgorithms::k_mean(ImgIn, classes, nTaille3, 10);

    for (int i = 0; i < nTaille3; i += 3)
    {
        Color c(ImgIn[i], ImgIn[i + 1], ImgIn[i + 2]);

        int colorClass = ImageAlgorithms::findClass(c, classes);

        ImgOut[i] = classes[colorClass].r;
        ImgOut[i + 1] = classes[colorClass].g;
        ImgOut[i + 2] = classes[colorClass].b;
    }

    ecrire_image_ppm("out/1/valeurmoyenne_out.ppm", ImgOut, nH, nW);

    std::cout << "PSNR = " << ImageAlgorithms::psnr(ImgIn, ImgOut, 255, nTaille3) << std::endl;
    std::cout << "nH :" << nH << " et nW: " << nW << std::endl;
    OCTET *Palette;
    ImageAlgorithms::createColorPalette_rgb(Palette, ImgOut, nTaille3, nW, nH);
    ecrire_image_ppm("out/1/palette_out.ppm", Palette, 1, 2);

    free(ImgIn);
    return 1;
}
