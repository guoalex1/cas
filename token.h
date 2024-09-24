#pragma once

#include <string>
#include <vector>

enum class TokenType {Number, Add, Subtract, Multiply, Exponent, OpenParentheses, CloseParentheses};

struct Token {
    TokenType type;
    int val; // only if type is TokenType::Number
};

std::vector<Token> tokenize(const std::string& expression);
