#include "tester.h"

int main() {
    /*srand(time(NULL));
    Tester tester(1000, 2, false);
*/

    DFA dfa;
    auto results1 = dfa.question1();
    //auto result2 = dfa.question2();
    //auto result3 = dfa.question3();
    //auto result4 = dfa.question4();
    //auto result5 = dfa.question5();
    // Verifico que el nuevo size de estados totales sea el mismo
    //assert(results1.first == results2.first);
    // Verifico que el tiempo de ejecución de la pregunta 2 sea menor al del 1
    //assert(results1.second >= results2.second);*/
    return 0;
}
/*
Figura 4.8 Un autómata con estados equivalentes.	// Figura 4.8 Un autómata con estados equivalentes.
4 3 1 0
0 0 2
0 1 1
1 0 1
1 1 3
2 0 1
2 1 2
3 0 2
3 1 0




7 3 1 5
0 0 0
0 1 1
1 0 5
1 1 4
2 0 5
2 1 3
3 0 1
3 1 6
4 0 2
4 1 3
5 0 0
5 1 6
6 0 6
6 1 3


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



7 3 4 3 0 5 2
0 0 3
0 1 6
1 0 3
1 1 2
2 0 3
2 1 4
3 0 0
3 1 5
4 0 4
4 1 0
5 0 5
5 1 1
6 0 4
6 1 0





5 0 1 3
0 0 2
0 1 0
1 0 2
1 1 0
2 0 3
2 1 3
3 0 3
3 1 3
4 0 3
4 1 3
*/

