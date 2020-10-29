#include "brzozowski.h"
int main() {

    auto f_dfa = build_b_dfa();
    auto f_nfa = build_nfa(f_dfa);
    auto reverse_dfa = nfa_to_b_dfa(f_nfa);
    auto reverse_nfa = build_nfa(reverse_dfa);
    auto final = nfa_to_b_dfa(reverse_nfa);

    std::cout << std::get<2>(final).size() << " " << std::get<0>(final) << " " << std::get<1>(final).size() << " ";
    for(auto & e : std::get<1>(final))
        std::cout << e << " ";
    std::cout << std::endl;
    for(auto & e : std::get<2>(final)){
        std::cout << e.first << " ";
        std::cout << "0 ";
        std::cout << e.second.first << std::endl;
        std::cout << e.first << " ";
        std::cout << "1 ";
        std::cout << e.second.second << std::endl;
    }


    /*testing
5 0 3 0 1 3
0 0 1
0 1 3
1 0 1
1 1 2
2 0 1
2 1 2
3 0 4
3 1 3
4 0 4
4 1 3



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
     */
    return 0;
}
