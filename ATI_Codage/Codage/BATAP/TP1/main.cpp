
#include "Tree.h"

int main()
{

    Tree tree = Tree();
    tree.createFromFile("./tree1");
    std::cout << "Breadth-First-Search :" << std::endl;
    tree.breadthFirstSearch();
    std::cout << "Depth-First-Search :" << std::endl;
    tree.depthFirstSearch();
}