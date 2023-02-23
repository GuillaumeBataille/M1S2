#ifndef Tree_H
#define Tree_H

#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <cassert>
#include <queue>

class Node
{
    private:
        Node* parent = nullptr;
        std::vector<Node*> childs;
    
    public:
        int id;
        int size = 0;

        Node(){};
        Node(Node* parentPtr, int id){
            this->id = id;
            parent = parentPtr;
        }
        Node(int id){
            this->id = id;
        }
        ~Node(){};

        Node* createChild(int id)
        {
            Node* nodePtr = new Node(this, id);
            childs.push_back(nodePtr);
            size++;
            return nodePtr;
        }

        bool hasChild()
        {
            if(childs.size() == 0)
            {
                return false;
            }
            return true;
        }

        int getSize()
        {
            return childs.size();
        }

        std::vector<Node*>* getChilds(){
            return &childs;
        }

};

class Tree
{
    private:
        Node* root;

    public:

        Tree(){};
        ~Tree(){};


        // file format :
        // root child_1, child_2, ...
        // child_x, child_3, child_4, ...
        void createFromFile(std::string path)
        {
            std::cout << "reading..." << std::endl;

            std::ifstream file;
            file.open(path);

            if(!file.is_open()){
                std::cout << "read file error" << std::endl;
            }

            std::map<int, Node*> created;
            std::string line;

            while(std::getline(file, line)) // while file has next line
            {
                // ---------- parsing ----------
                std::stringstream ss(line);
                std::vector<int> parsedLine;

                std::string word;
                while(std::getline(ss, word, ' '))
                {
                    parsedLine.push_back(std::stoi(word));
                }

                int idParent = parsedLine[0];
                assert(!parsedLine.empty());
                parsedLine.erase(parsedLine.begin()); // pop front
                // -----------------------------

                Node* actualNode;
                if(created.count(idParent)) // check if parent has already been created, if no => root, or bad file format
                {
                   actualNode = created.at(idParent);
                } else
                {
                    actualNode = new Node(idParent);
                    created.insert(std::pair<int, Node*>(idParent, actualNode));
                    root = actualNode;
                }

                for(int idChild : parsedLine) // create child
                {
                    Node* childPtr = actualNode->createChild(idChild);
                    created.insert(std::pair<int, Node*>(idChild, childPtr));
                }

            }

            file.close();
        }

        void breadthFirstSearch(){
            std::queue<Node*> idStack;
            if(root != nullptr){
                idStack.push(root);
            }

            while(!idStack.empty()){
                Node* actualNode = idStack.front();
                idStack.pop();

                std::cout << actualNode->id << " -> ";

                for(Node* child : *actualNode->getChilds()){
                    idStack.push(child);
                }
            }

            std::cout << "End." << std::endl;
        }

        void depthFirstSearch(){
            std::deque<Node*> idStack;
            if(root != nullptr) {
                idStack.push_front(root);
            }

            while(!idStack.empty()){
                Node* actualNode = idStack.front();
                idStack.pop_front();

                std::cout << actualNode->id << " -> ";

                std::vector<Node*>* childs = actualNode->getChilds();

                for(auto it = childs->rbegin() ; it != childs->rend(); it++){
                    idStack.push_front(*it);
                }
            }

            std::cout << "End." << std::endl;
        }
};

#endif