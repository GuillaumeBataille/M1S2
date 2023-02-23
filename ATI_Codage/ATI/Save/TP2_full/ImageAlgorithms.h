//
// Created by bat-portable on 21/01/2023.
//

#ifndef CODAGE_IMAGEALGORITHMS_H
#define CODAGE_IMAGEALGORITHMS_H

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

    // Trouve les deux couleurs les plus distantes possible
    void findDistantColor(Color &c1, Color &c2, OCTET *ImgIn, int nTaille3, int precision)
    {
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

    //Trouve deux couleurs aléatoire
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

    //Application du Kmean sur n-classes
    void k_mean(OCTET *ImgIn, std::vector<Color> &classes, int nTaille3, int repetition)
    {
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

    //Calcul du psnr
    float psnr(OCTET *ImgIn, OCTET *ImgOut, int maxSignal, int nTaille3)
    {
        float eqm; // erreur quadratique
        for (int i = 0; i < nTaille3; i++)
        {
            eqm += pow(ImgIn[i] - ImgOut[i], 2);
        }
        eqm /= nTaille3;

        float a = 10 * log10(static_cast<float>(pow(maxSignal, 2)) / eqm);
        return a;
    }

    std::vector<Color> colorPalette_rgb(OCTET *Img, int nTaille3)
    {
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

    void createColorPalette_rgb(OCTET *&Palette, OCTET *Img, int nTaille3, int &nW, int &nH)
    {
        std::vector<Color> palette = colorPalette_rgb(Img, nTaille3);

        int tailleImg_x = ceil(sqrt(palette.size()));
        int tailleImg = tailleImg_x * tailleImg_x;
        int tailleImg3 = tailleImg * 3;
        nW = tailleImg_x;
        nH = tailleImg_x;

        allocation_tableau(Palette, OCTET, tailleImg3);
        //std::cout << "plalette size : " << palette.size() << std::endl;
        int index = 0;
        for (int i = 0; i < palette.size(); i++)
        {
            Palette[index] = palette[i].r;
            Palette[index + 1] = palette[i].g;
            Palette[index + 2] = palette[i].b;

            index = index + 3;
        }
    }
}

#endif //CODAGE_IMAGEALGORITHMS_H
