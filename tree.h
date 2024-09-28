#pragma once

#include <memory>
#include <cmath>

class NodeBase {
public:
    virtual int evaluate() const = 0;
};

class UnaryNodeBase : public NodeBase {
public:
    UnaryNodeBase(std::unique_ptr<NodeBase> arg = nullptr);

    virtual int evaluate() const = 0;

protected:
    std::unique_ptr<NodeBase> arg;
};

class BinaryNodeBase : public NodeBase {
public:
    BinaryNodeBase(std::unique_ptr<NodeBase> left = nullptr, std::unique_ptr<NodeBase> right = nullptr);
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

public:
    int val;
};

class NodeVar : public NodeBase {
public:
    NodeVar(char symbol);

    int evaluate() const override;

public:
    char symbol;
};

class NodeAddInverse : public UnaryNodeBase {
public:
    NodeAddInverse(std::unique_ptr<NodeBase> arg = nullptr);

    int evaluate() const override;
};

class NodeSin : public UnaryNodeBase {
public:
    NodeSin(std::unique_ptr<NodeBase> arg = nullptr);

    int evaluate() const override;
};

class NodeCos : public UnaryNodeBase {
public:
    NodeCos(std::unique_ptr<NodeBase> arg = nullptr);

    int evaluate() const override;
};

class NodeExp : public UnaryNodeBase {
public:
    NodeExp(std::unique_ptr<NodeBase> arg = nullptr);

    int evaluate() const override;
};

class NodeLog : public UnaryNodeBase {
public:
    NodeLog(std::unique_ptr<NodeBase> arg = nullptr);

    int evaluate() const override;
};

class NodeAdd : public BinaryNodeBase {
public:
    NodeAdd(std::unique_ptr<NodeBase> left = nullptr, std::unique_ptr<NodeBase> right = nullptr);

    int evaluate() const override;
};

class NodeMultiply : public BinaryNodeBase {
public:
    NodeMultiply(std::unique_ptr<NodeBase> left = nullptr, std::unique_ptr<NodeBase> right = nullptr);

    int evaluate() const override;
};

class NodeExponent : public BinaryNodeBase {
public:
    NodeExponent(std::unique_ptr<NodeBase> left = nullptr, std::unique_ptr<NodeBase> right = nullptr);

    int evaluate() const override;
};
