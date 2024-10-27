#include "tree.h"

#include <iostream>
#include <cmath>

using std::string;
using std::unique_ptr;

NodeBase::NodeBase(int precedence)
    : precedence(precedence) {
}

UnaryNodeBase::UnaryNodeBase(unique_ptr<NodeBase> arg, int precedence)
    : NodeBase(precedence), arg(std::move(arg)) {
}

BinaryNodeBase::BinaryNodeBase(unique_ptr<NodeBase> left, unique_ptr<NodeBase> right, int precedence)
    : NodeBase(precedence), left(std::move(left)), right(std::move(right)) {
}

NodeVal::NodeVal(int val, int precedence) 
    : NodeBase(precedence), val(val) {
}

int NodeVal::evaluate() const {
    return val;
}

string NodeVal::toString() const {
    return std::to_string(val);
}

NodeVar::NodeVar(char symbol, int precedence)
    : NodeBase(precedence), symbol(symbol) {
}

int NodeVar::evaluate() const {
    return 0;
}

string NodeVar::toString() const {
    return string(1, symbol);
}

NodeAddInverse::NodeAddInverse(unique_ptr<NodeBase> arg, int precedence)
    : UnaryNodeBase(std::move(arg), precedence) {
}

int NodeAddInverse::evaluate() const {
    return -1 * arg->evaluate();
}

string NodeAddInverse::toString() const {
    if (precedence > arg->precedence) {
        return "-(" + arg->toString() + ")";
    } else {
        return "-" + arg->toString();
    }
}

NodeSin::NodeSin(unique_ptr<NodeBase> arg, int precedence)
    : UnaryNodeBase(std::move(arg), precedence) {
}

int NodeSin::evaluate() const {
    return sin(arg->evaluate() * M_PI / 180); // convert to radians
}

string NodeSin::toString() const {
    return "sin(" + arg->toString() + ")";
}

NodeCos::NodeCos(unique_ptr<NodeBase> arg, int precedence)
    : UnaryNodeBase(std::move(arg), precedence) {
}

int NodeCos::evaluate() const {
    return cos(arg->evaluate() * M_PI / 180); // convert to radians
}

string NodeCos::toString() const {
    return "cos(" + arg->toString() + ")";
}

NodeExp::NodeExp(unique_ptr<NodeBase> arg, int precedence)
    : UnaryNodeBase(std::move(arg), precedence) {
}

int NodeExp::evaluate() const {
    return exp(arg->evaluate());
}

string NodeExp::toString() const {
    return "exp(" + arg->toString() + ")";
}

NodeLog::NodeLog(unique_ptr<NodeBase> arg, int precedence)
    : UnaryNodeBase(std::move(arg), precedence) {
}

int NodeLog::evaluate() const {
    return log(arg->evaluate());
}

string NodeLog::toString() const {
    return "log(" + arg->toString() + ")";
}

NodeAdd::NodeAdd(unique_ptr<NodeBase> left, unique_ptr<NodeBase> right, int precedence)
    : BinaryNodeBase(std::move(left), std::move(right), precedence) {
}

int NodeAdd::evaluate() const {
    return left->evaluate() + right->evaluate();
}

string NodeAdd::toString() const {
    string leftString;
    string rightString;

    if (left->precedence < precedence) {
        leftString = "(" + left->toString() + ")";
    } else {
        leftString = left->toString();
    }

    if (right->precedence < precedence) {
        rightString = "(" + right->toString() + ")";
    } else {
        rightString = right->toString();
    }

    return leftString + "+" + rightString;
}

NodeSubtract::NodeSubtract(unique_ptr<NodeBase> left, unique_ptr<NodeBase> right, int precedence)
    : BinaryNodeBase(std::move(left), std::move(right), precedence) {
}

int NodeSubtract::evaluate() const {
    return left->evaluate() - right->evaluate();
}

string NodeSubtract::toString() const {
    string leftString;
    string rightString;

    if (left->precedence < precedence) {
        leftString = "(" + left->toString() + ")";
    } else {
        leftString = left->toString();
    }

    if (right->precedence < precedence) {
        rightString = "(" + right->toString() + ")";
    } else {
        rightString = right->toString();
    }

    return leftString + "-" + rightString;
}

NodeMultiply::NodeMultiply(unique_ptr<NodeBase> left, unique_ptr<NodeBase> right, int precedence)
    : BinaryNodeBase(std::move(left), std::move(right), precedence) {
}

int NodeMultiply::evaluate() const {
    return left->evaluate() * right->evaluate();
}

string NodeMultiply::toString() const {
    string leftString;
    string rightString;

    if (left->precedence < precedence) {
        leftString = "(" + left->toString() + ")";
    } else {
        leftString = left->toString();
    }

    if (right->precedence < precedence) {
        rightString = "(" + right->toString() + ")";
    } else {
        rightString = right->toString();
    }

    return leftString + "*" + rightString;
}

NodeDivide::NodeDivide(unique_ptr<NodeBase> left, unique_ptr<NodeBase> right, int precedence)
    : BinaryNodeBase(std::move(left), std::move(right), precedence) {
}

int NodeDivide::evaluate() const {
    return left->evaluate() / right->evaluate();
}

string NodeDivide::toString() const {
    string leftString;
    string rightString;

    if (left->precedence < precedence) {
        leftString = "(" + left->toString() + ")";
    } else {
        leftString = left->toString();
    }

    if (right->precedence < precedence) {
        rightString = "(" + right->toString() + ")";
    } else {
        rightString = right->toString();
    }

    return leftString + "/" + rightString;
}

NodeExponent::NodeExponent(unique_ptr<NodeBase> left, unique_ptr<NodeBase> right, int precedence)
    : BinaryNodeBase(std::move(left), std::move(right), precedence) {
}

int NodeExponent::evaluate() const {
    return pow(left->evaluate(), right->evaluate());
}

string NodeExponent::toString() const {
    string leftString;
    string rightString;

    if (left->precedence < precedence) {
        leftString = "(" + left->toString() + ")";
    } else {
        leftString = left->toString();
    }

    if (right->precedence <= precedence) {
        rightString = "(" + right->toString() + ")";
    } else {
        rightString = right->toString();
    }

    return leftString + "^" + rightString;
}
