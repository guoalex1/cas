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

enum class Mode{EVAL, DIFF};

void help() {
    cout << "Commands: " << endl;
    cout << "Evaluation mode: /e" << endl;
    cout << "Differentiate mode: /d <wrt>" << endl;
    cout << "Help: /h" << endl;
    cout << "Quit: /q" << endl;
}

void header(Mode m, char wrt) {
    if (m == Mode::DIFF) {
        cout << "Differentiation mode (wrt " << wrt << "):" << endl;
    } else {
        cout << "Evaluation mode:" << endl;
    }
}

int main(int argc, char** argv) {
    string expression;
    Mode mode = Mode::EVAL;
    char wrt = 'x';

    help(); 

    while (std::getline(cin, expression)) {
        if (expression == "/q") {
            break;
        } else if (expression == "/h") {
            help();
            continue;
        } else if (expression == "/e") {
            mode = Mode::EVAL;
            header(mode, wrt);
            continue;
        } else if (expression.substr(0, 2) == "/d") {
            mode = Mode::DIFF;
            wrt = expression.length() > 3 ? expression[3] : 'x';
            header(mode, wrt);
            continue;
        }

        vector<Token> tokens = tokenize(expression);

        if (! tokens.empty()) {
            unique_ptr<NodeBase> node = buildTree(tokens);
            if (mode == Mode::EVAL) {
                cout << "= " << node->evaluate() << endl;
            } else if (mode == Mode::DIFF) {
                cout << "d/d" << wrt << "(" << expression << ") = " << node->differentiate(wrt)->toString() << endl;
            }
        }

        header(mode, wrt);
    }

    return 0;
}
