#pragma once

#include "dfa.h"
#include <assert.h>

class Tester
{
  public:
    Tester(int n)
    {
      for (int i = 0; i < n; ++i)
      {
        std::cout << "\nTest n.°" << i << "\n-----------------\n";

        DFA dfa(rand()%6 + 2);
        dfa.question1();
        dfa.question2();
        //assert(dfa.getSize() >= 2);
        //std::cout << "Assert test n.° " << i << "\n";
        //std::cout << "Size: " << dfa.getSize() << "\n";
      }
    }
};
