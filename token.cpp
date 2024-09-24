#include "token.h"

using std::vector;
using std::string;

vector<Token> tokenize(const string& expression) {
    vector<Token> tokens;
    unsigned int pos = 0;

    while (pos < expression.length()) {
        char c = expression[pos];
        
        if (isdigit(c)) {
            int num = 0;
            while (pos < expression.length() && isdigit(expression[pos])) {
                num = num * 10 + (expression[pos] - '0');
                ++pos;
            }
            tokens.push_back(Token{TokenType::Number, num});
        } else {
            if (c == '+') {
                tokens.push_back(Token{TokenType::Add, 0});
            } else if (c == '-') {
                tokens.push_back(Token{TokenType::Subtract, 0});
            } else if (c == '*') {
                tokens.push_back(Token{TokenType::Multiply, 0});
            } else if (c == '^') {
                tokens.push_back(Token{TokenType::Exponent, 0});
            } else if (c == '(') {
                tokens.push_back(Token{TokenType::OpenParentheses, 0});
            } else if (c == ')') {
                tokens.push_back(Token{TokenType::CloseParentheses, 0});
            }

            ++pos;
        }
    }

    return tokens;
}
