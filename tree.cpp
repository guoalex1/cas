#include "tree.h"

#include <iostream>

NodeBase::NodeBase(std::unique_ptr<NodeBase> left, std::unique_ptr<NodeBase> right) 
    : left(std::move(left)), right(std::move(right)) {
}

NodeVal::NodeVal(int val) 
    : NodeBase(nullptr, nullptr), val(val) {
}

NodeAdd::NodeAdd(std::unique_ptr<NodeBase> left, std::unique_ptr<NodeBase> right)
    : NodeBase(std::move(left), std::move(right)) {
}

NodeMultiply::NodeMultiply(std::unique_ptr<NodeBase> left, std::unique_ptr<NodeBase> right)
    : NodeBase(std::move(left), std::move(right)) {
}

int NodeVal::evaluate() const {
    return val;
}

int NodeAdd::evaluate() const {
    return left->evaluate() + right->evaluate();
}

int NodeMultiply::evaluate() const {
    return left->evaluate() * right->evaluate();
}