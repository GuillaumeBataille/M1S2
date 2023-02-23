// test_couleur.cpp : Seuille une image couleur

#include <stdio.h>
#include <iostream>
#include "image_ppm.h"
#include <cstring>
#include <string>

//Fonction qui calcule le PSNR
float psnr(OCTET *ImgIn, OCTET *ImgOut, int maxSignal, int nTaille3)
{
    float eqm = 0; // erreur quadratique moyenne
    for (int i = 0; i < nTaille3; i++)
    {
        eqm += pow(ImgIn[i] - ImgOut[i], 2);
    }
    eqm /= nTaille3;

    float psnr = 10 * log10(pow(maxSignal, 2) / eqm);
    return psnr;
}

//Fonction qui convertit une image RGB en une image YCrCb
void RGBtoYCbCr(OCTET *ImgIn, OCTET *ImgOut, int taille)
{
    for (int i = 0; i < taille; i++)
    {

        ImgOut[3 * i] = std::min(255., std::max(0., 0.299 * ImgIn[3 * i] + 0.587 * ImgIn[3 * i + 1] + 0.114 * ImgIn[3 * i + 2]));
        ImgOut[3 * i + 1] = std::min(255., std::max(0., -0.1687 * ImgIn[3 * i] - 0.3313 * ImgIn[3 * i + 1] + 0.5 * ImgIn[3 * i + 2] + 128));
        ImgOut[3 * i + 2] = std::min(255., std::max(0., 0.5 * ImgIn[3 * i] - 0.4187 * ImgIn[3 * i + 1] - 0.0813 * ImgIn[3 * i + 2] + 128));
    }
}

//Fonction qui convertit une image YCrCb en RGB
void YCbCrtoRGB(OCTET *ImgIn, OCTET *ImgOut, int taille)
{
    for (int i = 0; i < taille; i++)
    {
        ImgOut[3 * i] = std::min(255., std::max(0., ImgIn[3 * i] + 1.402 * (ImgIn[3 * i + 2] - 128)));
        ImgOut[3 * i + 1] = std::min(255., std::max(0., ImgIn[3 * i] - 0.34414 * (ImgIn[3 * i + 1] - 128) - 0.71414 * (ImgIn[3 * i + 2] - 128)));
        ImgOut[3 * i + 2] = std::min(255., std::max(0., ImgIn[3 * i] + 1.772 * (ImgIn[3 * i + 1] - 128)));
    }
}

//-------------------------- MAIN -------------------------------
int main(int argc, char *argv[])
{
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille, nR, nG, nB;

    if (argc != 3)
    {
        printf("Usage: ImageIn.ppm ImageOut.ppm r/g/b(la composante qu'on garde) \n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);

    OCTET *ImgIn, *ImgIntermediaire, *baseY, *baseCr, *baseCb, *finalImg, *tinyCr, *tinyCb, *reconsCr, *reconsCb;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    int nTaille3 = nTaille * 3;

    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

    //allocation des tableau pgm des images reconstruites a partir des tiny
    allocation_tableau(baseY, OCTET, nTaille);
    allocation_tableau(baseCr, OCTET, nTaille);
    allocation_tableau(baseCb, OCTET, nTaille);

    //allocation des tableau des tiny Cr et tiny Cb
    allocation_tableau(tinyCr, OCTET, nTaille / 4);
    allocation_tableau(tinyCb, OCTET, nTaille / 4);

    //allocation des tableau reconstruction des tiny cr et cb
    allocation_tableau(reconsCr, OCTET, nTaille);
    allocation_tableau(reconsCb, OCTET, nTaille);

    //allocation de l'image finale et de l'image intermediaire
    allocation_tableau(ImgIntermediaire, OCTET, nTaille3);
    allocation_tableau(finalImg, OCTET, nTaille3);

    // ---------------------- Codage et compression --------------------

    //Changement d'espace couleur
    RGBtoYCbCr(ImgIn, ImgIntermediaire, nTaille);

    //Recup les plan Y Cr et Cb
    planR(baseY, ImgIntermediaire, nTaille);
    planV(baseCb, ImgIntermediaire, nTaille);
    planB(baseCr, ImgIntermediaire, nTaille);

    //Reduction de la taille des Cr et Cb par 2 dans tinyCr et tinyCb
    for (int i = 0; i < nH / 2; i++)
    {
        for (int j = 0; j < nW / 2; j++)
        {
            //*2 car l'image d'origine est 2 x plus large et haute
            int I = 2 * i;
            int J = 2 * j;

            //On somme le carré qu'on souhaite agréger pour le moyenner
            int valCr = baseCr[(I)*nW + J] + baseCr[(I + 1) * nW + (J)] + baseCr[(I)*nW + (J + 1)] + baseCr[(I + 1) * nW + (J + 1)];
            int valCb = baseCb[(I)*nW + J] + baseCb[(I + 1) * nW + (J)] + baseCb[(I)*nW + (J + 1)] + baseCb[(I + 1) * nW + (J + 1)];
            valCr /= 4;
            valCb /= 4;
            tinyCr[i * nW / 2 + j] = valCr;
            tinyCb[i * nW / 2 + j] = valCb;
        }
    }

    // Re-échantillonner les deux tiny images au bon format
    for (int i = 0; i < nH / 2; i++)
    {
        for (int j = 0; j < nW / 2; j++)
        {
            int valCr = tinyCr[i * nW / 2 + j]; // Le pixel courant G
            int valCb = tinyCb[i * nW / 2 + j]; // Le pixel courant G

            //*2 car l'image d'origine est 2 x plus large et haute
            int I = 2 * i;
            int J = 2 * j;
            //Composante CR
            reconsCr[(I)*nW + J] = valCr;
            reconsCr[(I + 1) * nW + (J)] = valCr;
            reconsCr[(I)*nW + (J + 1)] = valCr;
            reconsCr[(I + 1) * nW + (J + 1)] = valCr;

            //Composante CB
            reconsCb[(I)*nW + J] = valCb;
            reconsCb[(I + 1) * nW + (J)] = valCb;
            reconsCb[(I)*nW + (J + 1)] = valCb;
            reconsCb[(I + 1) * nW + (J + 1)] = valCb;
        }
    }

    // Remplir l'image intermediaire avec la baseY et les Cr et Cb re-échantillonné
    for (int i = 0; i < nTaille3; i += 3)
    {

        ImgIntermediaire[i] = baseY[i / 3];
        ImgIntermediaire[i + 1] = reconsCb[i / 3];
        ImgIntermediaire[i + 2] = reconsCr[i / 3];
    }

    //Changement final d'espace couleur pour une image RGB
    YCbCrtoRGB(ImgIntermediaire, finalImg, nTaille);

    // ------------- Ecriture -----------------//

    //Chemin et nom des fichiers a écrire

    //Les bases Y Cr et Cb
    std::string bY_string = "out_Ycrcb/base/baseY.pgm";
    char *bY = const_cast<char *>(bY_string.c_str());
    std::string bCr_string = "out_Ycrcb/base/baseCr.pgm";
    char *bCr = const_cast<char *>(bCr_string.c_str());
    std::string bCb_string = "out_Ycrcb/base/baseCb.pgm";
    char *bCb = const_cast<char *>(bCb_string.c_str());

    //Les tiny Cr et Cb
    std::string tCr_string = "out_Ycrcb/tiny/tinyCr.pgm";
    char *tCr = const_cast<char *>(tCr_string.c_str());
    std::string tCb_string = "out_Ycrcb/tiny/tinyCb.pgm";
    char *tCb = const_cast<char *>(tCb_string.c_str());

    //Les recons Cr et Cb

    std::string rCr_string = "out_Ycrcb/rec/recCr.pgm";
    char *recCr = const_cast<char *>(rCr_string.c_str());
    std::string rCb_string = "out_Ycrcb/rec/recCb.pgm";
    char *recCb = const_cast<char *>(rCb_string.c_str());

    // La version YCbCr
    std::string ycbcr_string = "out_Ycrcb/Ycrcb.pgm";
    char *ycbcr = const_cast<char *>(ycbcr_string.c_str());

    //Ecriture
    //Bases
    ecrire_image_pgm(bY, baseY, nH, nW);
    ecrire_image_pgm(bCr, baseCr, nH, nW);
    ecrire_image_pgm(bCb, baseCb, nH, nW);
    //Tiny
    ecrire_image_pgm(tCr, tinyCr, nH / 2, nW / 2);
    ecrire_image_pgm(tCb, tinyCb, nH / 2, nW / 2);
    //Reconstruct
    ecrire_image_pgm(recCr, reconsCr, nH, nW);
    ecrire_image_pgm(recCb, reconsCb, nH, nW);

    //Final et version YCbcr
    ecrire_image_ppm(cNomImgEcrite, finalImg, nH, nW);
    RGBtoYCbCr(ImgIn, ImgIntermediaire, nTaille); // On rééecrit dedans car on l'a overwrite
    ecrire_image_ppm(ycbcr, ImgIntermediaire, nH, nW);

    std::cout << "PSNR = " << psnr(ImgIn, finalImg, 255, nTaille3) << std::endl;

    free(ImgIn);
    return 1;
}
