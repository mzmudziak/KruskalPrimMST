//
// Created by Michal Zmudziak on 05/11/2018.
//

#include "Graph.h"
#include "Utils.h"

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
    cout << "Prim" << '\t' << "- cost:" << '\t' << "" << minWeight << "" << '\t' << "time: ";
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

int Graph::find(int* parent, int i){
    while (parent[i] != i) {
        i = parent[i];
    }
    return i;
}

void Graph::union1(int* parent, int i, int j)
{
    int a = find(parent, i);
    int b = find(parent, j);
    parent[a] = b;
}

void Graph::kruskal() {
    int parent[n];
    int minWeight = 0;
    for (int i = 0; i < n; i++) {
        parent[i] = i;
    }
    int edge_count = 0;
    while (edge_count < n - 1) {
        int min = INT_MAX;
        int a = -1;
        int b = -1;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (find(parent, i) != find(parent, j) && matrix[i][j] != 0 && matrix[i][j] < min) {
                    min = matrix[i][j];
                    a = i;
                    b = j;
                }
            }
        }
        
        union1(parent, a, b);
        if (verbose) {
            cout << a << " -> " << b << " (" << min << ")" << endl;
        }
        minWeight += min;
        edge_count++;
    }
    cout << endl << "Kruskal" << '\t' << "- cost:" << '\t' << "" << minWeight << "" << '\t' << "time: ";
}
