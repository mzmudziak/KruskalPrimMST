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
            cout << "[A] - Test from file" << endl;
            string filename = "/Users/michal/git/umcs/algorithms/prim/prim/prim/PrimKruska_data.txt";
            cout << "Reading from file: " << filename << endl;
            auto *graph = new Graph(filename, true);
            graph->print();
            auto start = chrono::steady_clock::now();
            graph->prim();
            auto end = chrono::steady_clock::now();
            auto diff = chrono::duration<double, nano>(end - start).count();
            cout << diff << " ";
            cout << "Execution: " << chrono::duration<double, nano>(diff).count() << " ns" << endl;
            
            // Execute KRUSKALs algorithm
            auto startK = chrono::steady_clock::now();
            graph->kruskal();
            auto endK = chrono::steady_clock::now();
            auto diffK = chrono::duration<double, nano>(endK - startK).count();
            cout << diffK << " ";
            cout << "Execution: " << chrono::duration<double, nano>(diffK).count() << " ns" << endl;
            
            // Execute KRUSKALs algorithm using Priority Queue
            auto startK_PQ = chrono::steady_clock::now();
            graph->kruskalPQ();
            auto endK_PQ = chrono::steady_clock::now();
            auto diffK_PQ = chrono::duration<double, nano>(endK_PQ - startK_PQ).count();
            cout << diffK_PQ << " ";
            cout << "Execution: " << chrono::duration<double, nano>(diffK_PQ).count() << " ns" << endl;
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
                auto minEdges = static_cast<int>(ceil(0.25 * i * (i - 1)));
                int maxEdges =  i * (i - 1) / 2;
                int amountOfEdges = static_cast<int>(maxEdges * saturation / 100);
                if (amountOfEdges < minEdges) {
                    cout << "Sorry, given saturation does not comply with dense graph rules. Applying min: " << minEdges << " edges " << endl;
                    amountOfEdges = minEdges;
                }
                cout << endl << "Run [" << i << "] - saturation: " << saturation << "% (" << amountOfEdges << " of " << maxEdges << " edges created)" << endl;
                double totalPrim = 0;
                double totalKruskal = 0;
                double totalKruskal_PQ = 0;
                for (int j = 0; j < amountOfExecutions; j++) {
                    auto *graph = new Graph(i, amountOfEdges, false);
                    // graf->print();

                    // Execute PRIMs algorithm
                    auto start = chrono::steady_clock::now();
                    graph->prim();
                    auto end = chrono::steady_clock::now();
                    auto diff = chrono::duration<double, nano>(end - start).count();
                    cout << diff << " ";
                    totalPrim += chrono::duration<double, nano>(diff).count();
                    
                    // Execute KRUSKALs algorithm
                    auto startK = chrono::steady_clock::now();
                    graph->kruskal();
                    auto endK = chrono::steady_clock::now();
                    auto diffK = chrono::duration<double, nano>(endK - startK).count();
                    cout << diffK << " ";
                    totalKruskal += chrono::duration<double, nano>(diffK).count();
                    
                    // Execute KRUSKALs algorithm using Priority Queue made on the fly :)
                    auto startK_PQ = chrono::steady_clock::now();
                    graph->kruskalPQ();
                    auto endK_PQ = chrono::steady_clock::now();
                    auto diffK_PQ = chrono::duration<double, nano>(endK_PQ - startK_PQ).count();
                    cout << diffK_PQ << " ";
                    totalKruskal_PQ += chrono::duration<double, nano>(diffK_PQ).count();
                }
                cout << endl << "TOTAL:" << endl;
                cout << "Prim" << '\t' << "- time: " << totalPrim / amountOfExecutions << endl;
                cout << "Kruskal" << '\t' << "- time: " << totalKruskal / amountOfExecutions << endl;
                cout << "Krus PQ" << '\t' << "- time: " << totalKruskal_PQ / amountOfExecutions << endl;

            }
            break;
        }
        default:
            cout << "Wrong mode... exiting..." << endl;
    }
    return 0;
}
