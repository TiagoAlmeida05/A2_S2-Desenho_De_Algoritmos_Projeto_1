#ifndef DRIVING_H
#define DRIVING_H

#include <iostream>
#include "Graph.h"
#include "MutablePriorityQueue.h"

/**
 * @file driving.h
 * @brief This header file contains function prototypes and class declarations used for console input operations in the graph processing system.
 */
using namespace std;

/**
 * @brief Marks specified nodes and segments as "visited" or "selected" for route planning.
 *
 * This function allows the user to specify nodes and segments to be avoided while calculating the route.
 * It will mark the nodes as visited and mark the edges as selected based on the provided input strings.
 * 
 * @param g The graph containing all the nodes and edges.
 * @param avoidN Comma-separated list of node IDs to be avoided.
 * @param avoidS Comma-separated list of edge segments to be avoided.
 */
void avoidNodesAndSegments(Graph<int> &g, string avoidN, string avoidS);

/**
 * @brief Finds the best driving route from source to destination using Dijkstra's algorithm.
 *
 * @param g Graph representing the road network.
 * @param source Starting node.
 * @param destination Destination node.
 */
void drivingMode(Graph<int> &g, const int &source, const int &destination);

/**
 * @brief Calculates a restricted driving route using Dijkstra's algorithm.
 *
 * @param g The graph representing the road network.
 * @param source The source vertex from which the path begins.
 * @param destination The destination vertex where the path ends.
 */
void restrictedRoute(Graph<int> &g, const int &source, const int &destination);

/**
 * @brief Finds a restricted driving route that includes a specific node.
 *
 * @param g Graph representing the road network.
 * @param source Starting node.
 * @param destination Destination node.
 * @param includeN Node that must be included in the path.
 */
void includeNode(Graph<int> &g, const int &source, const int &destination, string includeN);

/**
 * @brief Calculates the best driving and walking route with a given maximum walking time.
 *
 * @param g The graph representing the road network.
 * @param source The source vertex from which the path begins.
 * @param destination The destination vertex where the path ends.
 * @param maxWT The maximum walking time allowed.
 */
void drivingWalkingMode(Graph<int> &g, const int &source, const int &destination, string maxWT);

/**
 * @brief Finds an approximate driving and walking route when the best route exceeds walking limits.
 * @param g Graph representing the road network.
 * @param source Starting node.
 * @param destination Destination node.
 * @param outputFile Output file stream to write results.
 */
void approximateSolution(Graph<int> &g, const int &source, const int &destination);

#endif