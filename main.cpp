#include "tree.h"
#include "token.h"
#include "parse.h"
#include <iostream>
#include <string>
#include <sstream>

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::vector;

int main(int argc, char** argv) {
    string line;

    cout << "Enter an expression (q to quit):" << endl;
    while (std::getline(cin, line)) {

        if (line == "q") {
            break;
        }

        string expression = "";
        std::istringstream stream(line);
        string temp;

        while (stream >> temp) {
            expression += temp;
        }

        vector<Token> tokens = tokenize(expression);

        std::unique_ptr<NodeBase> node = buildTree(tokens);
        cout << "= " << node->evaluate() << endl;
        cout << "Enter an expression (q to quit):" << endl;
    }

    return 0;
}
