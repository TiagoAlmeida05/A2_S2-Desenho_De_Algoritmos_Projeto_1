#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include "Graph.h"
#include "MutablePriorityQueue.h"

using namespace std;

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
        double newDist = edge->getOrig()->getDist() + edge->getWWeight();
        if (newDist < edge->getDest()->getDist()) { // Only update if new distance is shorter

            edge->getDest()->setDist(newDist);
            edge->getDest()->setPath(edge); 
            return true;
        }
    }
    return false;
}


template <class T>
void dijkstra(Graph<T> * g, const int &origin,bool walking) {
    // Initialize the vertices
    for(Vertex<T>* v : g->getVertexSet()) {
        v->setDist(INF);
        v->setPath(nullptr);
        v->setWDist(INF);
    }

    Vertex<T>* s = g->findVertex(origin);
    if (!s) {
        cerr << "Error: Origin vertex not found!" << endl;
        return;
    }

    s->setDist(0);
    s->setWDist(0);

    MutablePriorityQueue<Vertex<T>> q;
    q.insert(s);
    while( ! q.empty() ) {
        Vertex<T>* v = q.extractMin();
        for(Edge<T>* e : v->getAdj()) {
            if(e->isSelected()) continue;
            if(v->isVisited()) continue;
            if(!walking) auto oldDist = e->getDest()->getDist();
            else auto oldDist = e->getDest()->getWDist();
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

template <class T>
static std::vector<T> getPath(Graph<T> * g, const int &origin, const int &dest,bool walking) {
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
        while(v->getWPath() != nullptr){
            v = v->getWPath()->getOrig();
            if(v->isVisited()) continue;
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