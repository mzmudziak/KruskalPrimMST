


#ifndef DJIKSTRA_GRAPH_H
#define DJIKSTRA_GRAPH_H

#include <iostream>
#include <queue>
#include <list>

using namespace std;

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
    void print();
    void prim();
    void kruskal();
    void kruskalPQ();
};
#endif
