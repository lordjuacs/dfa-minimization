#include "tester.h"

int main() {
    //Run the tester

    /*
    srand(time(NULL));
    Tester tester(1000, 2, false);
    */

    //Run with inputs

    DFA dfa;
    dfa.check_synchronizing_word();
    /*auto results1 = dfa.question1();
    auto results2 = dfa.question2();
    auto results3 = dfa.question3();
    auto results4 = dfa.question4();
    auto results5 = dfa.question5();
    assert(results1.first == results5.first);
    assert(results1.first == results4.first);
*/

    //Coded by:
    // Diego Ortiz
    // Victor Ostolaza
    // Joaquin Ramirez

    // UTEC - December 2020
    return 0;
}
