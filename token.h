#pragma once

#include <string>
#include <vector>

enum class TokenType {Number, Add, Subtract, Multiply, Divide, Exponent, OpenParentheses, CloseParentheses, Variable, Function};

struct Token {
    TokenType type;
    int val; // if type is TokenType::Number
    std::string name; // if type is TokenType::Variable or TokenType::Function
};

std::vector<Token> tokenize(const std::string& expression);
