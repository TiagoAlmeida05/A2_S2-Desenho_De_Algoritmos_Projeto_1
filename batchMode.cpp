#include <iostream>
#include <sstream>
#include <fstream>
#include "Graph.h"
#include "batchMode.h"
#include "MutablePriorityQueue.h"
#include "functions.h"

using namespace std;

void drivingModeBM(Graph<int> &g, const int &source, const int &destination)
{
    ofstream outputFile("output.txt");
    vector<int> path1, path2;
    dijkstra(&g, source, 0);
    path1 = getPath(&g, source, destination, 0);
    Vertex<int> *v1 = g.findVertex(destination);
    double dist1 = v1->getDist();
    outputFile << "Source:" << source << endl
               << "Destination:" << destination << endl;
    outputFile << "BestDrivingRoute:";
    if (path1.empty())
        outputFile << "none";
    else
    {
        for (size_t i = 0; i < path1.size(); i++)
        {
            outputFile << path1[i];
            if (i != path1.size() - 1)
                outputFile << ",";
            if (i > 0 && i < path1.size() - 1)
            {
                Vertex<int> *h = g.findVertex(path1[i]);
                h->setVisited(true);
            }
        }
        outputFile << '(' << dist1 << ')' << endl;
    }

    Vertex<int> *v2 = g.findVertex(destination);
    dijkstra(&g, source, 0);
    path2 = getPath(&g, source, destination, 0);
    double dist2 = v2->getDist();
    outputFile << "AlternativeDrivingRoute:";

    if (path2.empty())
        outputFile << "none";
    else
    {
        for (size_t i = 0; i < path2.size(); i++)
        {
            outputFile << path2[i];
            if (i != path2.size() - 1)
                outputFile << ",";
        }
        outputFile << '(' << dist2 << ')' << endl;
    }
    outputFile.close();
}

void includeNodeBM(Graph<int> &g, const int &source, const int &destination, string includeN)
{
    ofstream outputFile("output.txt");
    vector<int> path1, path2;
    int nod = stoi(includeN);
    dijkstra(&g, source, 0);
    path1 = getPath(&g, source, nod, 0);
    Vertex<int> *v1 = g.findVertex(nod);
    double dist1 = v1->getDist();
    outputFile << "Source:" << source << endl
               << "Destination:" << destination << endl;
    outputFile << "RestrictedDrivingRoute:";
    if (path1.empty())
        outputFile << "none";
    else
    {
        for (size_t i = 0; i < path1.size(); i++)
        {
            outputFile << path1[i];
            outputFile << ",";
            if (i < path1.size() - 1)
            {
                Vertex<int> *h = g.findVertex(path1[i]);
                h->setVisited(true);
            }
        }
        dijkstra(&g, nod, 0);
        path2 = getPath(&g, nod, destination, 0);
        Vertex<int> *v2 = g.findVertex(destination);
        double dist2 = v2->getDist();
        for (size_t i = 1; i < path2.size(); i++)
        {
            outputFile << path2[i];
            if (i != path2.size() - 1)
                outputFile << ",";
        }
        outputFile << '(' << dist2 + dist1 << ')' << endl;
    }
    outputFile.close();
}

void drivingWalkingModeBM(Graph<int> &g, const int &source, const int &destination, string maxWT) {
    ofstream outputFile("output.txt");
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

    outputFile << "Source:" << source << endl;
    outputFile << "Destination: " << destination << endl;

    if (bestParkingNode == -1) {


        outputFile << "DrivingRoute:"<<endl;
        outputFile << "ParkingNode:" <<endl;
        outputFile << "WalkingRoute:"<<endl;
        outputFile << "TotalTime:" <<endl;
        outputFile << "Message: No possible route with max. walking time of " << maxWT << " minutes." << endl;
        outputFile <<endl;
        approximateSolutionBM(g,source,destination, outputFile);

        return;
    }

    outputFile << "DrivingRoute:";
    for (size_t i = 0; i < bestDrivingPath.size(); i++) {
        outputFile << bestDrivingPath[i];
        if (i != bestDrivingPath.size() - 1) outputFile << ",";
    }
    outputFile << "(" << bestTotalTime - bestWalkingTime << ")" << endl;

    outputFile << "ParkingNode:" << bestParkingNode << endl;

    outputFile << "WalkingRoute:";
    if (bestWalkingPath.empty()) outputFile << "none";
    else {
        for (size_t i = 0; i < bestWalkingPath.size(); i++) {
        outputFile << bestWalkingPath[i];
        if (i != bestWalkingPath.size() - 1) outputFile << ",";
        }
    }
    outputFile << "(" << bestWalkingTime << ")" << endl;

    outputFile << "TotalTime:" << bestTotalTime << endl;
    outputFile.close();
}

void restrictedRouteBM(Graph<int> &g, const int &source, const int &destination)
{
    ofstream outputFile("output.txt");
    vector<int> path;
    dijkstra(&g, source, 0);
    path = getPath(&g, source, destination, 0);
    Vertex<int> *v = g.findVertex(destination);
    double dist = v->getDist();
    outputFile << "Source:" << source << endl
               << "Destination:" << destination << endl;
    outputFile << "RestrictedDrivingRoute:";
    if (path.empty())
        outputFile << "none";
    else
    {
        for (size_t i = 0; i < path.size(); i++)
        {
            outputFile << path[i];
            if (i != path.size() - 1)
                outputFile << ",";
        }
        outputFile << '(' << dist << ')' << endl;
    }
    outputFile.close();
}

void approximateSolutionBM(Graph<int> &g, const int &source, const int &destination, ofstream& outputFile) {
    vector<int> bestDrivingPath, bestWalkingPath;
    double bestTotalTime = INF, bestWalkingTime = -1;
    vector<int> SecondbestDrivingPath, SecondbestWalkingPath;
    double SecondbestTotalTime = INF, SecondbestWalkingTime = -1;
    double AproxSolTTime=INF,AproxSolwTime;
    int bestParkingNode = -1;
    int SecondBestParkingNode=-1;
    outputFile << "Source:" << source << endl;
    outputFile << "Destination:" << destination << endl;
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
    outputFile << "DrivingRoute1:";
    for (size_t i = 0; i < bestDrivingPath.size(); i++) {
        outputFile << bestDrivingPath[i];
        if (i != bestDrivingPath.size() - 1) outputFile << ",";
    }
    outputFile << "(" << bestTotalTime - bestWalkingTime << ")" << endl;

    outputFile << "ParkingNode1:" << bestParkingNode << endl;

    outputFile << "WalkingRoute1:";
    if (bestWalkingPath.empty()) outputFile << "none";
    else {
        for (size_t i = 0; i < bestWalkingPath.size(); i++) {
        outputFile << bestWalkingPath[i];
        if (i != bestWalkingPath.size() - 1) outputFile << ",";
        }
    }
    outputFile << "(" << bestWalkingTime << ")" << endl;

    outputFile << "TotalTime1:" << bestTotalTime << endl;
    outputFile << "DrivingRoute2:";
    for (size_t i = 0; i < SecondbestDrivingPath.size(); i++) {
        outputFile << SecondbestDrivingPath[i];
        if (i != SecondbestDrivingPath.size() - 1) outputFile << ",";
    }
    outputFile << "(" << SecondbestTotalTime - SecondbestWalkingTime << ")" << endl;

    outputFile << "ParkingNode2:" << SecondBestParkingNode << endl;

    outputFile << "WalkingRoute2:";
    if (SecondbestWalkingPath.empty()) outputFile << "none";
    else {
        for (size_t i = 0; i < SecondbestWalkingPath.size(); i++) {
        outputFile << SecondbestWalkingPath[i];
        if (i != SecondbestWalkingPath.size() - 1) outputFile << ",";
        }
    }
    outputFile << "(" << SecondbestWalkingTime << ")" << endl;

    outputFile << "TotalTime2:" << SecondbestTotalTime << endl;
}


