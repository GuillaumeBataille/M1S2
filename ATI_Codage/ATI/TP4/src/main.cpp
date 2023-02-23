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
    if (argc != 3)
    {
        printf("Usage: ImageIn.ppm Seuil\n");
        exit(1);
    }

    std::string inputName = argv[1];
    inputName = "../in/" + inputName;
    int nH, nW, nTaille, S;
    S = atoi(argv[2]);

    OCTET *ImgIn, *ImgOut, *ImgGrey, *Imgnb, *Imgflou;

    bool color = !(inputName.substr(inputName.size() - 3, 3)).compare("ppm");
    std::cout << "Image " << (color ? "couleur" : "niveau de gris") << " en input" << std::endl;

    (color ? lire_nb_lignes_colonnes_image_ppm(inputName, &nH, &nW) : lire_nb_lignes_colonnes_image_pgm(inputName, &nH, &nW));
    nTaille = nH * nW;
    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);

    allocation_tableau(ImgOut, OCTET, nTaille3);

    allocation_tableau(ImgGrey, OCTET, nTaille);

    allocation_tableau(Imgnb, OCTET, nTaille);

    lire_image_ppm(inputName, ImgIn, nH * nW);

    // -------------------- Traitement --------------------
    //1.Transformation de l'image pgm en image ppm
    ImageAlgorithms::ppmtopgm(ImgIn, ImgGrey, nH, nW);
    ecrire_image_pgm("../out/ppmtopgm.pgm", ImgGrey, nH, nW);
    ImageAlgorithms::writeHistoDatFile(ImgGrey, nTaille, "../dat/Histo.dat", false);

    //2.Seuillage pour dissocier le fond de l'objet

    ImageAlgorithms::seuillage(ImgGrey, Imgnb, nH, nW, S);
    ecrire_image_pgm("../out/seuilled.pgm", Imgnb, nH, nW);

    //3.Flou sur toute l'image
    //Gris in et out
    allocation_tableau(Imgflou, OCTET, nTaille);
    ImageAlgorithms::flou(ImgGrey, Imgflou, nH, nW, false);
    ecrire_image_pgm("../out/flougrey.pgm", Imgflou, nH, nW);
    //Couleur in et out
    allocation_tableau(Imgflou, OCTET, nTaille3);
    ImageAlgorithms::flou(ImgIn, Imgflou, nH, nW, true);
    ecrire_image_ppm("../out/floucolor.ppm", Imgflou, nH, nW);

    //4.Flou du background uniquement
    ImageAlgorithms::flou_background(ImgIn, Imgnb, ImgOut, nH, nW, color);
    ecrire_image_ppm("../out/floubackground.ppm", ImgOut, nH, nW);

    //5.a Erosion et Dilatation de l'image seuillé
    ImageAlgorithms::opening_nonBinary(Imgnb, Imgnb, nH, nW, 0, 0);
    ImageAlgorithms::closing_nonBinary(Imgnb, Imgnb, nH, nW, 0, 0);
    ImageAlgorithms::opening_nonBinary(Imgnb, Imgnb, nH, nW, 0, 0);
    ImageAlgorithms::closing_nonBinary(Imgnb, Imgnb, nH, nW, 0, 0);
    ecrire_image_pgm("../out/post_erodila.ppm", Imgnb, nH, nW);

    //5.b Flou du background avec l'image seuillée
    ImageAlgorithms::flou_background(ImgIn, Imgnb, ImgOut, nH, nW, color);
    ecrire_image_ppm("../out/floubackgroundBis.ppm", ImgOut, nH, nW);

    //6.courbe ROC, carte de vérité a dessiner sur gimp
    std::string truthName = "../in/kfc_verite.pgm";
    OCTET *Imgtruth;
    allocation_tableau(Imgtruth, OCTET, nH * nW);
    lire_image_pgm(truthName, Imgtruth, nTaille);
    ImageAlgorithms::writeROCDatFile(ImgGrey, nTaille, "../dat/ROC.dat", Imgtruth, nH, nW);
    ecrire_image_pgm("../out/testtruth.pgm", ImgGrey, nH, nW);
    free(ImgIn);
    free(ImgOut);
    free(Imgflou);
    free(Imgnb);

    return 1;
}
