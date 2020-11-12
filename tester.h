#pragma once

#include "dfa.h"
#include <assert.h>

class Tester {
public:
    Tester(int n, int m) {
        double cont = 0;
        for (int i = 0; i < n; ++i) {
            std::cout << "\nTest nro. " << i << "\n-----------------\n";
            DFA dfa(rand() % 6 + m);
            //dfa.question1();
            //dfa.question2();
            auto results1 = dfa.question1();
            auto results2 = dfa.question2();
            // Verifico que el nuevo size de estados totales sea el mismo
            assert(results1.first == results2.first);
            // Checking time performance: question 2 better/equal to question 1
            //assert(results1.second >= results2.second);
        }
    }
};
