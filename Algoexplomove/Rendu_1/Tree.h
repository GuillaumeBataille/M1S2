#ifndef Tree_H
#define Tree_H

#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <cassert>
#include <queue>

// ------------------------ classe noeud ---------------------

class Node
{
private:
    Node *parent = nullptr;     // ptr vers parent
    std::vector<Node *> childs; //liste de noeud enfant

public:
    int id;
    int size = 0;

    Node(){};
    Node(Node *parentPtr, int id)
    {
        this->id = id;
        parent = parentPtr;
    }
    Node(int id)
    {
        this->id = id;
    }
    ~Node(){};

    Node *createChild(int id) //Creation d'enfant
    {
        Node *nodePtr = new Node(this, id);
        childs.push_back(nodePtr);
        size++;
        return nodePtr;
    }

    bool hasChild() // Teste si y'a un enfant
    {
        if (childs.size() == 0)
        {
            return false;
        }
        return true;
    }

    int getSize() // Retour du nbr d'enfant
    {
        return childs.size();
    }

    std::vector<Node *> *getChilds()
    {
        return &childs;
    }
};

// ---------------------- classe Tree --------------------

class Tree
{
private:
    Node *root; //Noeud initial

public:
    Tree(){};
    ~Tree(){};

    void createFromFile(std::string path)
    {
        // Ouverture du fichier en lecture
        std::ifstream file;
        file.open(path);

        // Vérification de l'ouverture du fichier
        if (!file.is_open())
        {
            std::cout << "Erreur lors de la lecture du fichier" << std::endl;
            return;
        }

        // Map pour stocker les noeuds créés en utilisant leur ID comme clé
        std::map<int, Node *> created;
        std::string line;

        // Lecture de chaque ligne du fichier
        while (std::getline(file, line))
        {
            // Parsing de la ligne pour obtenir les ID des parents et des enfants
            std::stringstream ss(line);
            std::vector<int> parsedLine;

            std::string word;
            while (std::getline(ss, word, ' '))
            {
                parsedLine.push_back(std::stoi(word));
            }

            // Récupération de l'ID du parent
            int idParent = parsedLine[0];
            assert(!parsedLine.empty());
            parsedLine.erase(parsedLine.begin()); // suppression de la première valeur

            // Récupération du noeud parent s'il a déjà été créé, sinon création d'un nouveau noeud
            Node *actualNode;
            if (created.count(idParent))
            {
                actualNode = created.at(idParent); // récupération du pointeur vers le noeud parent
            }
            else
            {
                actualNode = new Node(idParent);
                created.insert(std::pair<int, Node *>(idParent, actualNode)); // insertion dans la map
                root = actualNode;
            }

            // Parcours des enfants et création de chaque noeud enfant
            for (int idChild : parsedLine)
            {
                Node *childPtr = actualNode->createChild(idChild);
                created.insert(std::pair<int, Node *>(idChild, childPtr));
            }
        }

        // Fermeture du fichier
        file.close();
    }

    // -----------------------------parcours en largeur ----------------------------
    void breadthFirstSearch()
    {
        // Initialisation d'une file d'attente pour stocker les noeuds à visiter
        std::queue<Node *> idStack;
        // Vérification que le noeud racine existe
        if (root != nullptr)
        {
            // Ajout du noeud racine à la file d'attente
            idStack.push(root);
        }

        // Boucle while qui continue tant que la file d'attente n'est pas vide
        while (!idStack.empty())
        {
            // Récupération du premier élément (le prochain noeud à visiter) de la file d'attente
            Node *actualNode = idStack.front();
            // Suppression de ce noeud de la file d'attente
            idStack.pop();

            // Affichage de l'ID du noeud courant
            std::cout << actualNode->id << " |--> ";

            // Parcours des enfants du noeud courant
            for (Node *child : *actualNode->getChilds())
            {
                // Ajout des enfants à la file d'attente pour qu'ils soient visités plus tard
                idStack.push(child);
            }
        }

        // Affichage de "End." pour indiquer la fin de la recherche
        std::cout << "End." << std::endl;
    }

    // -----------------------------parcours en profondeur ----------------------------
    void depthFirstSearch()
    {
        // Initialisation d'une pile pour stocker les noeuds à visiter
        std::deque<Node *> idStack;
        // Vérification que le noeud racine existe
        if (root != nullptr)
        {
            // Ajout du noeud racine à la pile
            idStack.push_front(root);
        }

        // Boucle while qui continue tant que la pile n'est pas vide
        while (!idStack.empty())
        {
            // Récupération du premier élément (le prochain noeud à visiter) de la pile
            Node *actualNode = idStack.front();
            // Suppression de ce noeud de la pile
            idStack.pop_front();

            // Affichage de l'ID du noeud courant
            std::cout << actualNode->id << " |--> ";

            // Récupération des enfants du noeud courant
            std::vector<Node *> *childs = actualNode->getChilds();

            // Parcours des enfants en utilisant un itérateur

            for (auto it = childs->rbegin(); it != childs->rend(); it++)
            {
                idStack.push_front(*it);
            }
        }

        std::cout << "End." << std::endl;
    }
};

#endif
