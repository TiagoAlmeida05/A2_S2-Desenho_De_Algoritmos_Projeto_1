#ifndef BATCHMODE_H
#define BATCHMODE_H

#include <iostream>
#include "Graph.h"
#include "MutablePriorityQueue.h"

/**
 * @file batchMode.h
 * @brief This header file contains function prototypes and class declarations used for batch mode operations in the graph processing system.
 */
using namespace std;

/**
 * @brief Finds the best driving route from source to destination using Dijkstra's algorithm.
 *
 * @param g Graph representing the road network.
 * @param source Starting node.
 * @param destination Destination node.
 */
void drivingModeBM(Graph<int> &g, const int &source, const int &destination);

/**
 * @brief Finds a restricted driving route that includes a specific node.
 *
 * @param g Graph representing the road network.
 * @param source Starting node.
 * @param destination Destination node.
 * @param includeN Node that must be included in the path.
 */
void includeNodeBM(Graph<int> &g, const int &source, const int &destination, string includeN);

/**
 * @brief Calculates the best driving and walking route with a given maximum walking time.
 *
 * @param g The graph representing the road network.
 * @param source The source vertex from which the path begins.
 * @param destination The destination vertex where the path ends.
 * @param maxWT The maximum walking time allowed.
 */
void drivingWalkingModeBM(Graph<int> &g, const int &source, const int &destination, string maxWT);

/**
 * @brief Calculates a restricted driving route using Dijkstra's algorithm.
 *
 * @param g The graph representing the road network.
 * @param source The source vertex from which the path begins.
 * @param destination The destination vertex where the path ends.
 */
void restrictedRouteBM(Graph<int> &g, const int &source, const int &destination);

/**
 * @brief Finds an approximate driving and walking route when the best route exceeds walking limits.
 * @param g Graph representing the road network.
 * @param source Starting node.
 * @param destination Destination node.
 * @param outputFile Output file stream to write results.
 */
void approximateSolutionBM(Graph<int> &g, const int &source, const int &destination, ofstream &outputFile);

#endif