#include "dfa.h"

// TODO: cambiar AFD a clase (más importante) y AFN (no es necesario)
// TODO: crear una clase tester

using eqMatrix = std::vector<std::vector<int>>;

void markFinalStates(eqMatrix &matrix, DFA &dfa){
    for (int i = 0; i < dfa.size; i++){
        for (int j = 0; j < i; j++){
            if (dfa.final_states.find(i)==dfa.final_states.end() ^ dfa.final_states.find(j)==dfa.final_states.end())
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


eqMatrix equivalencyMatrix(DFA dfa){
    eqMatrix matrix(dfa.size, std::vector<int>(dfa.size, 1));
    std::vector<bool> reachable(dfa.size, false);
    reachableDFA(dfa.trans, reachable, dfa.initial);
    // for (int i = 0; i < dfa.size; i++){
    //     matrix[i][i] = 0;
    // }
    markFinalStates(matrix, dfa);
    bool modified;
    do{
        modified = false;
        // O(n^2/2 - n) => O(n^2) creo
        for (int i = 0; i < dfa.size; i++){
            for (int j = 0; j < i; j++){
                if (matrix[i][j]){
                    if (dfa.trans[i].first == dfa.trans[j].first){
                        if (dfa.trans[i].second != dfa.trans[j].second){
                            if (dfa.trans[i].second >= dfa.trans[j].second){
                                if (!matrix[dfa.trans[i].second][dfa.trans[j].second]){
                                    matrix[i][j] = 0;
                                    modified = true;
                                }
                            }
                            else{
                                if (!matrix[dfa.trans[j].second][dfa.trans[i].second]){
                                    matrix[i][j] = 0;
                                    modified = true;
                                }
                            }

                        }
                    }
                    else{
                        if (dfa.trans[i].first >= dfa.trans[j].first){
                            if (!matrix[dfa.trans[i].first][dfa.trans[j].first]){
                                matrix[i][j] = 0;
                                modified = true;
                            }
                        }
                        else{
                            if (!matrix[dfa.trans[j].first][dfa.trans[i].first]){
                                matrix[i][j] = 0;
                                modified = true;
                            }
                        }
                    }
                }
            }
        }
    }while (modified);

    for (int i = 0; i < dfa.size; i++) {
        for (int j = 0; j < i; j++) {
            if (matrix[i][j]){
                if (dfa.trans[i].first >= dfa.trans[j].first){
                    if (!matrix[dfa.trans[i].first][dfa.trans[j].first]){
                        matrix[i][j] = 0;
                        modified = true;
                    }
                }
                else{
                    if (!matrix[dfa.trans[j].first][dfa.trans[i].first]){
                        matrix[i][j] = 0;
                        modified = true;
                    }
                }
                if (dfa.trans[i].second != dfa.trans[j].second){
                    if (dfa.trans[i].second >= dfa.trans[j].second){
                        if (!matrix[dfa.trans[i].second][dfa.trans[j].second]){
                            matrix[i][j] = 0;
                        }
                    }
                    else{
                        if (!matrix[dfa.trans[j].second][dfa.trans[i].second]){
                            matrix[i][j] = 0;
                        }
                    }

                }
            }
        }
    }


    return matrix;
}


// void markEquivalentStates(eqMatrix &matrix, DFA &dfa){
//     for (int i = 0; i < dfa.size; i++){
//         for (int j = 0; j < i; j++){
//             if (matrix[i][j]){
//                 matrix[i][j] = 1;
//             }
//         }
//     }
// }

void question2(){
    auto dfa = DFA();
    auto matrix = equivalencyMatrix(dfa);
    printMatrix(matrix);
}