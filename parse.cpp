#include "parse.h"
#include <iostream>

std::unique_ptr<NodeBase> buildTree(const std::vector<Token>& tokens) {
    unsigned int pos = 0;

    return parseExpressionAddition(tokens, pos);
}

std::unique_ptr<NodeBase> parseExpressionAddition(const std::vector<Token>& tokens, unsigned int& pos) {
    std::unique_ptr<NodeBase> node = parseExpressionMultiplication(tokens, pos);

    while (tokens[pos].type == TokenType::Add || tokens[pos].type == TokenType::Subtract) {
        TokenType operation = tokens[pos].type;
        ++pos;
        std::unique_ptr<NodeBase> right = parseExpressionMultiplication(tokens, pos);

        if (operation == TokenType::Add) {
            node = std::make_unique<NodeAdd>(std::move(node), std::move(right));
        } else if (operation == TokenType::Subtract) {
            node = std::make_unique<NodeAdd>(std::move(node), std::make_unique<NodeAddInverse>(std::move(right))); 
        }
    }

    return node;
}

std::unique_ptr<NodeBase> parseExpressionMultiplication(const std::vector<Token>& tokens, unsigned int& pos) {
    std::unique_ptr<NodeBase> node = parseExpressionExponent(tokens, pos);

    // TODO: Implement division
    while (tokens[pos].type == TokenType::Multiply) {
        TokenType operation = tokens[pos].type; // Unused for now
        ++pos;
        std::unique_ptr<NodeBase> right = parseExpressionExponent(tokens, pos);
        node = std::make_unique<NodeMultiply>(std::move(node), std::move(right));
    }

    return node;
}

std::unique_ptr<NodeBase> parseExpressionExponent(const std::vector<Token>& tokens, unsigned int& pos) {
    std::unique_ptr<NodeBase> node = parseExpressionVal(tokens, pos);

    while (tokens[pos].type == TokenType::Exponent) {
        ++pos;
        std::unique_ptr<NodeBase> right = parseExpressionVal(tokens, pos);
        node = std::make_unique<NodeExponent>(std::move(node), std::move(right));
    }

    return node;
}

std::unique_ptr<NodeBase> parseExpressionVal(const std::vector<Token>& tokens, unsigned int& pos) {
    if (tokens[pos].type == TokenType::OpenParentheses) {
        ++pos; // skip (
        std::unique_ptr<NodeBase> node = parseExpressionAddition(tokens, pos);
        ++pos; // skip )

        return node;
    } else if (tokens[pos].type == TokenType::Function) {
        std::string function = tokens[pos++].name;
        std::unique_ptr<NodeBase> arg = parseExpressionVal(tokens, pos);

        if (function == "sin") {
            return std::make_unique<NodeSin>(std::move(arg));
        } else if (function == "cos") {
            return std::make_unique<NodeCos>(std::move(arg));
        } else if (function == "exp") {
            return std::make_unique<NodeExp>(std::move(arg));
        } else if (function == "log") {
            return std::make_unique<NodeLog>(std::move(arg));
        }
    } else if (tokens[pos].type == TokenType::Variable) {
        return std::make_unique<NodeVar>(tokens[pos++].name[0]);
    } else {
        return std::make_unique<NodeVal>(tokens[pos++].val);
    }
}
