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
            node = std::make_unique<NodeSubtract>(std::move(node), std::move(right));
        }
    }

    return node;
}

std::unique_ptr<NodeBase> parseExpressionMultiplication(const std::vector<Token>& tokens, unsigned int& pos) {
    std::unique_ptr<NodeBase> node = parseExpressionExponent(tokens, pos);

    while (tokens[pos].type == TokenType::Multiply || tokens[pos].type == TokenType::Divide) {
        TokenType operation = tokens[pos].type; // Unused for now
        ++pos;
        std::unique_ptr<NodeBase> right = parseExpressionExponent(tokens, pos);

        if (operation == TokenType::Multiply) {
            node = std::make_unique<NodeMultiply>(std::move(node), std::move(right));
        } else if (operation == TokenType::Divide) {
            node = std::make_unique<NodeDivide>(std::move(node), std::move(right));
        }
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
    bool negate = false;

    while (tokens[pos].type == TokenType::Subtract) {
        negate = ! negate;

        ++pos;
    }

    std::unique_ptr<NodeBase> node;

    if (tokens[pos].type == TokenType::OpenParentheses) {
        ++pos; // skip (
        node = parseExpressionAddition(tokens, pos);
        ++pos; // skip )
    } else if (tokens[pos].type == TokenType::Function) {
        std::string function = tokens[pos++].name;
        std::unique_ptr<NodeBase> arg = parseExpressionVal(tokens, pos);

        if (function == "sin") {
            node = std::make_unique<NodeSin>(std::move(arg));
        } else if (function == "cos") {
            node = std::make_unique<NodeCos>(std::move(arg));
        } else if (function == "exp") {
            node = std::make_unique<NodeExp>(std::move(arg));
        } else if (function == "log") {
            node = std::make_unique<NodeLog>(std::move(arg));
        }
    } else if (tokens[pos].type == TokenType::Variable) {
        node = std::make_unique<NodeVar>(tokens[pos++].name[0]);
    } else {
        node = std::make_unique<NodeVal>(tokens[pos++].val);
    }

    if (negate) {
        return std::make_unique<NodeAddInverse>(std::move(node));
    } else {
        return node;
    }
}
