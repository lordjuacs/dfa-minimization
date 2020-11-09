#ifndef DFA_MINIMIZATION_DFA_H
#define DFA_MINIMIZATION_DFA_H

#include "brzozowski.h"
#include <unordered_set>

using transitions = std::unordered_map<int, std::pair<int, int>>;

struct DFA{
    int size;
    int initial;
    std::unordered_set<int> final_states;
    transitions trans;

    DFA(){
        int n_final;

        std::cin >> size;
        std::cin >> initial;
        std::cin >> n_final;

        for(int i = 0; i < n_final; ++i){
            int temp;
            std::cin >> temp;
            final_states.insert(temp);
        }

        int from, with, to;
        for(int i = 0; i < 2*size; ++i){
            std::cin >> from;
            std::cin >> with;
            std::cin >> to;
            with? trans[from].second = to : trans[from].first = to;
        }
    }
};



#endif //DFA_MINIMIZATION_DFA_H

