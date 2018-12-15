//
// Created by Michal Zmudziak on 17/11/2018.
//

#ifndef DJIKSTRA_UTILS_H
#define DJIKSTRA_UTILS_H

#include <vector>
#include <functional>
#include <cstdio>
#include "Graph.h"
using namespace std;

vector<int> indexedVector(int n);

vector<int> fisherYatesShuffle(vector<int> n);

vector<std::pair<int, int>> fisherYatesShuffle(vector<std::pair<int, int>> n);

int random(int min, int max);

#endif //DJIKSTRA_UTILS_H
