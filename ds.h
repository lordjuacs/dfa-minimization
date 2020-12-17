#ifndef DFA_MINIMIZATION_DS_H
#define DFA_MINIMIZATION_DS_H

#include <map>
#include<vector>

template <typename T>
struct Node {
    int parent = -1;
    int rank = 0;
    T data;
};

template <typename T>
class DisjoinSet // interface
{
public:
    /*
    * x, y are indexes
    */
    DisjoinSet(){};
    DisjoinSet(std::vector<T> data){};
    virtual  ~DisjoinSet(){};
    //MakseSet the element with index x
    virtual void MakeSet(int x) = 0;
    //MakseSet all elements
    virtual void MakeSet() = 0;
    //Find the root of x
    virtual int Find(int x) = 0;
    //Union two sets represented by x and y (apply rank)
    virtual void Union(int x, int y) = 0;

    //check whether there is a path between x and y
    virtual bool isConnected(int x, int y) = 0;
    //total number of elements
    virtual int size() = 0;
    //number of sets
    virtual int sets() = 0;
    //total number of elements that belong to the set of x
    virtual int size(int x) = 0;
    //add the element to the set of x
    virtual void Add(T element, int x) = 0;
    //return all elements that belong to the set of x
};

template <typename T>
class DisjoinSetTree: public DisjoinSet<T> {
private:
    // define the structures
    Node<T>* nodes = nullptr;
    int size_{};

public:

    DisjoinSetTree(): DisjoinSet<T>(){}
    DisjoinSetTree(std::vector<T> data): DisjoinSet<T>(data){
        size_ = data.size();
        nodes = new Node<T>[size_];
        for(int i = 0; i < size_; ++i){
            nodes[i] = Node<T>{-1, 0, data[i]};
        }
    }
    ~DisjoinSetTree() {
        delete nodes;
        nodes = nullptr;
    };
    // implement all functions

    //MakseSet the element with index x
    void MakeSet(int x) override{
        if (x < 0 || x >= size_)
            throw "Invalid index";
        nodes[x].parent = x;
    }
    //MakseSet all elements
    void MakeSet() override{
        for (int i = 0; i < size_; ++i) {
            nodes[i].parent = i;
        }
    }
    //Find the root of x
    int Find(int x) override{
        if (x < 0 || x >= size_)
            throw "Invalid index";
        if (nodes[x].parent == x)
            return x;
        return Find(nodes[x].parent);
    }
    //Union two sets represented by x and y (apply rank)
    void Union(int x, int y) override{
        int x_root = Find(x);
        int y_root = Find(y);
        if (x_root == y_root)
            return;
        if (nodes[y_root].rank > nodes[x_root].rank)
            nodes[x_root].parent = y_root;
        else if (nodes[x_root].rank > nodes[y_root].rank)
            nodes[y_root].parent = x_root;
        else {
            nodes[y_root].parent = x_root;
            nodes[x_root].rank++;
        }
    }

    //check whether there is a path between x and y
    bool isConnected(int x, int y) override{
        return Find(x) == Find(y);
    }
    //total number of elements
    int size() override{
        return size_;
    }
    //number of sets
    int sets() override{
        int sets = 0;
        for(int i = 0; i < size_; ++i) {
            if(i == Find(i))
                sets++;
        }
        return sets;
    }
    //total number of elements that belong to the set of x
    int size(int x) override{
        int x_root = Find(x);
        int cont = 0;
        for(int i = 0; i < size_; ++i)
            if(Find(i) == x_root)
                cont++;
        return cont;
    }
    //add the element to the set of x
    void Add(T element, int x) override{
        Node<T>* temp = new Node<T>[size_ + 1];
        for(int i = 0; i < size_; ++i)
            temp[i] = nodes[i];
        temp[size_++] = Node<T>{x, 0, element};
        delete nodes;
        nodes = nullptr;
        nodes = temp;
    }

    std::unordered_map<int, std::vector<int>> getSets() {
        std::unordered_map<int, std::vector<int>> parents;
        for (int i = 0; i < size_; i++)
            if (Find(i) == i){
                std::vector<int> temp;
                parents[i] = temp;
            }

        std::vector<std::vector<int>> res;

        for (int i = 0; i < size_; i++){
            parents[Find(i)].push_back(i);
        }

        return parents;
    }

};

#endif //DFA_MINIMIZATION_DS_H
