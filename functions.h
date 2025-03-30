
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include "Graph.h"
#include "MutablePriorityQueue.h"

/**
 * @file functions.h
 * @brief This file contains graph-related utility functions including Dijkstra's algorithm.
 */

using namespace std;

/**
 * @brief Relaxes an edge in the graph to update the shortest path estimate.
 * @brief Complexity O(1) - Constant time operation.
 * @tparam T The type of data stored in the graph's vertices.
 * @param edge The edge to relax.
 * @param walking Boolean flag indicating if walking mode is used.
 * @return True if the relaxation was successful, false otherwise.
 * @complexity O(1) - Constant time operation.
 */
template <class T>
bool relax(Edge<T> *edge,bool walking) {

    if(!walking) {
        double newDist = edge->getOrig()->getDist() + edge->getDWeight();
        if (newDist < edge->getDest()->getDist()) { // Only update if new distance is shorter

            edge->getDest()->setDist(newDist);
            edge->getDest()->setPath(edge); 
            return true;
        }
       
    }
    else {
        double newDist = edge->getOrig()->getWDist() + edge->getWWeight();
        if (newDist < edge->getDest()->getWDist()) { // Only update if new distance is shorter

            edge->getDest()->setWDist(newDist);
            edge->getDest()->setWPath(edge); 
            return true;
        }
    }
    return false;
}

/**
 * @brief Implements Dijkstra's algorithm to find the shortest path from a source vertex.
 * @brief Complexity O((V + E) log V) - Uses a priority queue for efficient shortest path computation.
 * @tparam T The type of data stored in the graph's vertices.
 * @param g The graph on which Dijkstra's algorithm is run.
 * @param origin The source vertex.
 * @param walking Boolean flag indicating if walking mode is used.
 */
template <class T>
void dijkstra(Graph<T> * g, const int &origin,bool walking) {
    // Initialize the vertices
    for(Vertex<T>* v : g->getVertexSet()) {
        v->walking=walking;
        if(!walking){
            v->setDist(INF);
            v->setPath(nullptr);
        }
        else {
            v->setWDist(INF);
            v->setWPath(nullptr);
        }
    }

    Vertex<T>* s = g->findVertex(origin);
    if (!s) {
        cerr << "Error: Origin vertex not found!" << endl;
        return;
    }
    if(!walking)  s->setDist(0);
    else s->setWDist(0);

    MutablePriorityQueue<Vertex<T>> q;
    q.insert(s);
    while( ! q.empty() ) {
        Vertex<T>* v = q.extractMin();
       
        if (v == nullptr) {
            cerr << "Error: Extracted null vertex from priority queue!" << endl;
            return;
        }
        for(Edge<T>* e : v->getAdj()) {
            if(e->isSelected()) continue;
            if(v->isVisited()) continue;
            double oldDist = walking ? e->getDest()->getWDist() : e->getDest()->getDist();

            if (relax(e,walking)) {
                if (oldDist == INF) {
                    q.insert(e->getDest());
                }
                else {
                    q.decreaseKey(e->getDest());
                }
            }
        }  
    }
    
}

/**
 * @brief Retrieves the shortest path from the origin to the destination.
 * @brief complexity O(V) - The function traces the path backward from destination to origin.
 * @tparam T The type of data stored in the graph's vertices.
 * @param g The graph containing the vertices.
 * @param origin The starting vertex.
 * @param dest The destination vertex.
 * @param walking Boolean flag indicating if walking mode is used.
 * @return A vector representing the shortest path from origin to destination.
 */
template <class T>
vector<T> getPath(Graph<T> * g, const int &origin, const int &dest,bool walking) {
    std::vector<T> res;
    Vertex<T>* v = g->findVertex(dest);
    if(!walking) {
        if (v == nullptr || v->getDist() == INF) { // missing or disconnected
            return res;
        }
        res.push_back(v->getInfo());
        while(v->getPath() != nullptr){
            v = v->getPath()->getOrig();
            if(v->isVisited()) continue;
            res.push_back(v->getInfo());        
        }
        reverse(res.begin(), res.end());
        if(res.empty() || res[0] != origin) {
            std::cout << "Origin not found!!" << std::endl;
        }
        return res;
    }
    else {
        if (v == nullptr || v->getWDist() == INF) { // missing or disconnected
            return res;
        }
        res.push_back(v->getInfo());
        while (v != nullptr && v->getWPath() != nullptr) {
            v = v->getWPath()->getOrig();
            if (v->isVisited()) continue;
            res.push_back(v->getInfo());
        }
        
        
        reverse(res.begin(), res.end());
        if(res.empty() || res[0] != origin) {
            std::cout << "Origin not found!!" << std::endl;
        }
        return res;
    } 
}


#endif