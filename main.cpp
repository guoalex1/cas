#include "tree.h"
#include "token.h"
#include "parse.h"
#include <iostream>
#include <string>

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::unique_ptr;

int main(int argc, char** argv) {
    string expression;

    cout << "Enter an expression (q to quit):" << endl;
    while (std::getline(cin, expression)) {

        if (expression == "q") {
            break;
        }

        vector<Token> tokens = tokenize(expression);

        if (! tokens.empty()) {
            unique_ptr<NodeBase> node = buildTree(tokens);
            cout << "= " << node->evaluate() << endl;
        }

        cout << "Enter an expression (q to quit):" << endl;
    }

    return 0;
}
