#pragma once

#include <memory>
#include <cmath>
#include <string>

class NodeBase {
public:
    NodeBase(int precedence);
    virtual ~NodeBase() = default;

    virtual int evaluate() const = 0;

    virtual std::string toString() const = 0;

public:
    const int precedence;
};

class UnaryNodeBase : public NodeBase {
public:
    UnaryNodeBase(std::unique_ptr<NodeBase> arg = nullptr, int precedence = 0);

    virtual int evaluate() const = 0;

protected:
    std::unique_ptr<NodeBase> arg;
};

class BinaryNodeBase : public NodeBase {
public:
    BinaryNodeBase(std::unique_ptr<NodeBase> left = nullptr, std::unique_ptr<NodeBase> right = nullptr, int precedence = 0);

    virtual int evaluate() const = 0;

protected:
    std::unique_ptr<NodeBase> left;
    std::unique_ptr<NodeBase> right;
};

class NodeVal : public NodeBase {
public:
    NodeVal(int val, int precedence = 5);

    int evaluate() const override;

    std::string toString() const override;

public:
    int val;
};

class NodeVar : public NodeBase {
public:
    NodeVar(char symbol, int precedence = 5);

    int evaluate() const override;

    std::string toString() const override;

public:
    char symbol;
};

class NodeAddInverse : public UnaryNodeBase {
public:
    NodeAddInverse(std::unique_ptr<NodeBase> arg = nullptr, int precedence = 1);

    int evaluate() const override;

    std::string toString() const override;
};

class NodeSin : public UnaryNodeBase {
public:
    NodeSin(std::unique_ptr<NodeBase> arg = nullptr, int precedence = 4);

    int evaluate() const override;

    std::string toString() const override;
};

class NodeCos : public UnaryNodeBase {
public:
    NodeCos(std::unique_ptr<NodeBase> arg = nullptr, int precedence = 4);

    int evaluate() const override;

    std::string toString() const override;
};

class NodeExp : public UnaryNodeBase {
public:
    NodeExp(std::unique_ptr<NodeBase> arg = nullptr, int precedence = 4);

    int evaluate() const override;

    std::string toString() const override;
};

class NodeLog : public UnaryNodeBase {
public:
    NodeLog(std::unique_ptr<NodeBase> arg = nullptr, int precedence = 4);

    int evaluate() const override;

    std::string toString() const override;
};

class NodeAdd : public BinaryNodeBase {
public:
    NodeAdd(std::unique_ptr<NodeBase> left = nullptr, std::unique_ptr<NodeBase> right = nullptr, int precedence = 1);

    int evaluate() const override;

    std::string toString() const override;
};

class NodeMultiply : public BinaryNodeBase {
public:
    NodeMultiply(std::unique_ptr<NodeBase> left = nullptr, std::unique_ptr<NodeBase> right = nullptr, int precedence = 2);

    int evaluate() const override;

    std::string toString() const override;
};

class NodeExponent : public BinaryNodeBase {
public:
    NodeExponent(std::unique_ptr<NodeBase> left = nullptr, std::unique_ptr<NodeBase> right = nullptr, int precedence = 3);

    int evaluate() const override;

    std::string toString() const override;
};
