#include "tree.h"

#include <iostream>
#include <cmath>

UnaryNodeBase::UnaryNodeBase(std::unique_ptr<NodeBase> next)
    : next(std::move(next)) {
}

BinaryNodeBase::BinaryNodeBase(std::unique_ptr<NodeBase> left, std::unique_ptr<NodeBase> right) 
    : left(std::move(left)), right(std::move(right)) {
}

NodeVal::NodeVal(int val) 
    : val(val) {
}

int NodeVal::evaluate() const {
    return val;
}

NodeAddInverse::NodeAddInverse(std::unique_ptr<NodeBase> next)
    : UnaryNodeBase(std::move(next)) {
}

int NodeAddInverse::evaluate() const {
    return -1 * next->evaluate();
}

NodeAdd::NodeAdd(std::unique_ptr<NodeBase> left, std::unique_ptr<NodeBase> right)
    : BinaryNodeBase(std::move(left), std::move(right)) {
}

int NodeAdd::evaluate() const {
    return left->evaluate() + right->evaluate();
}

NodeMultiply::NodeMultiply(std::unique_ptr<NodeBase> left, std::unique_ptr<NodeBase> right)
    : BinaryNodeBase(std::move(left), std::move(right)) {
}

int NodeMultiply::evaluate() const {
    return left->evaluate() * right->evaluate();
}


NodeExponent::NodeExponent(std::unique_ptr<NodeBase> left, std::unique_ptr<NodeBase> right)
    : BinaryNodeBase(std::move(left), std::move(right)) {
}

int NodeExponent::evaluate() const {
    return pow(left->evaluate(), right->evaluate());
}
