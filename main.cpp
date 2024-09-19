#include "tree.h"
#include "parse.h"
#include <iostream>
#include <string>
#include <sstream>

using std::string;
using std::cin;
using std::cout;
using std::endl;

int main(int argc, char** argv) {

    while (true) {
        string line;
        cout << "Enter an expression (q to quit):" << endl;
        std::getline(cin, line);

        if (line == "q") {
            break;
        }

        string expression = "";
        std::istringstream stream(line);
        string temp;

        while (stream >> temp) {
            expression += temp;
        }

        std::unique_ptr<NodeBase> node = buildTree(expression);
        cout << "= " << node->evaluate() << endl;
    }

    return 0;
}
