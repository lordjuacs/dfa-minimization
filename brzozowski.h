#ifndef DFA_MINIMIZATION_BRZOZOWSKI_H
#define DFA_MINIMIZATION_BRZOZOWSKI_H
//
// Created by AIO on 28/10/2020.
//
#include <iostream>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <vector>
#include <queue>
using transitions = std::unordered_map<int, std::pair<int, int>>;
using to_states = std::vector<int>;
using n_transitions = std::unordered_map<int, std::pair<to_states, to_states>>;
using dfa = std::tuple<int, std::vector<int>, transitions>;
using nfa = std::tuple<int, int, n_transitions, std::vector<int>>;

dfa build_b_dfa(){
    int n, initial, n_final;
    std::cin >> n;
    std::cin >> initial;
    std::cin >> n_final;
    std::vector<int> final_states(n_final);
    for(int i = 0; i < n_final; ++i)
        std::cin >> final_states[i];
    transitions trans;
    int from, with, to;
    for(int i = 0; i < 2*n; ++i){
        std::cin >> from;
        std::cin >> with;
        std::cin >> to;
        with? trans[from].second = to : trans[from].first = to;
    }
    return {initial, final_states, trans};
}


void is_reachable(n_transitions &um, std::vector<bool> &reachable, int id){
    if (reachable[id])
        return;
    reachable[id] = true;
    for(const auto & e : um[id].first)
        is_reachable(um, reachable, e);
    for(const auto & e : um[id].second)
        is_reachable(um, reachable, e);
}

nfa build_nfa(dfa& original){
    n_transitions n_trans;
    std::vector<int> empty;
    for(int i = 0; i < std::get<2>(original).size(); ++i)
        n_trans.insert(std::make_pair(i, std::make_pair(empty, empty)));
    for(auto & it : std::get<2>(original)){
        n_trans[it.second.first].first.push_back(it.first);
        n_trans[it.second.second].second.push_back(it.first);
    }
    int initial;
    int final_state = std::get<0>(original);
    std::vector<int> e_trans;
    std::vector<bool> reachable(n_trans.size());
    if(std::get<1>(original).size() == 1) {
        initial = std::get<1>(original)[0];
        is_reachable(n_trans, reachable, initial);
    }
    else{
        initial = -1;
        for(auto &e : std::get<1>(original)) {
            e_trans.push_back(e);
            is_reachable(n_trans, reachable, e);
        }
    }
    for(int i = 0; i < reachable.size(); i++)
        if (!reachable[i]) {
            n_trans.erase(i);
        }
    return {initial, final_state, n_trans, e_trans};
}

dfa nfa_to_dfa(nfa& original){
    std::vector<int> initial;
    std::vector<int> closure;
    std::vector<std::vector<int>> exists;
    std::queue<std::vector<int>> checking;
    n_transitions trans;
    initial.push_back(std::get<0>(original));
    for(const auto& e : std::get<3>(original))
        initial.push_back(e);
    std::sort(initial.begin(), initial.end());
    exists.push_back(initial);
    std::vector<int> which;
    transitions final_trans;
    int cont = 0;

    for(int i = 0; i < 2; ++i) {
        for (const int &j : initial) {
            if(j != - 1) {
                which = i ? std::get<2>(original).at(j).second : std::get<2>(original).at(j).first;
                for (const int &k : which)
                    if (std::find(closure.begin(), closure.end(), k) == closure.end())
                        closure.push_back(k);
            }
        }
        std::sort(closure.begin(), closure.end());
        i ? trans[cont].second = closure : trans[cont].first = closure;
        if(std::find(exists.begin(), exists.end(), closure) == exists.end()) {
            checking.push(closure);
        }
        closure.clear();
    }
    std::vector<std::vector<int>> contains_final_states;

    if(std::find(initial.begin(), initial.end(), std::get<1>(original)) != initial.end())
        contains_final_states.push_back(initial);

    std::vector<int> front;
    std::vector<int> final_states;
    while(!checking.empty()){
        front = checking.front();
        checking.pop();
        if(std::find(exists.begin(), exists.end(), front) == exists.end()){
            cont++;
            for(int i = 0; i < 2; ++i) {
                for (const int &j : front) {
                    if (j != -1) {
                        which = i ? std::get<2>(original).at(j).second : std::get<2>(original).at(j).first;
                        for (const int &k : which)
                            if (std::find(closure.begin(), closure.end(), k) == closure.end())
                                closure.push_back(k);
                    }
                }
                std::sort(closure.begin(), closure.end());
                i ? trans[cont].second = closure : trans[cont].first = closure;
                checking.push(closure);
                closure.clear();
            }
            exists.push_back(front);
            if(std::find(front.begin(), front.end(), std::get<1>(original)) != front.end())
                contains_final_states.push_back(front);
        }
    }
    int pos;
    for(auto & tran : trans){
        pos = std::find(exists.begin(), exists.end(), tran.second.first) - exists.begin();
        final_trans[tran.first].first = pos;
        pos = std::find(exists.begin(), exists.end(), tran.second.second) - exists.begin();
        final_trans[tran.first].second = pos;
    }
    for (auto & contains_final_state : contains_final_states){
        final_states.push_back(std::find(exists.begin(), exists.end(), contains_final_state) - exists.begin());
    }
    int final_initial = 0;

    return {final_initial, final_states, final_trans};
}

void question1(){
    auto f_dfa = build_b_dfa();
    auto f_nfa = build_nfa(f_dfa);
    auto reverse_dfa = nfa_to_dfa(f_nfa);
    auto reverse_nfa = build_nfa(reverse_dfa);
    auto final = nfa_to_dfa(reverse_nfa);

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
}
//revisar el cambio de vector a int  en los ids

#endif //DFA_MINIMIZATION_BRZOZOWSKI_H
