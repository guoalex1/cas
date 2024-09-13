#pragma once
#include <memory>

class NodeBase {
public:
    NodeBase(std::unique_ptr<NodeBase> left = nullptr, std::unique_ptr<NodeBase> right = nullptr);
    virtual ~NodeBase() = default;

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

class NodeAdd : public NodeBase {
public:
    NodeAdd(std::unique_ptr<NodeBase> left = nullptr, std::unique_ptr<NodeBase> right = nullptr);

    int evaluate() const override;
};

class NodeMultiply : public NodeBase {
public:
    NodeMultiply(std::unique_ptr<NodeBase> left = nullptr, std::unique_ptr<NodeBase> right = nullptr);

    int evaluate() const override;
};
