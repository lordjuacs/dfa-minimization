#pragma once

#include "dfa.h"
#include <assert.h>

class Tester
{
  public:
    Tester(int n, int m)
    {
      for (int i = 0; i < n; ++i)
      {
        std::cout << "\nTest n.°" << i << "\n-----------------\n";

        DFA dfa(rand()%6 + m);
        //dfa.question1();
        //dfa.question2();
        auto results1 = dfa.question1();
        auto results2 = dfa.question2();
        // Verifico que el nuevo size de estados totales sea el mismo
        assert(results1.first == results2.first);
        // Verifico que el tiempo de ejecución de la pregunta 2 sea menor al del 1
        assert(results1.second > results2.second);
      }
    }
};
