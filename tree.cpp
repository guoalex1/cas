#include "tree.h"

#include <iostream>
#include <cmath>



UnaryNodeBase::UnaryNodeBase(std::unique_ptr<NodeBase> arg)
    : arg(std::move(arg)) {
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

NodeVar::NodeVar(char symbol)
    : symbol(symbol) {
}

int NodeVar::evaluate() const {
    return 0;
}

NodeAddInverse::NodeAddInverse(std::unique_ptr<NodeBase> arg)
    : UnaryNodeBase(std::move(arg)) {
}

int NodeAddInverse::evaluate() const {
    return -1 * arg->evaluate();
}

NodeSin::NodeSin(std::unique_ptr<NodeBase> arg)
    : UnaryNodeBase(std::move(arg)) {
}

int NodeSin::evaluate() const {
    return sin(arg->evaluate() * M_PI / 180); // convert to radians
}

NodeCos::NodeCos(std::unique_ptr<NodeBase> arg)
    : UnaryNodeBase(std::move(arg)) {
}

int NodeCos::evaluate() const {
    return cos(arg->evaluate() * M_PI / 180); // convert to radians
}

NodeExp::NodeExp(std::unique_ptr<NodeBase> arg)
    : UnaryNodeBase(std::move(arg)) {
}

int NodeExp::evaluate() const {
    return exp(arg->evaluate());
}

NodeLog::NodeLog(std::unique_ptr<NodeBase> arg)
    : UnaryNodeBase(std::move(arg)) {
}

int NodeLog::evaluate() const {
    return log(arg->evaluate());
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
