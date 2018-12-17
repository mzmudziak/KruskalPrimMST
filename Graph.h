


#ifndef DJIKSTRA_GRAPH_H
#define DJIKSTRA_GRAPH_H

#include <iostream>
#include <queue>
#include <list>

using namespace std;
struct edge {
    int start;
    int end;
    int cost;
    bool operator<(const edge& rhs) const{
        return cost > rhs.cost;
    }
};

class Graph {
private:
    int n;
    bool verbose;
    int **matrix;
    void print(int parent[]);
    int findParent(int* parent, int i);
    void parentUnion(int* parent, int i, int j);
    int shortest(const int *distance, const bool *visited);
public:
    Graph(string filename, bool verbose);
    explicit Graph(int n, int k, bool verbose);
    priority_queue<edge, std::vector<edge>> getPriorityQueue();
    void print();
    void prim();
    void kruskal();
    void kruskalPQ(priority_queue<edge, std::vector<edge>> pq);
};
#endif
