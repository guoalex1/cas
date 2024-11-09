#pragma once

#include <memory>
#include <cmath>
#include <string>

/*
Precedence for nodes (order of operations):
5: NodeVal, NodeVar
4: Functions, NodeAddInverse
3: NodeExponent
2: NodeMultiply, NodeDivide
1: NodeAdd, NodeSubtract
*/

class NodeBase {
public:
    NodeBase(int precedence);
    virtual ~NodeBase() = default;

    virtual int evaluate() const = 0;

    virtual std::string toString() const = 0;

    virtual std::unique_ptr<NodeBase> clone() const = 0;

    int getPrecedence() const;

protected:
    const int precedence;
};

class UnaryNodeBase : public NodeBase {
public:
    UnaryNodeBase(std::unique_ptr<NodeBase> arg, int precedence);
    virtual ~UnaryNodeBase() = default;

    virtual int evaluate() const = 0;

protected:
    std::unique_ptr<NodeBase> arg;
};

class BinaryNodeBase : public NodeBase {
public:
    BinaryNodeBase(std::unique_ptr<NodeBase> left, std::unique_ptr<NodeBase> right, int precedence);
    virtual ~BinaryNodeBase() = default;

    virtual int evaluate() const = 0;

protected:
    std::unique_ptr<NodeBase> left;
    std::unique_ptr<NodeBase> right;
};

class NodeVal : public NodeBase {
public:
    NodeVal(int val);

    int evaluate() const override;

    std::string toString() const override;

    std::unique_ptr<NodeBase> clone() const override;

public:
    int val;
};

class NodeVar : public NodeBase {
public:
    NodeVar(char symbol);

    int evaluate() const override;

    std::string toString() const override;

    std::unique_ptr<NodeBase> clone() const override;

public:
    char symbol;
};

class NodeAddInverse : public UnaryNodeBase {
public:
    NodeAddInverse(std::unique_ptr<NodeBase> arg);

    int evaluate() const override;

    std::string toString() const override;

    std::unique_ptr<NodeBase> clone() const override;
};

class NodeSin : public UnaryNodeBase {
public:
    NodeSin(std::unique_ptr<NodeBase> arg);

    int evaluate() const override;

    std::string toString() const override;

    std::unique_ptr<NodeBase> clone() const override;
};

class NodeCos : public UnaryNodeBase {
public:
    NodeCos(std::unique_ptr<NodeBase> arg);

    int evaluate() const override;

    std::string toString() const override;

    std::unique_ptr<NodeBase> clone() const override;
};

class NodeExp : public UnaryNodeBase {
public:
    NodeExp(std::unique_ptr<NodeBase> arg);

    int evaluate() const override;

    std::string toString() const override;

    std::unique_ptr<NodeBase> clone() const override;
};

class NodeLog : public UnaryNodeBase {
public:
    NodeLog(std::unique_ptr<NodeBase> arg);

    int evaluate() const override;

    std::string toString() const override;

    std::unique_ptr<NodeBase> clone() const override;
};

class NodeAdd : public BinaryNodeBase {
public:
    NodeAdd(std::unique_ptr<NodeBase> left, std::unique_ptr<NodeBase> right);

    int evaluate() const override;

    std::string toString() const override;

    std::unique_ptr<NodeBase> clone() const override;
};

class NodeSubtract : public BinaryNodeBase {
public:
    NodeSubtract(std::unique_ptr<NodeBase> left, std::unique_ptr<NodeBase> right);

    int evaluate() const override;

    std::string toString() const override;

    std::unique_ptr<NodeBase> clone() const override;
};

class NodeMultiply : public BinaryNodeBase {
public:
    NodeMultiply(std::unique_ptr<NodeBase> left, std::unique_ptr<NodeBase> right);

    int evaluate() const override;

    std::string toString() const override;

    std::unique_ptr<NodeBase> clone() const override;
};

class NodeDivide : public BinaryNodeBase {
public:
    NodeDivide(std::unique_ptr<NodeBase> left, std::unique_ptr<NodeBase> right);

    int evaluate() const override;

    std::string toString() const override;

    std::unique_ptr<NodeBase> clone() const override;
};

class NodeExponent : public BinaryNodeBase {
public:
    NodeExponent(std::unique_ptr<NodeBase> left, std::unique_ptr<NodeBase> right);

    int evaluate() const override;

    std::string toString() const override;

    std::unique_ptr<NodeBase> clone() const override;
};
