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
    return 0;
}
