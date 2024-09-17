#include <iostream>
#include "tree.h"
#include "parse.h"
#include <string>
#include <cctype>

using std::string;
using std::cin;
using std::cout;
using std::endl;

bool checkWhitespace(const char c) {
    return std::isspace(static_cast<unsigned char>(c));
}

std::string removeWhitespace(const string& str) {
    string result;

    for (char c : str) {
        if (! checkWhitespace(c)) {
            result += c;
        }
    }

    return result;
}

int main(int argc, char** argv) {
    std::string expression;

    while (true) {
        cout << "Enter an expression (q to quit):" << endl;
        std::getline(cin, expression);

        if (expression == "q") {
            break;
        }

        expression = removeWhitespace(expression);
        std::unique_ptr<NodeBase> node = buildTree(expression);
        cout << "= " << node->evaluate() << endl;
    }

    return 0;
}
