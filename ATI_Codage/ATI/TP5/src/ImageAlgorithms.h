//
// Created by bat-portable on 21/01/2023.
//

#ifndef CODAGE_IMAGEALGORITHMS_H
#define CODAGE_IMAGEALGORITHMS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <random>
#include "color.h"

namespace ImageAlgorithms
{
    int findClass(Color &c, std::vector<Color> classes)
    {
        float distanceMin = std::numeric_limits<float>::max();
        int index;

        for (int colorIndex = 0; colorIndex < classes.size(); colorIndex++)
        {
            float d = c.dist(classes[colorIndex]);
            if (d < distanceMin)
            {
                distanceMin = d;
                index = colorIndex;
            }
        }

        return index;
    }

    void findDistantColor(Color &c1, Color &c2, OCTET *ImgIn, int nTaille, int precision)
    {
        int nTaille3 = nTaille * 3;

        float distanceMax = 0;
        precision *= 3;

        for (int i = 0; i < nTaille3; i += precision)
        {
            Color temp_c1(ImgIn[i], ImgIn[i + 1], ImgIn[i + 2]);

            for (int j = 0; j < nTaille3; j += precision)
            {
                Color temp_c2(ImgIn[j], ImgIn[j + 1], ImgIn[j + 2]);

                float distance = temp_c1.dist(temp_c2);

                if (distance > distanceMax)
                {
                    distanceMax = distance;
                    c1 = temp_c1;
                    c2 = temp_c2;
                }
            }
        }
    }

    std::vector<Color> findRandomColors(OCTET *ImgIn, int number, int width, int height)
    {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> distWidth(0, width);
        std::uniform_int_distribution<std::mt19937::result_type> distHeight(0, height);

        std::vector<Color> tab;

        for (int i = 0; i < number; i++)
        {
            int randWidth = distWidth(rng);
            int randHeight = distHeight(rng);

            int index = randWidth * 3 + width * randHeight * 3;

            Color c(ImgIn[index], ImgIn[index + 1], ImgIn[index + 2]);
            tab.push_back(c);
        }
        return tab;
    }

    void k_mean(OCTET *ImgIn, std::vector<Color> &classes, int nTaille, int repetition)
    {
        int nTaille3 = nTaille * 3;

        for (int k = 0; k < repetition; k++)
        {
            std::vector<Color_float> avgColorPerClasses = std::vector<Color_float>(classes.size());
            std::vector<int> countForAvgClasses = std::vector<int>(classes.size());

            for (int i = 0; i < nTaille3; i += 3)
            {
                Color actualColor(ImgIn[i], ImgIn[i + 1], ImgIn[i + 2]);
                float distanceMin = std::numeric_limits<float>::max();
                int index;

                for (int colorIndex = 0; colorIndex < classes.size(); colorIndex++)
                {
                    float d = actualColor.dist(classes[colorIndex]);
                    if (d < distanceMin)
                    {
                        distanceMin = d;
                        index = colorIndex;
                    }
                }
                avgColorPerClasses[index] = avgColorPerClasses[index] + actualColor;
                countForAvgClasses[index] = countForAvgClasses[index] + 1;
            }

            for (int i = 0; i < avgColorPerClasses.size(); i++)
            {
                classes[i] = (avgColorPerClasses[i] / countForAvgClasses[i]).convertToInt();
            }
        }
    }

    float psnr(OCTET *ImgIn, OCTET *ImgOut, int maxSignal, int arraySize)
    {
        float eqm; // erreur quadratique
        for (int i = 0; i < arraySize; i++)
        {
            eqm += pow(ImgIn[i] - ImgOut[i], 2);
        }
        eqm /= arraySize;

        float a = 10 * log10(static_cast<float>(pow(maxSignal, 2)) / eqm);
        return a;
    }

    std::vector<Color> colorPalette_rgb(OCTET *Img, int nTaille)
    {
        int nTaille3 = nTaille * 3;

        std::vector<Color> tab;
        bool isIn;

        for (int i = 0; i < nTaille3; i = i + 3)
        {
            Color c(Img[i], Img[i + 1], Img[i + 2]);

            isIn = false;

            for (Color c2 : tab)
            {
                if (c == c2)
                {
                    isIn = true;
                }
            }

            if (!isIn)
            {
                tab.push_back(c);
            }
        }
        return tab;
    }

    void createColorPalette_rgb(OCTET *&Palette, OCTET *Img, int nTaille, int &nW, int &nH)
    {
        int nTaille3 = nTaille * 3;

        std::vector<Color> palette = colorPalette_rgb(Img, nTaille3);

        int tailleImg_x = ceil(sqrt(palette.size()));
        int tailleImg = tailleImg_x * tailleImg_x;
        int tailleImg3 = tailleImg * 3;
        nW = tailleImg_x;
        nH = tailleImg_x;

        allocation_tableau(Palette, OCTET, tailleImg3);

        int index = 0;
        for (int i = 0; i < palette.size(); i++)
        {
            Palette[index] = palette[i].r;
            Palette[index + 1] = palette[i].g;
            Palette[index + 2] = palette[i].b;
            index = index + 3;
        }
    }

    void RGBtoYCbCr(OCTET *ImgIn, OCTET *ImgOut, int taille)
    {
        for (int i = 0; i < taille; i++)
        {

            ImgOut[3 * i] = std::min(255., std::max(0., 0.299 * ImgIn[3 * i] + 0.587 * ImgIn[3 * i + 1] + 0.114 * ImgIn[3 * i + 2]));
            ImgOut[3 * i + 1] = std::min(255., std::max(0., -0.1687 * ImgIn[3 * i] - 0.3313 * ImgIn[3 * i + 1] + 0.5 * ImgIn[3 * i + 2] + 128));
            ImgOut[3 * i + 2] = std::min(255., std::max(0., 0.5 * ImgIn[3 * i] - 0.4187 * ImgIn[3 * i + 1] - 0.0813 * ImgIn[3 * i + 2] + 128));
        }
    }

    void YCbCrtoRGB(OCTET *ImgIn, OCTET *ImgOut, int taille)
    {
        for (int i = 0; i < taille; i++)
        {
            ImgOut[3 * i] = std::min(255., std::max(0., ImgIn[3 * i] + 1.402 * (ImgIn[3 * i + 2] - 128)));
            ImgOut[3 * i + 1] = std::min(255., std::max(0., ImgIn[3 * i] - 0.34414 * (ImgIn[3 * i + 1] - 128) - 0.71414 * (ImgIn[3 * i + 2] - 128)));
            ImgOut[3 * i + 2] = std::min(255., std::max(0., ImgIn[3 * i] + 1.772 * (ImgIn[3 * i + 1] - 128)));
        }
    }

    // erosion : 0 = dilation, 1 = erosion
    void ero_dilat_nonBinary(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW, bool color, bool whiteObject, bool erosion)
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
                    int M;
                    if ((whiteObject && erosion) || (!whiteObject && !erosion))
                    {
                        M = 255;
                    }
                    else if ((whiteObject && !erosion) || (!whiteObject && erosion))
                    {
                        M = 0;
                    }
                    //boucle sur tout le voisinage de i-1 j -1 a i+1 j+1 en skippant les bords de l'image
                    for (int y = std::max(i - 1, 0); y <= std::min(i + 1, nH - 1); y++)
                    {
                        for (int x = std::max(j - 1, 0); x <= std::min(j + 1, nW - 1); x++)
                        {
                            if ((whiteObject && erosion) || (!whiteObject && !erosion))
                            {
                                M = std::min(M, (int)ImgIn[facteur * (y * nW + x) + k]);
                            }
                            else if ((whiteObject && !erosion) || (!whiteObject && erosion))
                            {
                                M = std::max(M, (int)ImgIn[facteur * (y * nW + x) + k]);
                            }
                        }
                    }
                    ImgOut[facteur * (i * nW + j) + k] = M;
                }
            }
        }
    }

    // color : 0 = greyScale, 1 = RGB
    // whiteObject : 0 = black object, 1 = white object
    void erosion_nonBinary(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW, bool color, bool whiteObject)
    {
        ero_dilat_nonBinary(ImgIn, ImgOut, nH, nW, color, whiteObject, true);
    }

    // color : 0 = greyScale, 1 = RGB
    // whiteObject : 0 = black object, 1 = white object
    void dilation_nonBinary(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW, bool color, bool whiteObject)
    {
        ero_dilat_nonBinary(ImgIn, ImgOut, nH, nW, color, whiteObject, false);
    }

    // color : 0 = greyScale, 1 = RGB
    // whiteObject : 0 = black object, 1 = white object
    void closing_nonBinary(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW, bool color, bool whiteObject)
    {
        OCTET *ImgTemp;
        allocation_tableau(ImgTemp, OCTET, nW * nH * (color ? 3 : 1));

        dilation_nonBinary(ImgIn, ImgTemp, nH, nW, color, whiteObject);
        erosion_nonBinary(ImgTemp, ImgOut, nH, nW, color, whiteObject);

        free(ImgTemp);
    }

    // color : 0 = greyScale, 1 = RGB
    // whiteObject : 0 = black object, 1 = white object
    void opening_nonBinary(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW, bool color, bool whiteObject)
    {
        OCTET *ImgTemp;
        allocation_tableau(ImgTemp, OCTET, nW * nH * (color ? 3 : 1));

        erosion_nonBinary(ImgIn, ImgTemp, nH, nW, color, whiteObject);
        dilation_nonBinary(ImgTemp, ImgOut, nH, nW, color, whiteObject);

        free(ImgTemp);
    }

    // color : 0 = greyScale, 1 = RGB
    // whiteObject : 0 = black object, 1 = white object
    void boundary_nonBinary(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW, bool color, bool whiteObject)
    {
        OCTET *ImgEro;
        OCTET *ImgDil;
        allocation_tableau(ImgEro, OCTET, nW * nH * (color ? 3 : 1));
        allocation_tableau(ImgDil, OCTET, nW * nH * (color ? 3 : 1));

        erosion_nonBinary(ImgIn, ImgEro, nH, nW, color, whiteObject);
        dilation_nonBinary(ImgIn, ImgDil, nH, nW, color, whiteObject);

        int S = 235;

        for (int i = 0; i < nW * nH * (color ? 3 : 1); ++i)
        {
            int val = 255 - (abs(ImgDil[i] - ImgEro[i])) / 2;
            ImgOut[i] = (val < S ? 0 : 255); // seuil
            //ImgOut[i] = val; // sinon juste la différence
        }

        free(ImgEro);
        free(ImgDil);
    }

    void writeHistoDatFile(OCTET *ImgIn, int arraySize, std::string path, bool isRGB)
    {
        std::ofstream file;

        file.open(path);
        if (!file.is_open())
        {
            std::cout << "Writer - Open File Error: " << path << std::endl;
            return;
        }

        int max = 255;

        std::vector<int> t_grey, t_r, t_g, t_b;

        if (isRGB)
        {
            t_r = std::vector<int>(max);
            t_g = std::vector<int>(max);
            t_b = std::vector<int>(max);

            for (int i = 0; i < arraySize; i = i + 3)
            {
                t_r[ImgIn[i]]++;
                t_g[ImgIn[i + 1]]++;
                t_b[ImgIn[i + 2]]++;
            }

            for (int i = 0; i < max; i++)
            {
                file << std::to_string(i + 1) + " " + std::to_string(t_r[i]) + " " + std::to_string(t_g[i]) + " " + std::to_string(t_b[i]) + "\n";
            }
        }
        else
        {
            t_grey = std::vector<int>(max);

            for (int i = 0; i < arraySize; i++)
            {
                t_grey[ImgIn[i]]++;
            }

            for (int i = 0; i < max; i++)
            {
                file << std::to_string(i + 1) + " " + std::to_string(t_grey[i]) + "\n";
            }
        }

        file.close();
    }

    void writeDistibutionDatFile(OCTET *ImgIn, int arraySize, std::string path, bool isRGB)
    {
        std::ofstream file;

        file.open(path);
        if (!file.is_open())
        {
            std::cout << "Writer - Open File Error: " << path << std::endl;
            return;
        }

        int max = 255;

        std::vector<float> t_grey, t_r, t_g, t_b;

        if (isRGB)
        {
            t_r = std::vector<float>(max);
            t_g = std::vector<float>(max);
            t_b = std::vector<float>(max);

            for (int i = 0; i < arraySize; i = i + 3)
            {
                t_r[ImgIn[i]]++;
                t_g[ImgIn[i + 1]]++;
                t_b[ImgIn[i + 2]]++;
            }

            for (int i = 0; i < max; i++)
            {
                file << std::to_string(i + 1) + " " + std::to_string(t_r[i] / arraySize) + " " + std::to_string(t_g[i] / arraySize) + " " + std::to_string(t_b[i] / arraySize) + "\n";
            }
        }
        else
        {
            t_grey = std::vector<float>(max);

            for (int i = 0; i < arraySize; i++)
            {
                t_grey[ImgIn[i]]++;
            }

            for (int i = 0; i < max; i++)
            {
                file << std::to_string(i + 1) + " " + std::to_string(t_grey[i] / arraySize) + "\n";
            }
        }

        file.close();
    }

    // Ecrit la carte de différence d'une image dans un tableau méthode A+B/2
    // Je n'ai pas fais la reconstruction car je n'ai pas su comment faire
    void diffMapAB2(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW)
    {
        std::vector<int> diff(nH * nW);

        for (int x = 1; x < nW; x++)
        {
            for (int y = 1; y < nH; y++)
            {
                diff[y * nW + x] = (ImgIn[y * nW + x - 1] + ImgIn[(y - 1) * nW + x]) / 2;
            }
        }

        for (int i = 0; i < nW * nH; i++)
        {
            ImgOut[i] = diff[i] - ImgIn[i] + 128;
        }
    }

    // Ecrit la carte de différence d'une image dans un tableau méthode MED PREDICTOR
    // Pas de reconstruction
    void diffMapMED(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW)
    {
        std::vector<int> diff(nH * nW);

        for (int x = 1; x < nW; x++)
        {
            for (int y = 1; y < nH; y++)
            {
                int A = ImgIn[y * nW + x - 1];
                int B = ImgIn[(y - 1) * nW + x];
                int C = ImgIn[(y - 1) * nW + x - 1];
                int X = 0;

                if (C >= std::max(A, B))
                {
                    X = std::min(A, B);
                }
                else if (C <= std::min(A, B))
                {
                    X = std::max(A, B);
                }
                else
                {
                    X = A + B - C;
                }

                diff[y * nW + x] = X;
            }
        }

        for (int i = 0; i < nW * nH; i++)
        {
            ImgOut[i] = diff[i] - ImgIn[i] + 128;
        }
    }

    void ppmtopgm(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW)
    {
        for (int i = 0; i < nH * nW * 3; i += 3)
        {
            int nr = ImgIn[i];
            int ng = ImgIn[i + 1];
            int nb = ImgIn[i + 2];

            int grey = nr * 0.3 + ng * 0.6 + nb * 0.1;

            ImgOut[i / 3] = grey;
        }
    }

    void seuillage(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW, int S)
    {
        for (int i = 0; i < nH; i++)
            for (int j = 0; j < nW; j++)
            {
                if (ImgIn[i * nW + j] < S)
                    ImgOut[i * nW + j] = 0;
                else
                    ImgOut[i * nW + j] = 255;
            }
    }

void flou(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW, bool color)
    {
        //Check si on est sur tu greyscale ou pas
        int facteur = color ? 3 : 1;
        int boucle = color ? 3 : 1;
        //Boucle sur chaque pixel de l'image


        for(int i = 0; i < nW * boucle; i++)
        {
            for(int k = 0; k < boucle; k++)
            {
                ImgOut[i + k] = ImgIn[i+k];
                ImgOut[i + nW * ((nH-1)*boucle) + k] = ImgIn[i + nW * ((nH-1)*boucle) + k];
            }
        }
        for(int i = 0; i < nH * boucle; i++)
        {
            for(int k = 0; k < boucle; k++)
            {
                ImgOut[nW * i + k] = ImgIn[nW * i + k];
                ImgOut[((nW - 1)*boucle) + nW * i + k] = ImgIn[((nW - 1)*boucle) + nW * i + k];
            }
        }

        for (int i = 1; i < nH-1; i++)
        {
            for (int j = 1; j < nW-1; j++)
            {
                for (int k = 0; k < boucle; k++)
                {
                    float sum = 0;
                    //boucle sur tout le voisinage de i-1 j -1 a i+1 j+1 en skippant les bords de l'image
                    for (int y = i - 1; y <= i + 1; y++)
                    {
                        for (int x = j - 1; x <= j + 1; x++)
                        {
                            int pixelIndex = facteur * (y * nW + x) + k;
                            sum += ImgIn[pixelIndex];
                        }
                    }
                    ImgOut[facteur * (i * nW + j) + k] = static_cast<OCTET>(sum / 9);
                }
            }
        }
    }

    void flou_background(OCTET *ImgIn, OCTET *Imgnb, OCTET *ImgOut, int nH, int nW, bool color)
    {
        //Charger la version flou de l'image
        OCTET *ImgFlou;
        allocation_tableau(ImgFlou, OCTET, nW * nH * (color ? 3 : 1));
        flou(ImgIn, ImgFlou, nH, nW, color);
        //Check si on est sur tu greyscale ou pas
        int facteur = color ? 3 : 1;
        int boucle = color ? 3 : 1;
        //Boucle sur chaque pixel de l'image
        for (int i = 0; i < nH; i++)
        {
            for (int j = 0; j < nW; j++)
            {
                int pixelid = (i * nW + j);
                //Si le pixel appartient au fond
                if (Imgnb[pixelid] == 255)
                {
                    for (int k = 0; k < boucle; k++)
                    {
                        ImgOut[facteur * pixelid + k] = ImgFlou[facteur * pixelid + k];
                    }
                }
                else
                {
                    for (int k = 0; k < boucle; k++)
                    {
                        ImgOut[facteur * pixelid + k] = ImgIn[facteur * pixelid + k];
                    }
                }
            }
        }
    }

    void findMinFPRate(int *VP, int *VN, int *FP, int *FN, int size, double &minFPRate, int &minIndex)
    {
        double FPRate[size];
        for (int i = 0; i < size; i++)
        {
            if (FP[i] + VN[i] == 0)
            {
                FPRate[i] = 0;
            }
            else
            {
                FPRate[i] = (double)FP[i] / (FP[i] + VN[i]);
            }
        }

        minFPRate = 2; // initialisation à une valeur supérieure à 1
        minIndex = -1; // initialisation à une valeur invalide
        for (int i = 0; i < size; i++)
        {
            if (FPRate[i] < minFPRate)
            {
                minFPRate = FPRate[i];
                minIndex = i;
            }
        }
    }

    void writeROCDatFile(OCTET *ImgIn, int arraySize, std::string path, OCTET *Imgtruth, int nH, int nW)
    {
        std::ofstream file;

        file.open(path);
        if (!file.is_open())
        {
            std::cout << "Writer - Open File Error: " << path << std::endl;
            return;
        }

        int max = 256;

        int *VP, *FP, *VN, *FN;
        allocation_tableau(VP, int, max);
        allocation_tableau(VN, int, max);
        allocation_tableau(FP, int, max);
        allocation_tableau(FN, int, max);

        OCTET *ImgSeuilled;
        allocation_tableau(ImgSeuilled, OCTET, nH * nW);
        //Parcours des 256 niveau de gris pour tester les seuils
        for (int k = 0; k < max; k++)
        {
            seuillage(ImgIn, ImgSeuilled, nH, nW, k);

            for (int i = 0; i < nH; i++)
            {
                for (int j = 0; j < nW; j++)
                {
                    int pix = i * nW + j;

                    if (Imgtruth[pix] == 255)
                    {
                        if (ImgSeuilled[pix] == 255)
                        {
                            // true positive
                            VP[k]++;
                        }
                        else
                        {
                            // false negative
                            FN[k]++;
                        }
                    }
                    else
                    {
                        if (ImgSeuilled[pix] == 255)
                        {
                            // false positive
                            FP[k]++;
                        }
                        else
                        {
                            // true negative
                            VN[k]++;
                        }
                    }
                }
            }
        }

        int min_i;
        double FPR;
        // C'est ça qui merde//
        findMinFPRate(VP, VN, FP, FN, max, FPR, min_i);
        //Ecriture
        std::cout << "i minimun : " << min_i << std::endl;
        // niveau de gris - Vrai pos - Faux pos - Vrai neg - Faux neg - Somme des pos/neg - Sensibilité - Specificité - (1-Specificite)
        for (int i = 0; i < max; i++)
        {
            int sum = FN[i] + FP[i] + VP[i] + VN[i];
            float sensi = VP[i] / (float)(VP[i] + FN[i]);
            float spec = VN[i] / (float)(VN[i] + FP[i]);
            file << std::to_string(i) + " " + std::to_string(VP[i]) + " " + std::to_string(FP[i]) + " " + std::to_string(VN[i]) + " " + std::to_string(FN[i]) + " " + std::to_string(sum) + " " + std::to_string(sensi) + " " + std::to_string(spec) + " " + std::to_string(1 - spec) + "\n";
        }

        file.close();
    }

    void gradient(OCTET* ImgIn, OCTET* ImgOut, int nH, int nW) {
    int gH, gV;
    for(int i=0; i<nH; i++) {
        for(int j=0; j<nW; j++) {
            gH = 0;
            gV = 0;
            if(i > 0) {
                gV -= ImgIn[(i-1)*nW + j];
            } else {
                gV -= ImgIn[i*nW + j];
            }
            if(i < nH-1) {
                gV += ImgIn[(i+1)*nW + j];
            } else {
                gV += ImgIn[i*nW + j];
            }
            if(j > 0) {
                gH -= ImgIn[i*nW + j-1];
            } else {
                gH -= ImgIn[i*nW + j];
            }
            if(j < nW-1) {
                gH += ImgIn[i*nW + j+1];
            } else {
                gH += ImgIn[i*nW + j];
            }
            ImgOut[i*nW+j] = sqrt(gV*gV + gH*gH);
        }
    }
}

    void writeProfilDatFile(OCTET *ImgIn, int nH, int nW, int indice, std::string path, bool isColumn)
    {
        std::ofstream file;

        file.open(path);
        if (!file.is_open())
        {
            std::cout << "Writer - Open File Error: " << path << std::endl;
            return;
        }

            std::vector<int> Profil;
            //Profil d'une colonne
            if (isColumn)
            {
                Profil = std::vector<int>(nW);
                for (int y = 0; y < nH; y++)
                {
                Profil[y] = ImgIn[indice + y * nW];
                }
            }
            else{
            //Profil d'une ligne
                Profil = std::vector<int>(nH);
                for (int x = 0; x < nH; x++)
                {
                Profil[x] = ImgIn[indice * nW + x];
                }
            }



            for (int i = 0; i < Profil.size(); i++)
            {
                file << std::to_string(i) + " " + std::to_string(Profil[i]) + "\n";
            }
        

        file.close();
    }


    void reverse(OCTET* ImgIn, OCTET* ImgOut, int size)
    {
        for(int i=0;i<size;i++)
        {
            ImgOut[i] = ( ImgIn[i] == 255? 0: 255);
        }
    }

void hysteresis(OCTET* ImgIn, OCTET* ImgOut, int nH, int nW, int seuilBas, int seuilHaut) {
    OCTET* ImgInter;
    allocation_tableau(ImgInter, OCTET, nH*nW);

    for(int i = 0; i < nH; i++) {
        for(int j = 0; j < nW; j++) {
            int val = ImgIn[i*nW+j];
            int ret = 0;
            if(val >= seuilHaut) {
                ret = 255;
            }
            ImgInter[i*nW+j] = ret;
        }
    }

    for(int i = 0; i < nH; i++) {
        for(int j = 0; j < nW; j++) {
            int valInter = ImgInter[i*nW+j];
            int valIn = ImgIn[i*nW+j];
            int ret = valInter;
            if(valInter == 0 && valIn >= seuilBas) {
                for(int a = -1; a <= 1; a++) {
                    for(int b = -1; b <= 1; b++) {
                        if(i+a >= 0 && i+a < nH && j+b >= 0 && j+b < nW) {
                            if(ImgInter[(i+a)*nW+j+b] == 255) {
                                ret = 255;
                                break; // on arrête dès qu'on trouve un pixel blanc
                            }
                        }
                    }
                    if(ret == 255) {
                        break; // on arrête la boucle si on a déjà trouvé un pixel blanc
                    }
                }
            }
            ImgOut[i*nW+j] = ret;
        }
    }

    free(ImgInter);
}
//Ecrit dans ImgOut laplacien + 128 pour avoir un visuel et stocke dans ImgLaplacien le laplacien standard (avec les valeurs positive et negative)
void Laplacien(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW, bool color)
{
    // Vérifie si l'image est en couleur ou en niveaux de gris
    int facteur = color ? 3 : 1;
    int boucle = color ? 3 : 1;

    // Copie des bords de l'image
    for(int i = 0; i < nW * boucle; i++)
    {
        for(int k = 0; k < boucle; k++)
        {
            ImgOut[i + k] = ImgIn[i+k];
            ImgOut[i + nW * ((nH-1)*boucle) + k] = ImgIn[i + nW * ((nH-1)*boucle) + k];
        }
    }
    for(int i = 0; i < nH * boucle; i++)
    {
        for(int k = 0; k < boucle; k++)
        {
            ImgOut[nW * i + k] = ImgIn[nW * i + k];
            ImgOut[((nW - 1)*boucle) + nW * i + k] = ImgIn[((nW - 1)*boucle) + nW * i + k];
        }
    }

    // Application de l'opérateur laplacien sur chaque pixel de l'image
    for (int i = 1; i < nH-1; i++)
    {
        for (int j = 1; j < nW-1; j++)
        {
            for (int k = 0; k < boucle; k++)
            {
                float sum = 0;
                int pix = facteur * (i * nW + j) + k; // pixel courant

                // Application de l'opérateur laplacien
                sum += 4 * ImgIn[pix]; // pixel central
                sum -= ImgIn[pix-facteur]; // gauche
                sum -= ImgIn[pix+facteur]; // droite
                sum -= ImgIn[pix-facteur*nW]; // haut
                sum -= ImgIn[pix+facteur*nW]; // bas

                // Évite les valeurs négatives et supérieures à 255
                if(sum < 0) sum = 0;
                if(sum > 255) sum = 255;

                ImgOut[pix] = static_cast<OCTET>(sum);
            }
        }
    }
}

int getNeighbor(float angle, int i, int j, int nW, int nH)
{
    if(-M_PI/6 <= angle        && angle < M_PI/6)    return i*nW+(j+1);
    else if( M_PI/6 <= angle   && angle < M_PI/3)    return (i+1)*nW+(j+1);
    else if( M_PI/3 <= angle   && angle < 2*M_PI/3)  return (i+1)*nW+j;
    else if(2*M_PI/3 <= angle  && angle < 5*M_PI/6)  return (i+1)*nW+(j-1);
    else if(-M_PI/3 <= angle   && angle < -M_PI/6)   return (i-1)*nW+(j+1);
    else if(-2*M_PI/3 <= angle && angle < -M_PI/3)   return (i-1)*nW+j;
    else if(-5*M_PI/6 <= angle && angle < -2*M_PI/3) return (i-1)*nW+(j-1);
    else                                             return i*nW+(j-1);
}
void passage_zero(OCTET *ImgLaplacien128, OCTET *ImgLaplacien, OCTET *ImgOut, int nH, int nW)
{
    std::vector<int> Laplacien(nH*nW);
    for (int i = 0; i < nH*nW; i++)
    {   
        Laplacien[i] = ImgLaplacien128[i]-128;
        ImgLaplacien[i] = Laplacien[i];
    }

    for (int i = 1; i < nH-1; i++)
    {
        for (int j = 1; j < nW-1; j++)
        {
            int pix = i * nW + j;
            int current_pix = Laplacien[pix];
            //Ai Aj G et D voir enoncé tp
            int Ai = Laplacien[(i+1)*nW + j] - Laplacien[i*nW+j];
            int Aj = Laplacien[i*nW+(j+1)] - Laplacien[i*nW+j];
            int G = std::max(abs(Ai),abs(Aj));
            //Si Ai = 0 on skip et on mets le pixel a 0
            if (Ai == 0)
            {
                ImgOut[pix] = 0;
            }
            else
            {
                float D = atan(abs(Aj)/abs(Ai));
                int voisin = getNeighbor(D,i,j,nW,nH);
                ImgOut[pix] = (voisin * current_pix < 0 ? G : 0);
            }
        }
    }
}

}

#endif //CODAGE_IMAGEALGORITHMS_H
