#include <iostream>
#include "tree.h"
#include "parse.h"
#include <string>

int main(int argc, char** argv) {
    std::string test = argv[1];

    std::unique_ptr<NodeBase> node = buildTree(test);

    std::cout << node->evaluate() << std::endl;
}
