#ifndef DFA_MINIMIZATION_DFA_H
#define DFA_MINIMIZATION_DFA_H

#include <iostream>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <vector>
#include <set>
#include <queue>
#include <unordered_set>

using eqMatrix = std::vector<std::vector<int>>;
using to_states = std::set<int>;
using transitions = std::unordered_map<int, std::pair<int, int>>;
using n_transitions = std::unordered_map<int, std::pair<to_states, to_states>>;
using nfa = std::tuple<int, int, n_transitions, std::set<int>>;
using dfa = std::tuple<int, std::unordered_set<int>, transitions>;
// {initial, final_states, trans}
class DFA{
  private:
    int size;
    int initial;
    std::unordered_set<int> final_states;
    transitions trans;
    eqMatrix matrix;

    // PREGUNTA 1
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
        std::set<int> empty;
        for(int i = 0; i < std::get<2>(original).size(); ++i)
            n_trans.insert(std::make_pair(i, std::make_pair(empty, empty)));
        for(auto & it : std::get<2>(original)){
            n_trans[it.second.first].first.insert(it.first);
            n_trans[it.second.second].second.insert(it.first);
        }
        int initial;
        int final_state = std::get<0>(original);
        std::set<int> e_trans;
        std::vector<bool> reachable(n_trans.size());
        if(std::get<1>(original).size() == 1) {
            initial = *std::get<1>(original).begin();
            is_reachable(n_trans, reachable, initial);
        }
        else{
            initial = -1;
            for(auto &e : std::get<1>(original)) {
                e_trans.insert(e);
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
        std::set<int> init;
        std::set<int> closure;
        std::vector<std::set<int>> exists;
        std::queue<std::set<int>> checking;
        n_transitions trans;
        init.insert(std::get<0>(original));
        for(const auto& e : std::get<3>(original))
            init.insert(e);
        //std::sort(init.begin(), init.end());
        exists.push_back(init);
        std::set<int> which;
        transitions f_trans;
        int cont = 0;

        for(int i = 0; i < 2; ++i) {
            for (const int &j : init) {
                if(j != - 1) {
                    which = i ? std::get<2>(original).at(j).second : std::get<2>(original).at(j).first;
                    for (const int &k : which)
                        if (std::find(closure.begin(), closure.end(), k) == closure.end())
                            closure.insert(k);
                }
            }
            //std::sort(closure.begin(), closure.end());
            i ? trans[cont].second = closure : trans[cont].first = closure;
            if(std::find(exists.begin(), exists.end(), closure) == exists.end()) {
                checking.push(closure);
            }
            closure.clear();
        }
        std::set<std::set<int>> contains_f_states;

        if(init.find(std::get<1>(original)) != init.end())
            contains_f_states.insert(init);

        std::set<int> front;
        std::unordered_set<int> f_states;
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
                                    closure.insert(k);
                        }
                    }
                    //std::sort(closure.begin(), closure.end());
                    i ? trans[cont].second = closure : trans[cont].first = closure;
                    checking.push(closure);
                    closure.clear();
                }
                exists.push_back(front);
                if(std::find(front.begin(), front.end(), std::get<1>(original)) != front.end())
                    contains_f_states.insert(front);
            }
        }
        int pos;
        for(auto & tran : trans){
            pos = std::find(exists.begin(), exists.end(), tran.second.first) - exists.begin();
            f_trans[tran.first].first = pos;
            pos = std::find(exists.begin(), exists.end(), tran.second.second) - exists.begin();
            f_trans[tran.first].second = pos;
        }
        for (auto & contains_final_state : contains_f_states){
            f_states.insert(std::find(exists.begin(), exists.end(), contains_final_state) - exists.begin());
        }

        return {0, f_states, f_trans};
    }

    // PREGUNTA 2
    void markFinalStates(eqMatrix &matrix){
        for (int i = 0; i < size; i++){
            for (int j = 0; j < i; j++){
                if (final_states.find(i)==final_states.end() ^ final_states.find(j)==final_states.end())
                    matrix[i][j] = 0;
            }
        }
    }

    void reachableDFA(transitions &trans, std::vector<bool> &reachable, int id){
        if (reachable[id])
            return;
        reachable[id] = true;
        reachableDFA(trans,	reachable, trans[id].first);
        reachableDFA(trans, reachable, trans[id].second);
    }

    void printMatrix(eqMatrix &matrix){
        for (int i = 0; i < matrix[0].size(); i++){
            for (int j = 0; j < i; j++){
                std::cout << matrix[i][j] << " ";
            }
            std::cout << "\n";
        }
    }

    eqMatrix equivalencyMatrix(){
        eqMatrix matrix(size, std::vector<int>(size, 1));
        std::vector<bool> reachable(size, false);
        reachableDFA(trans, reachable, initial);
        // for (int i = 0; i < dfa.size; i++){
        //     matrix[i][i] = 0;
        // }
        markFinalStates(matrix);
        bool modified;
        do{
            modified = false;
            // O(n^2/2 - n) => O(n^2) creo
            for (int i = 0; i < size; i++){
                for (int j = 0; j < i; j++){
                    if (matrix[i][j]){
                        if (trans[i].first == trans[j].first){
                            if (trans[i].second != trans[j].second){
                                if (trans[i].second >= trans[j].second){
                                    if (!matrix[trans[i].second][trans[j].second]){
                                        matrix[i][j] = 0;
                                        modified = true;
                                    }
                                }
                                else{
                                    if (!matrix[trans[j].second][trans[i].second]){
                                        matrix[i][j] = 0;
                                        modified = true;
                                    }
                                }

                            }
                        }
                        else{
                            if (trans[i].first >= trans[j].first){
                                if (!matrix[trans[i].first][trans[j].first]){
                                    matrix[i][j] = 0;
                                    modified = true;
                                }
                            }
                            else{
                                if (!matrix[trans[j].first][trans[i].first]){
                                    matrix[i][j] = 0;
                                    modified = true;
                                }
                            }
                        }
                    }
                }
            }
        }while (modified);

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < i; j++) {
                if (matrix[i][j]){
                    if (trans[i].first >= trans[j].first){
                        if (!matrix[trans[i].first][trans[j].first]){
                            matrix[i][j] = 0;
                            modified = true;
                        }
                    }
                    else{
                        if (!matrix[trans[j].first][trans[i].first]){
                            matrix[i][j] = 0;
                            modified = true;
                        }
                    }
                    if (trans[i].second != trans[j].second){
                        if (trans[i].second >= trans[j].second){
                            if (!matrix[trans[i].second][trans[j].second]){
                                matrix[i][j] = 0;
                            }
                        }
                        else{
                            if (!matrix[trans[j].second][trans[i].second]){
                                matrix[i][j] = 0;
                            }
                        }

                    }
                }
            }
        }


        return matrix;
    }    

  public:
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

    DFA(int size_){
        std::cout << "\nInput\n";
        std::cout << "..................\n";        

        size = size_;
        std::cout << size << " ";
        initial = rand()%size;
        std::cout << initial << " ";
        int n_final = (rand()%size)/2 + 1;
        std::cout << n_final << " ";
        
        for(int i = 0; i < n_final; ++i){
            int temp;
            temp = rand()%(size);
            std::cout << temp << " ";
            final_states.insert(temp);
        }
        
        std::cout << std::endl;

        int from, to;
        bool with = false;
        for(int i = 0; i < 2*size; ++i){
            //std::cout << "i: " << i << "\n";
            from = i/2;
            std::cout << from << " ";
            std::cout << with << " ";
            to = rand()%(size);
            std::cout << to << "\n";         
            with = !with;
            with? trans[from].second = to : trans[from].first = to;
        }
    }

    ~DFA() {}
    
    int getSize()
    {
      return size;
    }

    void question1(){
        dfa d{initial, final_states, trans};
        nfa f_nfa = build_nfa(d);
        dfa reverse_dfa = nfa_to_dfa(f_nfa);
        nfa reverse_nfa = build_nfa(reverse_dfa);
        dfa final = nfa_to_dfa(reverse_nfa);

        std::cout << "\nOutput: Brzozowski\n";
        std::cout << "..................\n";
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

    void question2(){
        auto matrix = equivalencyMatrix();
        std::cout << "\nOutput: Equivalencia de estados\n";
        std::cout << "..................";
        printMatrix(matrix);
    }
};



#endif //DFA_MINIMIZATION_DFA_H
