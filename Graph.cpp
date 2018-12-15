//
// Created by Michal Zmudziak on 05/11/2018.
//

#include "Graph.h"
#include <vector>
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <functional>

#include <queue>

using namespace std;

Graph::Graph(string filename, bool verbose){
    this->verbose = verbose;
    ifstream stream(filename);
    string line;
    std::getline(stream, line);
    this->n = std::stoi(line);
    this->matrix = new int *[n];
    for (int i = 0; i < n; i++) {
        this->matrix[i] = new int[n];
        for (int j = 0; j < n; j++) {
            this->matrix[i][j] = 0;
        }
    }

    while(std::getline(stream, line)) {
        std::istringstream s(line);
        std::string field;
        string start, end, cost;
        getline(s, start, ';') ;
        getline(s, end, ';') ;
        getline(s, cost);
        cout << start << "\t<->\t" << end << "\t=\t" << cost << endl;
        this->matrix[stoi(start)][stoi(end)] = stoi(cost);
        this->matrix[stoi(end)][stoi(start)] = stoi(cost);
    }
}

Graph::Graph(int vertices, int edges, bool verbose) {
    this->verbose = verbose;
    this->n = vertices;
    // generate 0-filled adjacency matrix
    this->matrix = new int *[vertices];
    for (int i = 0; i < vertices; i++) {
        this->matrix[i] = new int[vertices];
        for (int j = 0; j < vertices; j++) {
            this->matrix[i][j] = 0;
        }
    }
    // create required n-1 edges
    vector<int> randomIndexedVector = fisherYatesShuffle(indexedVector(vertices));
    for (int k = 0; k < vertices - 1; k++) {
        int index = randomIndexedVector[k];
        int nextIndex = randomIndexedVector[k + 1];
        int randomNumber = random(0, 100);
        this->matrix[index][nextIndex] = randomNumber;
        this->matrix[nextIndex][index] = randomNumber;
    }
    // find all possible edge (i,j) pairs
    vector<std::pair<int, int>> free;
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < i; ++j) {
            if (this->matrix[i][j] == 0) {
                free.emplace_back(i, j);
            }
        }
    }
    // shuffle the pairs so they are random
    vector<std::pair<int, int>> shuffledFree = fisherYatesShuffle(free);
    
    // create (edges - n + 1) edges
    for (int i = 0; i < edges - n + 1; i++) {
        int randomNumber = random(0, 100);
        this->matrix[shuffledFree[i].first][shuffledFree[i].second] = randomNumber;
        this->matrix[shuffledFree[i].second][shuffledFree[i].first] = randomNumber;
    }
}

int Graph::shortest(const int *distance, const bool *visited) {
    int min = INT_MAX;
    int min_index = -1;
    for (int i = 0; i < n; i++) {
        if (!visited[i] && distance[i] <= min) {
            min = distance[i];
            min_index = i;
        }
    }
    return min_index;
}

void Graph::print() {
    cout << "Graph with " << n << " vertices" << endl << '\t';
    for (int i = 0; i < n; ++i) {
        cout << i << "\t";
    }
    cout << endl;
    for (int i = 0; i < n; i++) {
        cout << i << "\t";
        for (int j = 0; j < n; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
}

void Graph::print(int parent[]){
    int minWeight = 0;
    for (int i = 1; i < n; i++) {
        int currMinWeight = matrix[i][parent[i]];
        if (verbose) {
            cout << parent[i] << " -> " << i << " (" << currMinWeight << ")" << endl;
        }
        minWeight += currMinWeight;
    }
    cout << endl << "Prim" << '\t' << "-" << '\t' << "cost:" << '\t' << "" << minWeight << "" << '\t' << "time: ";
}

void Graph::prim() {
    int parent[n];
    int key[n];
    bool mstSet[n];
    for (int i = 0; i < n; i++) {
        key[i] = INT_MAX;
        mstSet[i] = false;
    }
    key[0] = 0;
    parent[0] = -1;
    
    for (int count = 0; count < n-1; count++)        {
        int u = shortest(key, mstSet);
        mstSet[u] = true;
        for (int i = 0; i < n; i++)
            if (matrix[u][i] && mstSet[i] == false && matrix[u][i] < key[i]) {
                parent[i] = u;
                key[i] = matrix[u][i];
            }
    }
    print(parent);
}

int Graph::findParent(int* parent, int i) {
    if (parent[i] == i) {
        return i;
    }
    parent[i] = findParent(parent, parent[i]);
    return parent[i];
}


void Graph::parentUnion(int* parents, int i, int j){
    int a = findParent(parents, i);
    int b = findParent(parents, j);
    parents[a] = b;
}

void Graph::kruskal() {
    int parents[n];
    int mstWeight = 0;
    for (int i = 0; i < n; i++) {
        parents[i] = i;
    }
    int edgesVisited = 0;
    while (edgesVisited < n - 1) {
        int currentMinEdge = INT_MAX;
        int startX = -1;
        int startY = -1;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (findParent(parents, i) != findParent(parents, j) && matrix[i][j] != 0 && matrix[i][j] < currentMinEdge) {
                    currentMinEdge = matrix[i][j];
                    startX = i;
                    startY = j;
                }
            }
        }
        
        parentUnion(parents, startX, startY);
        if (verbose) {
            cout << startX << " -> " << startY << " (" << currentMinEdge << ")" << endl;
        }
        mstWeight += currentMinEdge;
        edgesVisited++;
    }
    cout << endl << "Kruskal" << '\t' << "-" << '\t' << "cost:" << '\t' << "" << mstWeight << "" << '\t' << "time: ";
}

struct edge {
    int start;
    int end;
    int cost;
};

void Graph::kruskalPQ() {
    auto cmp = [](edge left, edge right) { return (left.cost > right.cost);};
    std::priority_queue<edge, std::vector<edge>, decltype(cmp)> edges(cmp);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (matrix[i][j] != 0) {
                edge e;
                e.start = i;
                e.end = j;
                e.cost = matrix[i][j];
                edges.push(e);
            }
        }
    }
    
    int parents[n];
    int mstWeight = 0;
    
    for (int i = 0; i < n; i++) {
        parents[i] = i;
    }
    
    while(!edges.empty()) {
        edge e = edges.top();
        edges.pop();

        if (findParent(parents, e.start) != findParent(parents, e.end)) {
            if (verbose) {
                cout << e.start << " -> " << e.end << " (" << e.cost << ")" << endl;
            }
            parents[e.start] = findParent(parents, e.end);
            mstWeight += e.cost;
        }
    }
    
    cout << endl << "Krus PQ" << '\t' << "-" << '\t' << "cost:" << '\t' << "" << mstWeight << "" << '\t' << "time: ";
}
