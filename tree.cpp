#include "tree.h"

#include <iostream> // debug
#include <cmath>

const int valPrecedence = 5; // NodeVal, NodeVar
const int unaryPrecedence = 4; // functions, NodeAddInverse
const int exponentPrecedence = 3; // NodeExponent
const int multiplyPrecendence = 2; // NodeMultiply, NodeDivide
const int addPrecedence = 1; // NodeAdd, NodeSubtract

using std::string;
using std::unique_ptr;
using std::make_unique;

NodeBase::NodeBase(int precedence) : precedence(precedence) {
}

int NodeBase::getPrecedence() const {
    return precedence;
}

UnaryNodeBase::UnaryNodeBase(unique_ptr<NodeBase> arg, int precedence)
    : NodeBase(precedence), arg(std::move(arg)) {
}

BinaryNodeBase::BinaryNodeBase(unique_ptr<NodeBase> left, unique_ptr<NodeBase> right, int precedence)
    : NodeBase(precedence), left(std::move(left)), right(std::move(right)) {
}

NodeVal::NodeVal(int val) : NodeBase(valPrecedence), val(val) {
}

int NodeVal::evaluate() const {
    return val;
}

string NodeVal::toString() const {
    return std::to_string(val);
}

unique_ptr<NodeBase> NodeVal::clone() const {
    return make_unique<NodeVal>(*this);
}

unique_ptr<NodeBase> NodeVal::differentiate(char wrt) const {
    return make_unique<NodeVal>(0);
}

NodeVar::NodeVar(char symbol)
    : NodeBase(valPrecedence), symbol(symbol) {
}

int NodeVar::evaluate() const {
    return 0;
}

string NodeVar::toString() const {
    return string(1, symbol);
}

unique_ptr<NodeBase> NodeVar::clone() const {
    return make_unique<NodeVar>(*this);
}

unique_ptr<NodeBase> NodeVar::differentiate(char wrt) const {
    return symbol == wrt ? make_unique<NodeVal>(1) : make_unique<NodeVal>(0);
}

NodeAddInverse::NodeAddInverse(unique_ptr<NodeBase> arg)
    : UnaryNodeBase(std::move(arg), unaryPrecedence) {
}

int NodeAddInverse::evaluate() const {
    return -1 * arg->evaluate();
}

string NodeAddInverse::toString() const {
    if (precedence > arg->getPrecedence()) {
        return "-(" + arg->toString() + ")";
    } else {
        return "-" + arg->toString();
    }
}

unique_ptr<NodeBase> NodeAddInverse::clone() const {
    return make_unique<NodeAddInverse>(arg->clone());
}

unique_ptr<NodeBase> NodeAddInverse::differentiate(char wrt) const {
    return make_unique<NodeAddInverse>(arg->differentiate(wrt));
}

NodeSin::NodeSin(unique_ptr<NodeBase> arg)
    : UnaryNodeBase(std::move(arg), unaryPrecedence) {
}

int NodeSin::evaluate() const {
    return sin(arg->evaluate() * M_PI / 180); // convert to radians
}

string NodeSin::toString() const {
    return "sin(" + arg->toString() + ")";
}

unique_ptr<NodeBase> NodeSin::clone() const {
    return make_unique<NodeSin>(arg->clone());
}

unique_ptr<NodeBase> NodeSin::differentiate(char wrt) const {
    return make_unique<NodeMultiply>(arg->differentiate(wrt), make_unique<NodeCos>(arg->clone()));
}

NodeCos::NodeCos(unique_ptr<NodeBase> arg)
    : UnaryNodeBase(std::move(arg), unaryPrecedence) {
}

int NodeCos::evaluate() const {
    return cos(arg->evaluate() * M_PI / 180); // convert to radians
}

string NodeCos::toString() const {
    return "cos(" + arg->toString() + ")";
}

unique_ptr<NodeBase> NodeCos::clone() const {
    return make_unique<NodeCos>(arg->clone());
}

unique_ptr<NodeBase> NodeCos::differentiate(char wrt) const {
    return make_unique<NodeMultiply>
           (make_unique<NodeAddInverse>(arg->differentiate(wrt)), make_unique<NodeSin>(arg->clone()));
}

NodeExp::NodeExp(unique_ptr<NodeBase> arg)
    : UnaryNodeBase(std::move(arg), unaryPrecedence) {
}

int NodeExp::evaluate() const {
    return exp(arg->evaluate());
}

string NodeExp::toString() const {
    return "exp(" + arg->toString() + ")";
}

unique_ptr<NodeBase> NodeExp::clone() const {
    return make_unique<NodeExp>(arg->clone());
}

unique_ptr<NodeBase> NodeExp::differentiate(char wrt) const {
    return make_unique<NodeMultiply>(arg->differentiate(wrt), make_unique<NodeExp>(arg->clone()));
}

NodeLog::NodeLog(unique_ptr<NodeBase> arg)
    : UnaryNodeBase(std::move(arg), unaryPrecedence) {
}

int NodeLog::evaluate() const {
    return log(arg->evaluate());
}

string NodeLog::toString() const {
    return "log(" + arg->toString() + ")";
}

unique_ptr<NodeBase> NodeLog::clone() const {
    return make_unique<NodeLog>(arg->clone());
}

unique_ptr<NodeBase> NodeLog::differentiate(char wrt) const {
    return make_unique<NodeDivide>(arg->differentiate(wrt), arg->clone());
}

NodeAdd::NodeAdd(unique_ptr<NodeBase> left, unique_ptr<NodeBase> right)
    : BinaryNodeBase(std::move(left), std::move(right), addPrecedence) {
}

int NodeAdd::evaluate() const {
    return left->evaluate() + right->evaluate();
}

string NodeAdd::toString() const {
    string leftString;
    string rightString;

    if (left->getPrecedence() < precedence) {
        leftString = "(" + left->toString() + ")";
    } else {
        leftString = left->toString();
    }

    if (right->getPrecedence() < precedence) {
        rightString = "(" + right->toString() + ")";
    } else {
        rightString = right->toString();
    }

    return leftString + "+" + rightString;
}

unique_ptr<NodeBase> NodeAdd::clone() const {
    return make_unique<NodeAdd>(left->clone(), right->clone());
}

unique_ptr<NodeBase> NodeAdd::differentiate(char wrt) const {
    return make_unique<NodeAdd>(left->differentiate(wrt), right->differentiate(wrt));
}

NodeSubtract::NodeSubtract(unique_ptr<NodeBase> left, unique_ptr<NodeBase> right)
    : BinaryNodeBase(std::move(left), std::move(right), addPrecedence) {
}

int NodeSubtract::evaluate() const {
    return left->evaluate() - right->evaluate();
}

string NodeSubtract::toString() const {
    string leftString;
    string rightString;

    if (left->getPrecedence() < precedence) {
        leftString = "(" + left->toString() + ")";
    } else {
        leftString = left->toString();
    }

    if (right->getPrecedence() < precedence) {
        rightString = "(" + right->toString() + ")";
    } else {
        rightString = right->toString();
    }

    return leftString + "-" + rightString;
}

unique_ptr<NodeBase> NodeSubtract::clone() const {
    return make_unique<NodeSubtract>(left->clone(), right->clone());
}

unique_ptr<NodeBase> NodeSubtract::differentiate(char wrt) const {
    return make_unique<NodeSubtract>(left->differentiate(wrt), right->differentiate(wrt));
}

NodeMultiply::NodeMultiply(unique_ptr<NodeBase> left, unique_ptr<NodeBase> right)
    : BinaryNodeBase(std::move(left), std::move(right), multiplyPrecendence) {
}

int NodeMultiply::evaluate() const {
    return left->evaluate() * right->evaluate();
}

string NodeMultiply::toString() const {
    string leftString;
    string rightString;

    if (left->getPrecedence() < precedence) {
        leftString = "(" + left->toString() + ")";
    } else {
        leftString = left->toString();
    }

    if (right->getPrecedence() < precedence) {
        rightString = "(" + right->toString() + ")";
    } else {
        rightString = right->toString();
    }

    return leftString + "*" + rightString;
}

unique_ptr<NodeBase> NodeMultiply::clone() const {
    return make_unique<NodeMultiply>(left->clone(), right->clone());
}

unique_ptr<NodeBase> NodeMultiply::differentiate(char wrt) const {
    return make_unique<NodeAdd>(make_unique<NodeMultiply>(left->differentiate(wrt), right->clone()),
                                     make_unique<NodeMultiply>(right->differentiate(wrt), left->clone()));
}

NodeDivide::NodeDivide(unique_ptr<NodeBase> left, unique_ptr<NodeBase> right)
    : BinaryNodeBase(std::move(left), std::move(right), multiplyPrecendence) {
}

int NodeDivide::evaluate() const {
    return left->evaluate() / right->evaluate();
}

string NodeDivide::toString() const {
    string leftString;
    string rightString;

    if (left->getPrecedence() < precedence) {
        leftString = "(" + left->toString() + ")";
    } else {
        leftString = left->toString();
    }

    if (right->getPrecedence() < precedence) {
        rightString = "(" + right->toString() + ")";
    } else {
        rightString = right->toString();
    }

    return leftString + "/" + rightString;
}

unique_ptr<NodeBase> NodeDivide::clone() const {
    return make_unique<NodeDivide>(left->clone(), right->clone());
}

unique_ptr<NodeBase> NodeDivide::differentiate(char wrt) const {
    return make_unique<NodeDivide>(make_unique<NodeSubtract>(
        make_unique<NodeMultiply>(left->differentiate(wrt), right->clone()),
        make_unique<NodeMultiply>(right->differentiate(wrt), left->clone())),
        make_unique<NodeExponent>(right->clone(), make_unique<NodeVal>(2)));
}

NodeExponent::NodeExponent(unique_ptr<NodeBase> left, unique_ptr<NodeBase> right)
    : BinaryNodeBase(std::move(left), std::move(right), exponentPrecedence) {
}

int NodeExponent::evaluate() const {
    return pow(left->evaluate(), right->evaluate());
}

string NodeExponent::toString() const {
    string leftString;
    string rightString;

    if (left->getPrecedence() < precedence) {
        leftString = "(" + left->toString() + ")";
    } else {
        leftString = left->toString();
    }

    if (right->getPrecedence() <= precedence) {
        rightString = "(" + right->toString() + ")";
    } else {
        rightString = right->toString();
    }

    return leftString + "^" + rightString;
}

unique_ptr<NodeBase> NodeExponent::clone() const {
    return make_unique<NodeExponent>(left->clone(), right->clone());
}

unique_ptr<NodeBase> NodeExponent::differentiate(char wrt) const {
    return make_unique<NodeVal>(1); // TODO
}
