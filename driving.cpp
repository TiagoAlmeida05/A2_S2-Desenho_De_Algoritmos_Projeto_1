#include <iostream>
#include <sstream>
#include "Graph.h"
#include "driving.h"
#include "MutablePriorityQueue.h"
#include "functions.h"

using namespace std;

void avoidNodesAndSegments(Graph<int> &g, string avoidN, string avoidS, const int &source, const int &destination) {
    string temp, orig, dest;
    stringstream ss(avoidN);
    while (getline(ss, temp, ','))
    {
        Vertex<int> *h = g.findVertex(stoi(temp));
        h->setVisited(true);
    }

    stringstream sa(avoidS);
    while (getline(sa, temp, '('))
    {
        getline(sa, orig, ',');
        getline(sa, dest, ')');
        Vertex<int> *v = g.findVertex(stoi(orig));
        for (Edge<int> *e : v->getAdj())
        {
            if (e->getDest()->getInfo() == stoi(dest))
            {
                e->setSelected(true);
            }
        }
    }

}

void restrictedRoute(Graph<int> &g, const int &source, const int &destination) {
    vector<int> path;
    dijkstra(&g, source,0);
    path = getPath(&g, source, destination,0);
    Vertex<int> *v = g.findVertex(destination);
    double dist = v->getDist();
    cout << "Source:" << source << endl
    << "Destination:" << destination << endl;
    cout << "RestrictedDrivingRoute:";
    if (path.empty())
        cout << "none";
    else
    {
        for (size_t i = 0; i < path.size(); i++)
        {
            cout << path[i];
            if (i != path.size() - 1)
                 cout << ",";
        }
        cout << '(' << dist << ')' << endl;
    }
}

void includeNode(Graph<int> &g, const int &source, const int &destination, string includeN) {
    vector<int> path1, path2;
    int nod = stoi(includeN);
    
    dijkstra(&g, source,0);
    path1 = getPath(&g, source, nod,0);
    
    Vertex<int> *v1 = g.findVertex(nod);
    double dist1 = v1->getDist();
    cout << "Source:" << source << endl
    << "Destination:" << destination << endl;
    cout << "RestrictedDrivingRoute:";
    if (path1.empty())
        cout << "none";
    else
    {
        for (size_t i = 0; i < path1.size(); i++)
        {
            cout << path1[i];
            cout << ",";
            if (i < path1.size() - 1)
            {
                Vertex<int> *h = g.findVertex(path1[i]);
                h->setVisited(true);
            }
        }
        dijkstra(&g, nod,0);
        path2 = getPath(&g, nod, destination,0);
        Vertex<int> *v2 = g.findVertex(destination);
        double dist2 = v2->getDist();
        for (size_t i = 1; i < path2.size(); i++)
        {
            cout << path2[i];
            if (i != path2.size() - 1)
                cout << ",";
        }
        cout << '(' << dist2 + dist1 << ')' << endl;
    }
}


void drivingMode(Graph<int> &g, const int &source, const int &destination) {
    vector<int> path1, path2;
    dijkstra(&g, source,0);
    path1 = getPath(&g, source, destination,0);
    Vertex<int> *v1 = g.findVertex(destination);
    double dist1 = v1->getDist();
    cout << "Source:" << source << endl
         << "Destination:" << destination << endl;
    cout << "BestDrivingRoute:";
    if (path1.empty())
        cout << "none";
    else
    {
        for (size_t i = 0; i < path1.size(); i++)
        {
            cout << path1[i];
            if (i != path1.size() - 1)
                cout << ",";
            if (i > 0 && i < path1.size() - 1)
            {
                Vertex<int> *h = g.findVertex(path1[i]);
                h->setVisited(true);
            }
        }
        cout << '(' << dist1 << ')' << endl;
    }

    Vertex<int> *v2 = g.findVertex(destination);
    dijkstra(&g, source,0);
    path2 = getPath(&g, source, destination,0);
    double dist2 = v2->getDist();
    cout << "AlternativeDrivingRoute:";

    if (path2.empty())
        cout << "none";
    else
    {
        for (size_t i = 0; i < path2.size(); i++)
        {
            cout << path2[i];
            if (i != path2.size() - 1)
                cout << ",";
        }
        cout << '(' << dist2 << ')' << endl;
    }
}

void approximateSolution(Graph<int> &g, const int &source, const int &destination) {
    vector<int> bestDrivingPath, bestWalkingPath;
    double bestTotalTime = INF, bestWalkingTime = -1;
    vector<int> SecondbestDrivingPath, SecondbestWalkingPath;
    double SecondbestTotalTime = INF, SecondbestWalkingTime = -1;
    double AproxSolTTime=INF,AproxSolwTime;
    int bestParkingNode = -1;
    int SecondBestParkingNode=-1;
    cout << "Source: " << source << endl;
    cout << "Destination: " << destination << endl;
    for (Vertex<int>* v : g.getVertexSet()) {
        if (v->hasPark() && v->getDist() < INF) { // Valid parking node
            int parkingNode = v->getInfo();
            double drivingTime = v->getDist();

            
            dijkstra(&g, parkingNode,1);
            Vertex<int>* destVertex = g.findVertex(destination);

            if (destVertex == nullptr || destVertex->getWDist() >= INF) continue;
            double walkingTime = destVertex->getWDist();
            double totalTime = drivingTime + walkingTime;

            // Step 4: Select the best parking node and route
            if (totalTime < SecondbestTotalTime || (totalTime == SecondbestTotalTime && walkingTime > SecondbestTotalTime)) {
                if (totalTime < bestTotalTime || (totalTime == bestTotalTime && walkingTime > bestWalkingTime)) {
                        SecondbestTotalTime = bestTotalTime;
                        SecondbestWalkingTime = bestWalkingTime;
                        SecondbestDrivingPath = bestDrivingPath;
                        SecondbestWalkingPath = bestWalkingPath;
                        SecondBestParkingNode = bestParkingNode;
                        bestTotalTime = totalTime;
                        bestWalkingTime = walkingTime;
                        bestDrivingPath = getPath(&g, source, parkingNode,0);
                        bestWalkingPath = getPath(&g, parkingNode, destination,1);
                        bestParkingNode = parkingNode;
                }
                else {
                        SecondbestTotalTime = totalTime;
                        SecondbestWalkingTime = walkingTime;
                        SecondbestDrivingPath = getPath(&g, source, parkingNode,0);
                        SecondbestWalkingPath = getPath(&g, parkingNode, destination,1);
                        SecondBestParkingNode = parkingNode;
                }
                }
        }
            
    }
    cout << "DrivingRoute1: ";
    for (size_t i = 0; i < bestDrivingPath.size(); i++) {
        cout << bestDrivingPath[i];
        if (i != bestDrivingPath.size() - 1) cout << ",";
    }
    cout << "(" << bestTotalTime - bestWalkingTime << ")" << endl;

    cout << "ParkingNode1: " << bestParkingNode << endl;

    cout << "WalkingRoute1: ";
    if (bestWalkingPath.empty()) cout << "none";
    else {
        for (size_t i = 0; i < bestWalkingPath.size(); i++) {
        cout << bestWalkingPath[i];
        if (i != bestWalkingPath.size() - 1) cout << ",";
        }
    }
    cout << "(" << bestWalkingTime << ")" << endl;

    cout << "TotalTime1: " << bestTotalTime << endl;
    cout << "DrivingRoute2: ";
    for (size_t i = 0; i < SecondbestDrivingPath.size(); i++) {
        cout << SecondbestDrivingPath[i];
        if (i != SecondbestDrivingPath.size() - 1) cout << ",";
    }
    cout << "(" << SecondbestTotalTime - SecondbestWalkingTime << ")" << endl;

    cout << "ParkingNode2: " << SecondBestParkingNode << endl;

    cout << "WalkingRoute2: ";
    if (SecondbestWalkingPath.empty()) cout << "none";
    else {
        for (size_t i = 0; i < SecondbestWalkingPath.size(); i++) {
        cout << SecondbestWalkingPath[i];
        if (i != SecondbestWalkingPath.size() - 1) cout << ",";
        }
    }
    cout << "(" << SecondbestWalkingTime << ")" << endl;

    cout << "TotalTime2: " << SecondbestTotalTime << endl;
}

void drivingWalkingMode(Graph<int> &g, const int &source, const int &destination, string maxWT) {
    vector<int> bestDrivingPath, bestWalkingPath;
    double bestTotalTime = INF, bestWalkingTime = -1;
    double AproxSolTTime=INF,AproxSolwTime;
    int bestParkingNode = -1;
    int maxWalkingTime = stoi(maxWT);

    dijkstra(&g, source,0);
    
    for (Vertex<int>* v : g.getVertexSet()) {
        if (v->hasPark() && v->getDist() < INF) { // Valid parking node
            int parkingNode = v->getInfo();
            double drivingTime = v->getDist();

            
            dijkstra(&g, parkingNode,1);
            Vertex<int>* destVertex = g.findVertex(destination);

            if (destVertex == nullptr || destVertex->getWDist() >= INF) continue;
            double walkingTime = destVertex->getWDist();
            double totalTime = drivingTime + walkingTime;

            // Step 4: Select the best parking node and route
            if (walkingTime <= maxWalkingTime) {
                if (totalTime < bestTotalTime || 
                   (totalTime == bestTotalTime && walkingTime > bestWalkingTime)) {
                    bestTotalTime = totalTime;
                    bestWalkingTime = walkingTime;
                    bestDrivingPath = getPath(&g, source, parkingNode,0);
                    bestWalkingPath = getPath(&g, parkingNode, destination,1);
                    bestParkingNode = parkingNode;
                }
            }
        }
    }

    cout << "Source: " << source << endl;
    cout << "Destination: " << destination << endl;

    if (bestParkingNode == -1) {


        cout << "DrivingRoute: "<<endl;
        cout << "ParkingNode: " <<endl;
        cout << "WalkingRoute: "<<endl;
        cout << "TotalTime: " <<endl;
        cout << "Message: No possible route with max. walking time of " << maxWT << " minutes." << endl;
        cout <<endl;
        approximateSolution(g,source,destination);

        return;
    }

    cout << "DrivingRoute: ";
    for (size_t i = 0; i < bestDrivingPath.size(); i++) {
        cout << bestDrivingPath[i];
        if (i != bestDrivingPath.size() - 1) cout << ",";
    }
    cout << "(" << bestTotalTime - bestWalkingTime << ")" << endl;

    cout << "ParkingNode: " << bestParkingNode << endl;

    cout << "WalkingRoute: ";
    if (bestWalkingPath.empty()) cout << "none";
    else {
        for (size_t i = 0; i < bestWalkingPath.size(); i++) {
        cout << bestWalkingPath[i];
        if (i != bestWalkingPath.size() - 1) cout << ",";
        }
    }
    cout << "(" << bestWalkingTime << ")" << endl;

    cout << "TotalTime: " << bestTotalTime << endl;
}