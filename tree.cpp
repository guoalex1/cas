#include "tree.h"

#include <iostream> // debug
#include <cmath>

const int valPrecedence = 5; // NodeVal, NodeVar
const int unaryPrecedence = 4; // functions, NodeAddInverse
const int exponentPrecedence = 3; // NodeExponent
const int multiplyPrecedence = 2; // NodeMultiply, NodeDivide
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

unique_ptr<NodeBase> NodeVal::simplify() const {
    return make_unique<NodeVal>(val);
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

unique_ptr<NodeBase> NodeVar::simplify() const {
    return make_unique<NodeVar>(symbol);
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

unique_ptr<NodeBase> NodeAddInverse::simplify() const {
    return make_unique<NodeAddInverse>(arg->simplify());
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

unique_ptr<NodeBase> NodeSin::simplify() const {
    return make_unique<NodeSin>(arg->simplify());
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

unique_ptr<NodeBase> NodeCos::simplify() const {
    return make_unique<NodeCos>(arg->simplify());
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

unique_ptr<NodeBase> NodeExp::simplify() const {
    unique_ptr<NodeBase> argSimple = arg->simplify();

    NodeVal* leftVal = dynamic_cast<NodeVal*>(argSimple.get());

    if (leftVal != nullptr && argSimple->evaluate() == 0) {
        return make_unique<NodeVal>(1);
    } else {
        return make_unique<NodeExp>(std::move(argSimple));
    }
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

unique_ptr<NodeBase> NodeLog::simplify() const {
    unique_ptr<NodeBase> argSimple = arg->simplify();

    NodeVal* leftVal = dynamic_cast<NodeVal*>(argSimple.get());

    if (leftVal != nullptr && argSimple->evaluate() == 1) {
        return make_unique<NodeVal>(0);
    } else {
        return make_unique<NodeLog>(std::move(argSimple));
    }
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

unique_ptr<NodeBase> NodeAdd::simplify() const {
    unique_ptr<NodeBase> leftSimple = left->simplify();
    unique_ptr<NodeBase> rightSimple = right->simplify();

    NodeBase* leftVal = dynamic_cast<NodeVal*>(leftSimple.get());
    NodeBase* rightVal = dynamic_cast<NodeVal*>(rightSimple.get());

    // check for special simplification rules
    if (leftVal != nullptr && rightVal != nullptr) {
        return make_unique<NodeVal>(leftVal->evaluate() + rightVal->evaluate());
    } else if (leftVal != nullptr && leftVal->evaluate() == 0) {
        return rightSimple;
    } else if (rightVal != nullptr && rightVal->evaluate() == 0) {
        return leftSimple;
    } else {
        return make_unique<NodeAdd>(std::move(leftSimple), std::move(rightSimple));
    }
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

unique_ptr<NodeBase> NodeSubtract::simplify() const {
    unique_ptr<NodeBase> leftSimple = left->simplify();
    unique_ptr<NodeBase> rightSimple = right->simplify();

    NodeBase* leftVal = dynamic_cast<NodeVal*>(leftSimple.get());
    NodeBase* rightVal = dynamic_cast<NodeVal*>(rightSimple.get());

    // check for special simplification rules
    if (leftVal != nullptr && rightVal != nullptr) {
        return make_unique<NodeVal>(leftVal->evaluate() - rightVal->evaluate());
    } else if (leftVal != nullptr && leftVal->evaluate() == 0) {
        return make_unique<NodeAddInverse>(std::move(rightSimple));
    } else if (rightVal != nullptr && rightVal->evaluate() == 0) {
        return leftSimple;
    } else {
        return make_unique<NodeSubtract>(std::move(leftSimple), std::move(rightSimple));
    }
}

NodeMultiply::NodeMultiply(unique_ptr<NodeBase> left, unique_ptr<NodeBase> right)
    : BinaryNodeBase(std::move(left), std::move(right), multiplyPrecedence) {
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
                                make_unique<NodeMultiply>(left->clone(), right->differentiate(wrt)));
}

unique_ptr<NodeBase> NodeMultiply::simplify() const {
    unique_ptr<NodeBase> leftSimple = left->simplify();
    unique_ptr<NodeBase> rightSimple = right->simplify();

    // check for special simplification rules
    NodeBase* leftVal = dynamic_cast<NodeVal*>(leftSimple.get());
    NodeBase* rightVal = dynamic_cast<NodeVal*>(rightSimple.get());

    if (leftVal != nullptr && rightVal != nullptr) {
        return make_unique<NodeVal>(leftVal->evaluate() * rightVal->evaluate());
    } else if (leftVal != nullptr && leftVal->evaluate() == 1) {
        return rightSimple;
    } else if (leftVal != nullptr && leftVal->evaluate() == -1) {
        return make_unique<NodeAddInverse>(std::move(rightSimple));
    } else if (leftVal != nullptr && leftVal->evaluate() == 0) {
        return make_unique<NodeVal>(0);
    } else if (rightVal != nullptr && rightVal->evaluate() == 1) {
        return leftSimple;
    } else if (rightVal != nullptr && rightVal->evaluate() == -1) {
        return make_unique<NodeAddInverse>(std::move(leftSimple));
    } else if (rightVal != nullptr && rightVal->evaluate() == 0) {
        return make_unique<NodeVal>(0);
    } else {
        return make_unique<NodeMultiply>(std::move(leftSimple), std::move(rightSimple));
    }
}

NodeDivide::NodeDivide(unique_ptr<NodeBase> left, unique_ptr<NodeBase> right)
    : BinaryNodeBase(std::move(left), std::move(right), multiplyPrecedence) {
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

unique_ptr<NodeBase> NodeDivide::simplify() const {
    unique_ptr<NodeBase> leftSimple = left->simplify();
    unique_ptr<NodeBase> rightSimple = right->simplify();

    NodeBase* leftVal = dynamic_cast<NodeVal*>(leftSimple.get());
    NodeBase* rightVal = dynamic_cast<NodeVal*>(rightSimple.get());

    // check for special simplification rules
    if (leftVal != nullptr && rightVal != nullptr) {
        return make_unique<NodeVal>(leftVal->evaluate() * rightVal->evaluate());
    } else if (leftVal != nullptr && leftVal->evaluate() == 0) {
        return make_unique<NodeVal>(0);
    } else if (rightVal != nullptr && rightVal->evaluate() == 1) {
        return leftSimple;
    } else if (rightVal != nullptr && rightVal->evaluate() == -1) {
        return make_unique<NodeAddInverse>(std::move(leftSimple));
    } else {
        return make_unique<NodeDivide>(std::move(leftSimple), std::move(rightSimple));;
    }
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

// power rule
unique_ptr<NodeBase> NodeExponent::differentiate(char wrt) const {
    return make_unique<NodeMultiply>(left->differentiate(wrt),
                                    make_unique<NodeMultiply>(make_unique<NodeVal>(right->evaluate()),
                                                              make_unique<NodeExponent>(left->clone(),
                                                                                        make_unique<NodeVal>(right->evaluate() - 1))));
}

unique_ptr<NodeBase> NodeExponent::simplify() const {
    unique_ptr<NodeBase> leftSimple = left->simplify();
    unique_ptr<NodeBase> rightSimple = right->simplify();

    NodeBase* leftVal = dynamic_cast<NodeVal*>(leftSimple.get());
    NodeBase* rightVal = dynamic_cast<NodeVal*>(rightSimple.get());

    // check for special simplification rules
    if (leftVal != nullptr && rightVal != nullptr) {
        return make_unique<NodeVal>(leftVal->evaluate() * rightVal->evaluate());
    } else if (leftVal != nullptr && leftVal->evaluate() == 0) {
        return make_unique<NodeVal>(0);
    } else if (leftVal != nullptr && leftVal->evaluate() == 1) {
        return make_unique<NodeVal>(1);
    } else if (rightVal != nullptr && rightVal->evaluate() == 0) {
        return make_unique<NodeVal>(1);
    } else if (rightVal != nullptr && rightVal->evaluate() == 1) {
        return leftSimple;
    } else {
        return clone();
    }
}
