#include <stdio.h>
#include <algorithm>
#include "fct_gyom.h"
#include "image_ppm.h"
#include <cstring>
using namespace std;

int main(int argc, char *argv[])
{
    char cNomImgLue[250];
    char cNomImgEcrite[250];
    int nH, nW, nTaille;

    if (argc != 4)
    {
        printf("Usage: ImageIn.p(gp)m ImageOut.p(gp)m Type\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);

    char ext[4] = "ppm";
    bool color = strcmp(cNomImgLue + strlen(cNomImgLue) - 3, ext) == 0;

    OCTET *ImgIn, *ImgOut;

    if (color)
        lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    else
        lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);

    nTaille = color ? nH * nW * 3 : nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    allocation_tableau(ImgOut, OCTET, nTaille);

    if (color)
        lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
    else
        lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

    switch (atoi(argv[3]))
    {
    case 1:
        erosion(ImgIn, ImgOut, nH, nW, color);
        break;
    case 2:
        dilatation(ImgIn, ImgOut, nH, nW, color);
        break;
    case 3:
        fermeture(ImgIn, ImgOut, nH, nW, color);
        break;
    case 4:
        ouverture(ImgIn, ImgOut, nH, nW, color);
        break;
    case 5:
        difference(ImgIn, ImgOut, nH, nW, color);
        break;
    default:
        printf("Param type doit être 1: erosion, 2: dilatation, 3: fermeture, 4: ouverture, 5: difference");
        break;
    }

    if (color)
        ecrire_image_ppm(cNomImgEcrite, ImgOut, nH, nW);
    else
        ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);

    free(ImgIn);
    free(ImgOut);

    return 1;
}
