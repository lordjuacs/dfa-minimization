#include <ctime>
#include "tester.h"

int main() {
    srand(time(NULL));
    Tester tester(10);
    //DFA dfa;
    //dfa.question1();
    //dfa.question2();
    //dfa.question2();
    //std::cout << dfa.getSize() << std::endl;
    //question1();
    //question2();

/*
INPUTS QUE NO FUNCIONAN EN PREGUNTA 1, porque devuelven más estados en lugar de menos o igual
5 1 1 4 
0 0 0
0 1 1
1 0 2
1 1 3
2 0 2
2 1 0
3 0 4
3 1 1
4 0 2
4 1 1

4 2 2 0 3 
0 0 3
0 1 3
1 0 3
1 1 1
2 0 0
2 1 2
3 0 1
3 1 2
*/

/*
7 4 1 3 
0 0 5
0 1 5
1 0 5
1 1 2
2 0 1
2 1 2
3 0 5
3 1 5
4 0 5
4 1 5
5 0 0
5 1 1
6 0 4
6 1 6
*/

/*testing
2 0 1 1
0 0 1
0 1 0
1 0 0
1 1 1
6 0 3 2 3 4
0 0 1
0 1 2
1 0 0
1 1 3
2 0 4
2 1 5
3 0 4
3 1 5
4 0 4
4 1 5
5 0 5
5 1 5
6 0 2 3 5
0 0 1
0 1 3
1 0 0
1 1 3
2 0 1
2 1 4
3 0 5
3 1 5
4 0 3
4 1 3
5 0 5
5 1 5
5 0 1 4
0 0 1
0 1 3
1 0 1
1 1 2
2 0 1
2 1 4
3 0 1
3 1 3
4 0 1
4 1 3
     8 0 2 5 7
0 0 2
0 1 1
1 0 2
1 1 0
2 0 3
2 1 3
3 0 4
3 1 7
4 0 5
4 1 5
5 0 5
5 1 7
6 0 4
6 1 7
7 0 7
7 1 7
// Figura 4.8 Un autómata con estados equivalentes.
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
