#ifndef DRIVING_H
#define DRIVING_H

#include <iostream>
#include "Graph.h"
#include "MutablePriorityQueue.h"

using namespace std;

void avoidNodesAndSegments(Graph<int> &g, string avoidN, string avoidS);

void drivingMode(Graph<int> &g, const int &source, const int &destination);

void restrictedRoute(Graph<int> &g, const int &source, const int &destination);

void includeNode(Graph<int> &g, const int &source, const int &destination, string includeN);

void drivingWalkingMode(Graph<int> &g, const int &source, const int &destination, string maxWT);

void approximateSolution(Graph<int> &g, const int &source, const int &destination);


#endif