#pragma once

#include "dfa.h"
#include <assert.h>

class Tester {
public:
    Tester(int n, int m, bool check_time) {
        for (int i = 0; i < n; ++i) {
            std::cout << "\nTest nro. " << i << "\n-----------------\n";
            DFA dfa(m);
            //dfa.question1();
            //dfa.question2();
            auto results1 = dfa.question1();
            auto results2 = dfa.question2();
            auto results3 = dfa.question3();
            auto results4 = dfa.question4();
            auto results5 = dfa.question5();
            // Verifico que el nuevo size de estados totales sea el mismo
            assert(results1.first == results4.first);
            // Checking time performance: question 2 better/equal to question 1
            //if (check_time)
            //    assert(results1.second >= results2.second);
        }
    }
};
