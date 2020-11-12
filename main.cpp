#include "tester.h"

int main() {
    srand(time(NULL));
    Tester tester(100, 2);
    
    /*
    DFA dfa;
    auto results1 = dfa.question1();
    auto results2 = dfa.question2();
    // Verifico que el nuevo size de estados totales sea el mismo
    assert(results1.first == results2.first);
    // Verifico que el tiempo de ejecución de la pregunta 2 sea menor al del 1
    //assert(results1.second >= results2.second);*/

/*
Figura 4.8 Un autómata con estados equivalentes.	// Figura 4.8 Un autómata con estados equivalentes.
8 0 1 2
0 0 1
0 1 5
1 0 6
1 1 2
2 0 0
2 1 2
3 0 2
3 1 6
4 0 7
4 1 5
5 0 2
5 1 6
6 0 6
6 1 4
7 0 6
7 1 2
*/

    return 0;
}
