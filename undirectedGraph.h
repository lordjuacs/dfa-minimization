#pragma once

#include <iostream>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <vector>
#include <set>
#include <queue>
#include <unordered_set>

struct node
{
    int key;
    std::vector<node*> edges;
};

class undirectedGraph
{
    private:
        std::vector<node*> adjacencyList;
        std::vector<int> v;
        int n_edges = 0;
        int contConnected = 0;

        bool searchEdge(int start_node_key, int end_node_key)
        {
            for(auto x: adjacencyList[start_node_key]->edges) 
            {
                if(x->key == end_node_key) 
                {
                    return true;
                }
            }
            return false;
        };

        int searchVertexPriv(int key)
        {
            for (int i = 0; i < adjacencyList.size(); ++i) 
            {
                if (adjacencyList[i]->key == key) 
                {
                    return i;
                }
            }
            return -1;
        };

        void setAllToNotVisited(bool *visited)
        {
            for (int vertex = 0; vertex < adjacencyList.size(); ++vertex)
            {
                visited[vertex] = false;
            }
        }

        void dfs(int vertex, bool *visited)
        {
            visited[vertex] = true;
            //std::cout << adjacencyList[vertex]->key << " ";
            v.push_back(adjacencyList[vertex]->key);
            
            for (auto it = adjacencyList[vertex]->edges.begin(); it != adjacencyList[vertex]->edges.end(); ++it)
            {
                int pos = searchVertexPriv((*it)->key);

                if (pos != -1)
                {
                  if (!visited[pos])
                  {
                      dfs(pos, visited);
                  }
                }
            }
        }

    public:
        undirectedGraph() {};
        undirectedGraph(int n)
        {
          for (int i = 0; i < n; ++i)
          {
            insertVertex(i);
          }
        };

        ~undirectedGraph() {};

        void insertVertex(int key) 
        {
            if (searchVertexPriv(key) == -1)
            {
              node* new_node = new node{key};
              adjacencyList.push_back(new_node);
            }
        }

        void insertEdge(int start_node_key, int end_node_key) 
        {
            int pos_start = searchVertexPriv(start_node_key);
            int pos_end = searchVertexPriv(end_node_key);

            if(!searchEdge(pos_start,end_node_key)) 
            {
                adjacencyList[pos_start]->edges.push_back(adjacencyList[pos_end]);
                adjacencyList[pos_end]->edges.push_back(adjacencyList[pos_start]);
                ++n_edges;
            }
        }

        void printGraph() 
        {
            for (const auto &it: adjacencyList)
            {
                std::cout<<"key:"<<it->key<<", edges ->";
                for (const auto &it2 : it->edges)
                {
                    std::cout<<" {key:"<<it2->key << "} ";
                }
                std::cout<<std::endl;
            }
            std::cout << std::endl;
        }

        int connectedComponents()
        {
            bool visited[adjacencyList.size()];

            setAllToNotVisited(visited);
        
            for (int vertex = 0; vertex < adjacencyList.size(); ++vertex) 
            {
                if (!visited[vertex]) 
                {
                    dfs(vertex, visited);

                    contConnected += v.size() - 1;
                    v.clear();
                }
            }

            return contConnected;
        }
};
