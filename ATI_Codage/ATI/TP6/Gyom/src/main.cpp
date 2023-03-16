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
    if (argc != 4)
    {
        printf("Usage: ImageIn.ppm Seuil_Variance_Split Seuil_Moyenne_Merge\n");
        exit(1);
    }

    std::string inputName = argv[1];
    inputName = "../in/" + inputName;
    int nH, nW, nTaille, S, S_;
    S = atoi(argv[2]);
    S_ = atoi(argv[3]);

    OCTET *ImgIn, *ImgOut, *ImgInter;

    bool color = !(inputName.substr(inputName.size() - 3, 3)).compare("ppm");
    (color ? lire_nb_lignes_colonnes_image_ppm(inputName, &nH, &nW) : lire_nb_lignes_colonnes_image_pgm(inputName, &nH, &nW));
    std::cout << "Image " << (color ? "couleur" : "niveau de gris") << " en input de taille : " << nH << "x" << nW << std::endl;

    nTaille = nH * nW;
    int nTaille3 = nTaille * 3;
    // Allocation et lecture
    allocation_tableau(ImgIn, OCTET, nTaille);
    allocation_tableau(ImgOut, OCTET, nTaille);
    allocation_tableau(ImgInter, OCTET, nTaille);

    lire_image_pgm(inputName, ImgIn, nH * nW);

    // 1-InitialSplit
    ImageAlgorithms::splitImg(ImgIn,ImgOut,nH,nW,S);
    ecrire_image_pgm("../out/ImgSplitMoy.pgm", ImgOut, nH, nW);


    // 2-RecurseSplit (with a QuadTree)
    int cpt = 0; //compteur d'id
    ImageAlgorithms::Node *Tree = ImageAlgorithms::buildQuadtreeGray(ImgIn,ImgOut,0,0,nW,S,nW,cpt);
    ecrire_image_pgm("../out/ImgRecurseandBuildQuadTree.pgm", ImgOut, nH, nW);

    //3-MergeWith QuadTree
    std::vector<ImageAlgorithms::Node*> leaves;
    collect_leaves(Tree, leaves); // Recolter les feuilles de l'arbre dans leaves
    std::vector<std::pair<ImageAlgorithms::Node*, ImageAlgorithms::Node*>> edges;
    build_RAG(leaves, edges,nW); //Stocker dans edges tout les voisinages inter-bloc
    //print_RAG(leaves, edges); //Afficher leaves et edges
    ImageAlgorithms::blend_RAG(leaves,edges,ImgOut,nW,nH,S_); // Merge en fonction de Seuil moyenne
    ecrire_image_pgm("../out/ImgMerged.pgm", ImgOut, nH, nW);

    //4-RGB version

    return 1;
}
