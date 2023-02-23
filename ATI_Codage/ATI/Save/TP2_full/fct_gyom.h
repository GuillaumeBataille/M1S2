#ifndef FCT_GYOM
#define FCT_GYOM

#include "image_ppm.h"
#include <iostream>
using namespace std;

// Dilation noir et blanc pour les exos 1 a 4
void dilationnb(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW)
{
    //Pour chaque pixel
    for (int i = 0; i < nH; i++)
    {
        for (int j = 0; j < nW; j++)
        {
            int M = 255; // La valeur de comparaison est 255 (blanc)
            //On regarde tout le voisinage
            for (int y = max(i - 1, 0); y <= min(i + 1, nH - 1); y++)
            {
                for (int x = max(j - 1, 0); x <= min(j + 1, nW - 1); x++)
                {
                    //On prends la valeur la plus petite entre 255 et la valeur du voisinage pour tout le voisinage
                    //Donc si un des pixel du voisinage est noir alors le pixel sera noir
                    M = min(M, (int)ImgIn[y * nW + x]);
                }
            }
            ImgOut[i * nW + j] = M;
        }
    }
}

// Erosion noir et blanc pour les exos 1 a 4
void erosionnb(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW)
{
    for (int i = 0; i < nH; i++)
    {
        for (int j = 0; j < nW; j++)
        {
            int M = 0;
            for (int y = max(i - 1, 0); y <= min(i + 1, nH - 1); y++)
            {
                for (int x = max(j - 1, 0); x <= min(j + 1, nW - 1); x++)
                {
                    M = max(M, (int)ImgIn[y * nW + x]);
                }
            }
            ImgOut[i * nW + j] = M;
        }
    }
}
//Fonction pour remplir imgIn avec le resultat de imgout pour un exercice
void refill(OCTET *IA, OCTET *IB, int taille)
{
    for (int i = 0; i < taille; i++)
    {
        IA[i] = IB[i];
    }
};

//----------------------- Partie Exo5 -------------------------------------------
void erosion(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW, bool color)
{
    //Check si on est sur tu greyscale ou pas
    int facteur = color ? 3 : 1;
    int boucle = color ? 3 : 1;
    //Boucle sur chaque pixel de l'image
    for (int i = 0; i < nH; i++)
    {
        for (int j = 0; j < nW; j++)
        {
            for (int k = 0; k < boucle; k++)
            {
                int M = 0;
                //boucle sur tout le voisinage de i-1 j -1 a i+1 j+1 en skippant les bords de l'image
                for (int y = max(i - 1, 0); y <= min(i + 1, nH - 1); y++)
                {
                    for (int x = max(j - 1, 0); x <= min(j + 1, nW - 1); x++)
                    {
                        M = max(M, (int)ImgIn[facteur * (y * nW + x) + k]); // Comme on érode on garde le max
                    }
                }
                ImgOut[facteur * (i * nW + j) + k] = M;
            }
        }
    }
}

void dilatation(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW, bool color)
{
    int facteur = color ? 3 : 1;
    int boucle = color ? 3 : 1;

    for (int i = 0; i < nH; i++)
    {
        for (int j = 0; j < nW; j++)
        {
            for (int k = 0; k < boucle; k++)
            {
                int m = 255;
                for (int y = max(i - 1, 0); y <= min(i + 1, nH - 1); y++)
                {
                    for (int x = max(j - 1, 0); x <= min(j + 1, nW - 1); x++)
                    {
                        m = min(m, (int)ImgIn[facteur * (y * nW + x) + k]); // Comme on dilate on garde le mi
                    }
                }
                ImgOut[facteur * (i * nW + j) + k] = m;
            }
        }
    }
}

void fermeture(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW, bool color)
{
    OCTET *ImgTemp;
    allocation_tableau(ImgTemp, OCTET, nW * nH * (color ? 3 : 1));

    dilatation(ImgIn, ImgTemp, nH, nW, color);
    erosion(ImgTemp, ImgOut, nH, nW, color);

    free(ImgTemp);
}

void ouverture(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW, bool color)
{
    OCTET *ImgTemp;
    allocation_tableau(ImgTemp, OCTET, nW * nH * (color ? 3 : 1));

    erosion(ImgIn, ImgTemp, nH, nW, color);
    dilatation(ImgTemp, ImgOut, nH, nW, color);

    free(ImgTemp);
}

void difference(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW, bool color)
{
    OCTET *ImgEro;
    OCTET *ImgDil;
    allocation_tableau(ImgEro, OCTET, nW * nH * (color ? 3 : 1));
    allocation_tableau(ImgDil, OCTET, nW * nH * (color ? 3 : 1));

    erosion(ImgIn, ImgEro, nH, nW, color);
    dilatation(ImgIn, ImgDil, nH, nW, color);

    int S = 235;

    for (int i = 0; i < nW * nH * (color ? 3 : 1); ++i)
    {
        int val = 255 - (abs(ImgDil[i] - ImgEro[i])) / 2;
        ImgOut[i] = (val < S ? 0 : 255);
    }

    free(ImgEro);
    free(ImgDil);
}
#endif