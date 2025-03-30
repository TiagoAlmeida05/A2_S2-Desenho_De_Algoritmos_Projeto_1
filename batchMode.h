#ifndef BATCHMODE_H
#define BATCHMODE_H

#include <iostream>
#include "Graph.h"
#include "MutablePriorityQueue.h"

using namespace std;

void drivingModeBM(Graph<int> &g, const int &source, const int &destination);

void includeNodeBM(Graph<int> &g, const int &source, const int &destination, string includeN);

void drivingWalkingModeBM(Graph<int> &g, const int &source, const int &destination, string maxWT);

void restrictedRouteBM(Graph<int> &g, const int &source, const int &destination);


#endif