#pragma once

#include <ctime>
#include "undirectedGraph.h"
#include <unordered_map>
#include "ds.h"

struct hash_pair {
    template<class T1, class T2>
    size_t operator()(const std::pair<T1, T2> &p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

using eqMatrix = std::vector<std::vector<int>>;
using to_states = std::set<int>;
using transitions = std::unordered_map<int, std::pair<int, int>>;
using n_transitions = std::unordered_map<int, std::pair<to_states, to_states>>;
using nfa = std::tuple<int, int, n_transitions, std::set<int>>;
using dfa = std::tuple<int, std::unordered_set<int>, transitions>;
using statePair = std::pair<int, int>;
using dependencyMap = std::unordered_map<statePair, std::vector<statePair>, hash_pair>;

// {initial, final_states, trans}
class DFA {
private:
    int size;
    int initial;
    std::unordered_set<int> final_states;
    transitions trans;
    eqMatrix matrix;
    dependencyMap dependencies;

    // PREGUNTA 1
    void printFinalStates() {
        std::cout << "FINAL STATES:\n";
        for (auto it = final_states.begin(); it != final_states.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << "\n";
    }

    void printTransitions() {
        std::cout << "TRANSITIONS:\n";

        for (auto it = trans.begin(); it != trans.end(); ++it) {
            std::cout << (*it).first << " 0 " << (*it).second.first << std::endl;
            std::cout << (*it).first << " 1 " << (*it).second.second << std::endl;
        }
        std::cout << "\n";
    }

    void is_reachable(n_transitions &um, std::vector<bool> &reachable, int id) {
        if (reachable[id])
            return;
        reachable[id] = true;
        for (const auto &e : um[id].first)
            is_reachable(um, reachable, e);
        for (const auto &e : um[id].second)
            is_reachable(um, reachable, e);
    }

    nfa build_nfa(dfa &original) {
        n_transitions n_trans;
        std::set<int> empty;
        for (int i = 0; i < std::get<2>(original).size(); ++i)
            n_trans.insert(std::make_pair(i, std::make_pair(empty, empty)));
        for (auto &it : std::get<2>(original)) {
            n_trans[it.second.first].first.insert(it.first);
            n_trans[it.second.second].second.insert(it.first);
        }
        int initial;
        int final_state = std::get<0>(original);
        std::set<int> e_trans;
        std::vector<bool> reachable(n_trans.size());
        if (std::get<1>(original).size() == 1) {
            initial = *std::get<1>(original).begin();
            is_reachable(n_trans, reachable, initial);
        } else {
            initial = -1;
            for (auto &e : std::get<1>(original)) {
                e_trans.insert(e);
                is_reachable(n_trans, reachable, e);
            }
        }
        for (int i = 0; i < reachable.size(); i++)
            if (!reachable[i]) {
                n_trans.erase(i);
            }
        return {initial, final_state, n_trans, e_trans};
    }

    dfa nfa_to_dfa(nfa &original) {
        std::set<int> init;
        std::set<int> closure;
        std::vector<std::set<int>> exists;
        std::queue<std::set<int>> checking;
        n_transitions trans;
        if (std::get<0>(original) != -1)
            init.insert(std::get<0>(original));
        else if (std::get<3>(original).empty())
            init.insert(std::get<0>(original));

        for (const auto &e : std::get<3>(original))
            init.insert(e);
        //std::sort(init.begin(), init.end());
        exists.push_back(init);
        std::set<int> which;
        transitions f_trans;
        int cont = 0;

        for (int i = 0; i < 2; ++i) {
            for (const int &j : init) {
                if (j != -1) {
                    which = i ? std::get<2>(original).at(j).second : std::get<2>(original).at(j).first;
                    for (const int &k : which)
                        if (std::find(closure.begin(), closure.end(), k) == closure.end())
                            closure.insert(k);
                }
            }
            //std::sort(closure.begin(), closure.end());
            i ? trans[cont].second = closure : trans[cont].first = closure;
            if (std::find(exists.begin(), exists.end(), closure) == exists.end()) {
                checking.push(closure);
            }
            closure.clear();
        }
        std::set<std::set<int>> contains_f_states;

        if (init.find(std::get<1>(original)) != init.end())
            contains_f_states.insert(init);

        std::set<int> front;
        std::unordered_set<int> f_states;
        while (!checking.empty()) {
            front = checking.front();
            checking.pop();
            if (std::find(exists.begin(), exists.end(), front) == exists.end()) {
                cont++;
                for (int i = 0; i < 2; ++i) {
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
                if (std::find(front.begin(), front.end(), std::get<1>(original)) != front.end())
                    contains_f_states.insert(front);
            }
        }
        int pos;
        for (auto &tran : trans) {
            pos = std::find(exists.begin(), exists.end(), tran.second.first) - exists.begin();
            f_trans[tran.first].first = pos;
            pos = std::find(exists.begin(), exists.end(), tran.second.second) - exists.begin();
            f_trans[tran.first].second = pos;
        }
        for (auto &contains_final_state : contains_f_states) {
            f_states.insert(std::find(exists.begin(), exists.end(), contains_final_state) - exists.begin());
        }

        return {0, f_states, f_trans};
    }

    // PREGUNTA 2
    void markFinalStates(eqMatrix &matrix) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < i; j++) {
                if (final_states.find(i) == final_states.end() ^ final_states.find(j) == final_states.end())
                    matrix[i][j] = 0;
            }
        }
    }

    void optimizedMarkFinalStates(eqMatrix &matrix) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < i; j++) {
                if (final_states.find(i) == final_states.end() ^ final_states.find(j) == final_states.end()) {
                    for (auto &dependency : dependencies[std::make_pair(i, j)])
                        matrix[dependency.first][dependency.second] = 0;
                    matrix[i][j] = 0;
                }
            }
        }
    }

    void printMatrix(eqMatrix &matrix, undirectedGraph &ugraph) {
        for (int i = 0; i < matrix[0].size(); i++) {
            for (int j = 0; j < i; j++) {
                if (matrix[i][j]) {
                    ugraph.insertVertex(i);
                    ugraph.insertVertex(j);
                    ugraph.insertEdge(i, j);
                }

                std::cout << matrix[i][j] << " ";
            }
            std::cout << "\n";
        }
    }

    eqMatrix equivalencyMatrix() {
        eqMatrix matrix(size, std::vector<int>(size, 1));
        //std::vector<bool> reachable(size, false);
        //reachableDFA(trans, reachable, initial);
        // for (int i = 0; i < dfa.size; i++){
        //     matrix[i][i] = 0;
        // }
        markFinalStates(matrix);
        bool modified;

        do {
            modified = false;
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < i; j++) {
                    if (matrix[i][j]) {
                        if (trans[i].first >= trans[j].first) {
                            if (!matrix[trans[i].first][trans[j].first]) {
                                matrix[i][j] = 0;
                                modified = true;
                            }
                        } else {
                            if (!matrix[trans[j].first][trans[i].first]) {
                                matrix[i][j] = 0;
                                modified = true;
                            }
                        }
                        if (trans[i].second != trans[j].second) {
                            if (trans[i].second >= trans[j].second) {
                                if (!matrix[trans[i].second][trans[j].second]) {
                                    matrix[i][j] = 0;
                                    modified = true;
                                }
                            } else {
                                if (!matrix[trans[j].second][trans[i].second]) {
                                    matrix[i][j] = 0;
                                    modified = true;
                                }
                            }

                        }
                    }
                }
            }
        } while (modified);

        return matrix;
    }

    std::pair<statePair, statePair> getResultingStates(int row, int col) {
        return make_pair(std::make_pair(trans[row].first, trans[col].first),
                         std::make_pair(trans[row].second, trans[col].second));
    }

    eqMatrix optimizedEquivalencyMatrix() {
        this->matrix = eqMatrix(size, std::vector<int>(size, 1));

        for (int row = 0; row < size; row++)
            for (int col = 0; col < size; col++) {
                std::vector<statePair> temp;
                auto curr = std::make_pair(row, col);
                dependencies[curr] = temp;
                auto transitions = getResultingStates(row, col);
                if (dependencies.find(transitions.first) != dependencies.end())
                    dependencies[transitions.first].push_back(curr);
                if (dependencies.find(transitions.second) != dependencies.end())
                    dependencies[transitions.second].push_back(curr);
            }

        optimizedMarkFinalStates(matrix);

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < i; j++) {
                if (matrix[i][j]) {
                    if (trans[i].first >= trans[j].first) {
                        if (!matrix[trans[i].first][trans[j].first])
                            matrix[i][j] = 0;
                    } else {
                        if (!matrix[trans[j].first][trans[i].first])
                            matrix[i][j] = 0;
                    }
                    if (trans[i].second != trans[j].second) {
                        if (trans[i].second >= trans[j].second) {
                            if (!matrix[trans[i].second][trans[j].second])
                                matrix[i][j] = 0;
                        } else {
                            if (!matrix[trans[j].second][trans[i].second])
                                matrix[i][j] = 0;
                        }

                    }
                }
            }
        }

        return matrix;
    }


    // Tester
    void is_reachable_dfa(std::unordered_set<int> &reachable, int id) {
        //std::cout << "ID: " << id << "\n";
        if (reachable.find(id) != reachable.end())
            return;
        reachable.insert(id);
        is_reachable_dfa(reachable, trans[id].first);
        is_reachable_dfa(reachable, trans[id].second);
    }

public:
    DFA() {
        int n_final;

        std::cin >> size;
        std::cin >> initial;
        std::cin >> n_final;

        for (int i = 0; i < n_final; ++i) {
            int temp;
            std::cin >> temp;
            final_states.insert(temp);
        }

        int from, with, to;
        for (int i = 0; i < 2 * size; ++i) {
            std::cin >> from;
            std::cin >> with;
            std::cin >> to;
            with ? trans[from].second = to : trans[from].first = to;
        }

        //printFinalStates();
        //printTransitions();
    }

    DFA(int size_) {
        std::cout << "\nInput\n";
        std::cout << "..................\n";

        size = size_;
        int n_final, from, to;
        bool with, all_can_be_reached;

        while (!all_can_be_reached) {
            //std::cout << "Size 0: " << size << std::endl;
            std::vector<int> possible_states;
            std::unordered_set<int> reachable;
            trans.clear();
            final_states.clear();

            //std::cout << "Size 1: " << size << std::endl;
            for (int i = 0; i < size; ++i) {
                possible_states.push_back(i);
            }

            //std::cout << "Size 2: " << size << std::endl;
            std::cout << size << " ";
            initial = rand() % size;
            std::cout << initial << " ";
            n_final = (rand() % size) / 2 + 1;
            std::cout << n_final << " ";

            for (int i = 0; i < n_final; ++i) {
                int temp = rand() % (possible_states.size());
                int x = possible_states[temp];
                possible_states.erase(possible_states.begin() + temp);
                std::cout << x << " ";
                final_states.insert(x);
            }

            std::cout << std::endl;

            with = false;
            for (int i = 0; i < 2 * size; ++i) {
                //std::cout << "i: " << i << "\n";
                from = i / 2;
                std::cout << from << " ";
                std::cout << with << " ";
                to = rand() % (size);
                std::cout << to << "\n";
                with ? trans[from].second = to : trans[from].first = to;
                //trans.insert({from, std::make_pair(with, to)});
                //std::cout << ""
                with = !with;
            }

            is_reachable_dfa(reachable, initial);
            all_can_be_reached = (reachable.size() == size);
            if (!all_can_be_reached)
                std::cout << "Automata que tiene por lo menos un estado\naislado al cual no se puede llegar\n\n";

        }

        //printFinalStates();
        //printTransitions();
    }

    ~DFA() {}

    int getSize() {
        return size;
    }

    dfa huffman_moore(eqMatrix m, std::vector<int> to_remove) {
        std::vector<int> v;
        for (const auto &key : trans)
            v.push_back(key.first);

        DisjoinSetTree<int> ds(v);
        ds.MakeSet();

        for (int row = 1; row < size; row++)
            for (int col = 0; col < row; col++)
                if (m[row][col]) {
                    if (ds.Find(row) != ds.Find(col)) {
                        ds.Union(row, col);
                    }
                }
        auto sets = ds.getSets();
        transitions new_trans;
        std::unordered_map<int, int> numeration;
        int cont = 0;
        for (const auto &key: sets)
            numeration[key.first] = cont++;

        for (const auto &key: sets) {
            new_trans[numeration[key.first]] = std::pair<int, int>();
            new_trans[numeration[key.first]].first = numeration[ds.Find(trans[key.first].first)];
            new_trans[numeration[key.first]].second = numeration[ds.Find(trans[key.first].second)];

        }
        int begin = numeration[ds.Find(initial)];
        std::unordered_set<int> final;
        for (const auto &k : final_states) {
            final.insert(numeration[ds.Find(k)]);
        }
        return {begin, final, new_trans};
    }

    std::pair<int, double> question1() {
        clock_t start, end;
        start = clock();

        dfa d{initial, final_states, trans};
        nfa f_nfa = build_nfa(d);
        dfa reverse_dfa = nfa_to_dfa(f_nfa);
        nfa reverse_nfa = build_nfa(reverse_dfa);
        dfa final = nfa_to_dfa(reverse_nfa);

        end = clock();
        auto time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        std::cout << "\nTiempo de demora 1: " << time_taken << "s\n";


        std::cout << "\nOutput: Brzozowski\n";
        std::cout << "..................\n";
        std::cout << std::get<2>(final).size() << " " << std::get<0>(final) << " " << std::get<1>(final).size() << " ";
        for (auto &e : std::get<1>(final))
            std::cout << e << " ";
        std::cout << std::endl;
        for (auto &e : std::get<2>(final)) {
            std::cout << e.first << " ";
            std::cout << "0 ";
            std::cout << e.second.first << std::endl;
            std::cout << e.first << " ";
            std::cout << "1 ";
            std::cout << e.second.second << std::endl;
        }
        return {std::get<2>(final).size(), time_taken};
    }

    std::pair<int, double> question2() {
        clock_t start, end;
        start = clock();
        auto matrix = equivalencyMatrix();
        end = clock();
        auto time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        std::cout << "\nTiempo de demora 2: " << time_taken << "s\n";
        std::cout << "\nOutput: Equivalencia de estados\n";
        std::cout << "..................";
        undirectedGraph ugraph;
        printMatrix(matrix, ugraph);
        ugraph.printGraph();
        size = size - ugraph.connectedComponents();
        return {size, time_taken};

    }

    std::pair<int, double> question3() {
        clock_t start, end;
        start = clock();
        auto matrix = optimizedEquivalencyMatrix();
        end = clock();
        auto time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        std::cout << "\nTiempo de demora 2: " << time_taken << "s\n";
        std::cout << "\nOutput: Equivalencia de estados\n";
        std::cout << "..................";
        undirectedGraph ugraph;
        printMatrix(matrix, ugraph);
        ugraph.printGraph();
        size = size - ugraph.connectedComponents();
        return {size, time_taken};
    }

    std::pair<int, double> question4() {
        clock_t start, end;
        start = clock();
        std::unordered_set<int> reachable;
        is_reachable_dfa(reachable, initial);
        std::vector<int> to_remove;
        for (auto &i : trans) {
            if (reachable.find(i.first) == reachable.end()) {
                to_remove.push_back(i.first);
            }
        }/*
        for (const auto &i : to_remove) {
            trans.erase(i);
            size--;
            for(auto)
        }*/

        auto m = optimizedEquivalencyMatrix();
        auto huffmanMoore = huffman_moore(m, to_remove);
        end = clock();
        auto time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        std::cout << "\nTiempo de demora 4: " << time_taken << "s\n";
        std::cout << "..................";
        std::cout << "\nOutput: Huffman-Moore\n";
        std::cout << "..................\n";
        std::cout << std::get<2>(huffmanMoore).size() << " " << std::get<0>(huffmanMoore) << " " << std::get<1>(huffmanMoore).size() << " ";
        for (auto &e : std::get<1>(huffmanMoore))
            std::cout << e << " ";
        std::cout << std::endl;
        for (auto &e : std::get<2>(huffmanMoore)) {
            std::cout << e.first << " ";
            std::cout << "0 ";
            std::cout << e.second.first << std::endl;
            std::cout << e.first << " ";
            std::cout << "1 ";
            std::cout << e.second.second << std::endl;
        }
        return {std::get<1>(huffmanMoore).size(), time_taken};
    }
};
