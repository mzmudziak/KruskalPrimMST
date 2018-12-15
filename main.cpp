#include <iostream>
#include <vector>
#include "Graph.h"
#include "Utils.h"
#include <random>

using namespace std;


int main() {
    char mode;
    cout << "Choose mode:" << endl;
    cout << "A - single test for random graph" << endl;
    cout << "B - benchmark test for multiple randomly generated graphs" << endl;
    
    cin >> mode;
    switch (mode) {
        case 'a':
        case 'A': {
            int n, k;
            cout << "[A] - Single test" << endl;
            cout << "How many vertices?" << endl;
            cin >> n;
            int maxEdges = n * (n - 1) / 2;
            auto minEdges = static_cast<int>(ceil(0.25 * n * (n - 1)));
            uniform_int_distribution<int> uni(minEdges, maxEdges);
            cout << "How many edges? ("<< minEdges << ".." << maxEdges << ")" << endl;
            cin >> k;
            while (k < minEdges || k > maxEdges) {
                cin >> k;
            }
            auto *g = new Graph(n, k, true);
            
            // Execute PRIMs algorithm
            g->print();
            auto start = chrono::steady_clock::now();
            g->prim();
            auto end = chrono::steady_clock::now();
            auto diff = chrono::duration<double, nano>(end - start).count();
            cout << diff << " ";
            cout << "Execution: " << chrono::duration<double, nano>(diff).count() << " ns" << endl;
            
            // Execute KRUSKALs algorithm
            auto startK = chrono::steady_clock::now();
            g->kruskal();
            auto endK = chrono::steady_clock::now();
            auto diffK = chrono::duration<double, nano>(endK - startK).count();
            cout << diffK << " ";
            cout << "Execution: " << chrono::duration<double, nano>(diffK).count() << " ns" << endl;
            break;
        }
        case 'b':
        case 'B': {
            int min, max, amountOfExecutions, saturation;
            cout << "[B] - Benchmark!" << endl;
            cout << "How many vertices at minimum?" << endl;
            cin >> min;
            cout << "How many vertices at maximum?: " << endl;
            cin >> max;
            cout << "Graph saturation percentage: " << endl;
            cin >> saturation;
            cout << "How many executions per iteration? " << endl;
            cin >> amountOfExecutions;
            for (int i = min; i < max; i++) {
                int maxEdges =  i * (i - 1) / 2;
                int amountOfEdges = static_cast<int>(maxEdges * saturation / 100);
                cout << endl << "Run [" << i << "] - saturation: " << saturation << "% (" << amountOfEdges << " of " << maxEdges << " edges created)" << endl;
                double totalPrim = 0;
                double totalKruskal = 0;
                for (int j = 0; j < amountOfExecutions; j++) {
                    auto *graf = new Graph(i, amountOfEdges, false);
                    // Execute PRIMs algorithm
                    auto start = chrono::steady_clock::now();
                    graf->prim();
                    auto end = chrono::steady_clock::now();
                    auto diff = chrono::duration<double, nano>(end - start).count();
                    cout << diff << " ";
                    totalPrim += chrono::duration<double, nano>(diff).count();
                    
                    // Execute KRUSKALs algorithm
                    auto startK = chrono::steady_clock::now();
                    graf->kruskal();
                    auto endK = chrono::steady_clock::now();
                    auto diffK = chrono::duration<double, nano>(endK - startK).count();
                    cout << diffK << " ";
                    totalKruskal += chrono::duration<double, nano>(diffK).count();
                }
                cout << endl << "TOTAL:" << endl;
                cout << "Prim" << '\t' << "- time: " << totalPrim / amountOfExecutions << endl;
                cout << "Kruskal" << '\t' << "- time: " << totalKruskal / amountOfExecutions << endl;
            }
            break;
        }
        default:
            cout << "Wrong mode... exiting..." << endl;
    }
    return 0;
}
