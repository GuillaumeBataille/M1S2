// test_couleur.cpp : Seuille une image couleur

#include <stdio.h>
#include <iostream>
#include "image_ppm.h"
#include <cstring>
#include <string>

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

int main(int argc, char *argv[])
{
    char cNomImgLue[250], cNomImgEcrite[250];
    char _RGB_[10];
    int nH, nW, nTaille, nR, nG, nB;

    if (argc != 4)
    {
        printf("Usage: ImageIn.ppm ImageOut.ppm r/g/b(la composante qu'on garde) \n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);
    sscanf(argv[3], "%s", _RGB_);

    if (strcmp(_RGB_, "r") && strcmp(_RGB_, "g") && strcmp(_RGB_, "b"))
    {
        printf("r ou g ou b en 3eme arg\n");
        exit(1);
    }

    std::cout << "On conserve la composante " << _RGB_ << std::endl;

    OCTET *ImgIn, *ImgOut, *baseR, *baseB, *baseG, *tinyR, *tinyG, *tinyB, *reconsR, *reconsG, *reconsB;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    int nTaille3 = nTaille * 3;

    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille3);

    //allocation des tableau pgm des bases R G B
    allocation_tableau(baseR, OCTET, nTaille);
    allocation_tableau(baseG, OCTET, nTaille);
    allocation_tableau(baseB, OCTET, nTaille);

    //allocation des tableau pgm de tinyR, tinyG, tinyB
    allocation_tableau(tinyR, OCTET, nTaille / 4);
    allocation_tableau(tinyG, OCTET, nTaille / 4);
    allocation_tableau(tinyB, OCTET, nTaille / 4);

    //allocation des tableau pgm des images reconstruites a partir des tiny
    allocation_tableau(reconsR, OCTET, nTaille);
    allocation_tableau(reconsG, OCTET, nTaille);
    allocation_tableau(reconsB, OCTET, nTaille);

    //Génération des 3 pgm

    planR(baseR, ImgIn, nTaille);
    planV(baseG, ImgIn, nTaille);
    planB(baseB, ImgIn, nTaille);

    //Reduction de la taille des images par 4
    for (int i = 0; i < nH / 2; i++)
    {
        for (int j = 0; j < nW / 2; j++)
        {
            //*2 car l'image d'origine est 2 x plus large et haute
            int I = 2 * i;
            int J = 2 * j;

            //On somme le carré qu'on souhaite agréger pour le moyenner
            int valG = baseG[(I)*nW + J] + baseG[(I + 1) * nW + (J)] + baseG[(I)*nW + (J + 1)] + baseG[(I + 1) * nW + (J + 1)];
            int valR = baseR[(I)*nW + J] + baseR[(I + 1) * nW + (J)] + baseR[(I)*nW + (J + 1)] + baseR[(I + 1) * nW + (J + 1)];
            int valB = baseB[(I)*nW + J] + baseB[(I + 1) * nW + (J)] + baseB[(I)*nW + (J + 1)] + baseB[(I + 1) * nW + (J + 1)];
            valR /= 4;
            valG /= 4;
            valB /= 4;
            tinyR[i * nW / 2 + j] = valR;
            tinyG[i * nW / 2 + j] = valG;
            tinyB[i * nW / 2 + j] = valB;
        }
    }

    // Retransformée en image du bon format
    for (int i = 0; i < nH / 2; i++)
    {
        for (int j = 0; j < nW / 2; j++)
        {
            int valR = tinyR[i * nW / 2 + j]; // Le pixel courant G
            int valG = tinyG[i * nW / 2 + j]; // Le pixel courant G
            int valB = tinyB[i * nW / 2 + j]; // Le pixel courant G

            //*2 car l'image d'origine est 2 x plus large et haute
            int I = 2 * i;
            int J = 2 * j;
            //Composante R
            reconsR[(I)*nW + J] = valR;
            reconsR[(I + 1) * nW + (J)] = valR;
            reconsR[(I)*nW + (J + 1)] = valR;
            reconsR[(I + 1) * nW + (J + 1)] = valR;

            //Composante G
            reconsG[(I)*nW + J] = valG;
            reconsG[(I + 1) * nW + (J)] = valG;
            reconsG[(I)*nW + (J + 1)] = valG;
            reconsG[(I + 1) * nW + (J + 1)] = valG;

            //Composante B
            reconsB[(I)*nW + J] = valB;
            reconsB[(I + 1) * nW + (J)] = valB;
            reconsB[(I)*nW + (J + 1)] = valB;
            reconsB[(I + 1) * nW + (J + 1)] = valB;
        }
    }

    // Ecriture finale
    for (int i = 0; i < nTaille3; i += 3)
    {
        switch (_RGB_[0])
        {
        case 'r':                     // On conserve R
            ImgOut[i] = baseR[i / 3]; //<-----
            ImgOut[i + 1] = reconsG[i / 3];
            ImgOut[i + 2] = reconsB[i / 3];
            break;

        case 'g': // On conserve G
            ImgOut[i] = reconsR[i / 3];
            ImgOut[i + 1] = baseG[i / 3]; //<-----
            ImgOut[i + 2] = reconsB[i / 3];
            break;

        case 'b': // On conserve B
            ImgOut[i] = reconsR[i / 3];
            ImgOut[i + 1] = reconsG[i / 3];
            ImgOut[i + 2] = baseB[i / 3]; //<-----
            break;

        default:
            break;
        }
    }

    //Declaration des noms de fichiers base, tiny et reconstruct
    //Base
    std::string br_string = "out/base/baseR.pgm";
    char *br = const_cast<char *>(br_string.c_str());

    std::string bg_string = "out/base/baseG.pgm";
    char *bg = const_cast<char *>(bg_string.c_str());

    std::string bb_string = "out/base/baseB.pgm";
    char *bb = const_cast<char *>(bb_string.c_str());

    //Tiny
    std::string tr_string = "out/tiny/tinyR.pgm";
    char *tr = const_cast<char *>(tr_string.c_str());

    std::string tg_string = "out/tiny/tinyG.pgm";
    char *tg = const_cast<char *>(tg_string.c_str());

    std::string tb_string = "out/tiny/tinyB.pgm";
    char *tb = const_cast<char *>(tb_string.c_str());

    //Reconstruct
    std::string rr_string = "out/reconstruct/reconsR.pgm";
    char *rr = const_cast<char *>(rr_string.c_str());

    std::string rg_string = "out/reconstruct/reconsG.pgm";
    char *rg = const_cast<char *>(rg_string.c_str());

    std::string rb_string = "out/reconstruct/reconsB.pgm";
    char *rb = const_cast<char *>(rb_string.c_str());

    // ------------- Ecriture -----------------//

    //Les compostantes de base en grey-levels
    ecrire_image_pgm(br, baseR, nH, nW);
    ecrire_image_pgm(bg, baseG, nH, nW);
    ecrire_image_pgm(bb, baseB, nH, nW);

    //Les composantes version petite (divisé par 2) en grey level
    ecrire_image_pgm(tr, tinyR, nH / 2, nW / 2);
    ecrire_image_pgm(tg, tinyG, nH / 2, nW / 2);
    ecrire_image_pgm(tb, tinyB, nH / 2, nW / 2);

    //Les composantes reconstruites à partir des petites en grey level
    ecrire_image_pgm(rr, reconsR, nH, nW);
    ecrire_image_pgm(rg, reconsG, nH, nW);
    ecrire_image_pgm(rb, reconsB, nH, nW);

    //Image finale compréssée par 2
    ecrire_image_ppm(cNomImgEcrite, ImgOut, nH, nW);

    std::cout << "PSNR = " << psnr(ImgIn, ImgOut, 255, nTaille3) << std::endl;
    std::cout << " nW: " << nW << " et nH : " << nH << std::endl;

    free(ImgIn);
    return 1;
}
