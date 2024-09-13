#include <iostream>
#include "tree.h"

int main() {
    std::unique_ptr<NodeVal> leftNode = std::make_unique<NodeVal>(5);
    std::unique_ptr<NodeVal> rightNode = std::make_unique<NodeVal>(3);
    std::unique_ptr<NodeAdd> addNode = std::make_unique<NodeAdd>(std::move(leftNode), std::move(rightNode));

    std::unique_ptr<NodeVal> valNode = std::make_unique<NodeVal>(-10);

    std::unique_ptr<NodeMultiply> multiplyNode = std::make_unique<NodeMultiply>(std::move(addNode), std::move(valNode));

    std::unique_ptr<NodeAddInverse> addInverseNode = std::make_unique<NodeAddInverse>(std::move(multiplyNode));

    std::cout << addInverseNode->evaluate() << std::endl;
}
