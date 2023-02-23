#ifndef FCT_GYOM
#define FCT_GYOM

#include "image_ppm.h"
#include <vector>
#include <iostream>
using namespace std;

// TEMPLATE
void temp(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW)
{
}
//---------------------------- HISTOGRAMME SEUILLAGE ETC. -------------------------------//

int *histogramme_pgm(OCTET *ImgIn, int nTaille)
{
    int *Histo;
    allocation_tableau(Histo, int, 255);
    // Generation de l'histo
    for (int i = 0; i < nTaille; i++)
    {
        Histo[ImgIn[i]]++; //Sauvegarde des occurences dans Histo
    }
    return Histo;
}

//---------------------------- SPECIFICATION, EXPANSION DYNAMIQUES, EGALISATION --------------------//
void expansion_pgm(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW)
{
    int nTaille = nW * nH;
    int *Histo, *Histo_Expanded;

    int maxGris = 255;
    allocation_tableau(Histo, int, maxGris);
    allocation_tableau(Histo_Expanded, int, maxGris);

    int a0 = INT8_MAX; //la borne inf
    int a1 = 0;        //la bonne sup
    //Comptage des niveau de gris et obtenir a0, a1 les bornes non nul
    for (int i = 0; i < nTaille; i++)
    {
        Histo[ImgIn[i]]++;
        //  Calcul de a0
        if (ImgIn[i] < a0)
        {
            a0 = ImgIn[i];
        }
        // Calcul de a1
        if (ImgIn[i] > a1)
        {
            a1 = ImgIn[i];
        }
    }

    std::cout << "Borne min a0 :" << a0 << std::endl;
    std::cout << "Borne min a1 :" << a1 << std::endl;

    //std::cout << " ----------------------------" << std::endl;
    int beta;
    int alpha;

    alpha = ((-255 * a0) / (a1 - a0));
    beta = (255 / (a1 - a0));

    std::cout << "Valeur du coefficient alpha -> " << alpha << std::endl;
    std::cout << "Valeur du coefficient beta -> " << beta << std::endl;

    //--expansion de l'image
    //Pour chaque pixel
    for (int i = 0; i < nH; i++)
    {
        for (int j = 0; j < nW; j++)
        {
            //Pixel courant
            int p = ImgIn[i * nW + j];
            //Pixel expanded
            int p_ = (a0 - p) * (-1 * (255 / (float)(a1 - a0)));
            ImgOut[i * nW + j] = p_;
            //Histo_Expanded[p_]++;
        }
    }
}

void expansion_ppm(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW)
{
    //-----Split en 3 plan gris
    int nTaille = nH * nW;
    int taille3 = 3 * nTaille;
    OCTET *ImgR, *ImgG, *ImgB; // 3 composantes

    allocation_tableau(ImgR, OCTET, nTaille);
    allocation_tableau(ImgG, OCTET, nTaille);
    allocation_tableau(ImgB, OCTET, nTaille);

    //Recuperer les plans R G et B
    planR(ImgR, ImgIn, nTaille);
    planV(ImgG, ImgIn, nTaille);
    planB(ImgB, ImgIn, nTaille);

    //----expansion de chaque plan
    OCTET *ImgR_, *ImgG_, *ImgB_;

    allocation_tableau(ImgR_, OCTET, nTaille);
    allocation_tableau(ImgG_, OCTET, nTaille);
    allocation_tableau(ImgB_, OCTET, nTaille);
    expansion_pgm(ImgR, ImgR_, nH, nW);
    expansion_pgm(ImgG, ImgG_, nH, nW);
    expansion_pgm(ImgB, ImgB_, nH, nW);

    //----fusion de chaque plan pour retourner en RGB - ppm

    int nR, nG, nB;
    //Recuperer les plans R G et B
    for (int i = 0; i < taille3; i += 3)
    {
        //Chaque couleur courante de chaque image
        nR = ImgR_[i / 3];
        nG = ImgG_[i / 3];
        nB = ImgB_[i / 3];

        ImgOut[i] = nR;

        ImgOut[i + 1] = nG;

        ImgOut[i + 2] = nB;
    }
}

void extrema_seuil(OCTET *ImgIn, OCTET *ImgOut, std::vector<int> min, std::vector<int> max, int nH, int nW, bool Color)
{
    bool color = Color;
    int nTaille = nH * nW;

    int facteur = color ? 3 : 1;
    int boucle = facteur;

    for (int i = 0; i < nTaille; i++) //Pour tout les pixel
    {
        for (int j = 0; j < boucle; j++) // Pour chaque composante
        {
            int id = facteur * i + j;  // id courant
            int current = ImgIn[id];   // Valeur pixel courant
            if (current < min[j])      // Si inf au min courant ( min R G ou B)
                ImgOut[id] = min[j];   // il est clamp a cette valeur
            else if (current > max[j]) // Pareil pour max
                ImgOut[id] = max[j];
            else
                ImgOut[id] = current; // Sinon il prends la valeur de l'image d'origine
        }
    }
}

float *densiteproba_pgm(OCTET *ImgIn, int nTaille)
{

    int *Histo = histogramme_pgm(ImgIn, nTaille); // Generation de l'histogramme
    float *DDP;
    allocation_tableau(DDP, float, 255);

    // Generation de l'histo
    for (int i = 0; i < 256; i++)
    {
        float ddp = Histo[i] / float(nTaille);
        DDP[i] = ddp;
    }
    return DDP;
}

float *Fonctionrepartion_pgm(OCTET *ImgIn, int nTaille)
{
    float *ddp = densiteproba_pgm(ImgIn, nTaille);
    float *FdR;
    float cumul = 0.; //
    allocation_tableau(FdR, float, 255);
    for (int i = 0; i < 256; i++)
    {
        cumul += ddp[i]; //Cumul des ddp
        FdR[i] = cumul;  // Stockage des valeurs de la fonciton de repartition dans FdR
    }

    return FdR;
}

void egalisation(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW)
{
    int nTaille = nW * nH;
    float *FdR = Fonctionrepartion_pgm(ImgIn, nTaille);
    for (int i = 0; i < nTaille; i++)
    {
        int p = ImgIn[i];
        int p_ = round(FdR[p] * 255);
        ImgOut[i] = p_;
    }
}

void specialisation(OCTET *ImgIn, OCTET *ImgRef, OCTET *ImgOut, int nH, int nW, int nHref, int nWref)
{
    int nTaille = nH * nW;
    int nTailleref = nHref * nWref;
    OCTET *ImgTemp;
    allocation_tableau(ImgTemp, OCTET, nTaille);
    // Egalisation avec la FdR de l'imageIn dans ImgTemp
    egalisation(ImgIn, ImgTemp, nH, nW);

    //Recupérer la fonction de repartition de l'image de reférence
    float *FdR_ref = Fonctionrepartion_pgm(ImgRef, nTailleref);
    int reverse_FdR_Reverse = 0;

    for (int i = 0; i < nTaille; i++) // Pour tout les pixels de ImgTemp
    {
        int p_ = ImgTemp[i];
        //On retrouve la valeur inverse de la fonction de repartition
        for (int j = 0; j < 256; j++) //Pour tout les niveau de gris
        {
            if (FdR_ref[j] >= ((float)p_ / 255)) // Dès qu'on dépasse le FdR_rer
            {
                reverse_FdR_Reverse = j; // On choppe le pixel correspondant as nouvelle intensité
                break;
            }
        }
        ImgOut[i] = reverse_FdR_Reverse;
    }
}

//

//---------------------------------- OPERATEUR MORPHIQUES -------------------------------//

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