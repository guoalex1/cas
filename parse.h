#pragma once

#include "tree.h"
#include "token.h"

std::unique_ptr<NodeBase> buildTree(const std::vector<Token>& tokens);

std::unique_ptr<NodeBase> parseExpressionAddition(const std::vector<Token>& tokens, unsigned int& pos);
std::unique_ptr<NodeBase> parseExpressionMultiplication(const std::vector<Token>& tokens, unsigned int& pos);
std::unique_ptr<NodeBase> parseExpressionExponent(const std::vector<Token>& tokens, unsigned int& pos);
std::unique_ptr<NodeBase> parseExpressionVal(const std::vector<Token>& tokens, unsigned int& pos); // int or ()
