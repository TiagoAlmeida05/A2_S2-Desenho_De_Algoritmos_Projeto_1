#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include "Graph.h"
#include "functions.h"
#include "driving.h"
#include <vector>

using namespace std;

void loadGraphFromFile(Graph<int> &g);
void displayMenu();
void findRoute(Graph<int> &g);
void batchMode(Graph<int> &g);
void drivingModeBM(Graph<int> &g, const int &source, const int &destination);
void includeNodeBM(Graph<int> &g, const int &source, const int &destination, string includeN);
void drivingWalkingModeBM(Graph<int> &g, const int &source, const int &destination, string maxWT);
void restrictedRouteBM(Graph<int> &g, const int &source, const int &destination);

int main()
{
    system("cls");
    char choice;
    Graph<int> g;
    do
    {

        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case '1':
            loadGraphFromFile(g);
            break;
        case '2':
            findRoute(g);
            break;
        case '3':
            batchMode(g);
            break;
        case '4':
            cout << "Exiting Program... \n";
            break;
        default:
            cout << "Invalid choice \n";
        }
    } while (choice != '4');

    return 0;
}

void displayMenu()
{
    cout << "\n===== Route Planning Analysis Tool =====\n";
    cout << "1. Load Graph from File\n";
    cout << "2. Find Shortest Route\n";
    cout << "3. Batch Mode\n";
    cout << "4. Exit\n";
}

void loadGraphFromFile(Graph<int> &g)
{
    string filename;
    string line;
    map<string, int> codeToId;

    cout << "Enter File Name With Nodes:  \n";
    cin >> filename;

    ifstream inputfile1(filename);

    if (!inputfile1)
    {
        cout << "Error: Could not open " << filename << "!\n";
    }
    getline(inputfile1, line);
    while (getline(inputfile1, line))
    {
        stringstream ss(line);
        string location, idst, code, parkingst;

        getline(ss, location, ',');
        getline(ss, idst, ',');
        getline(ss, code, ',');
        getline(ss, parkingst, ',');

        int id = stoi(idst);
        bool parking = (parkingst == "1");

        g.addVertex(id);
        Vertex<int> *v = g.findVertex(id);
        v->setPark(parking);
        v->setCode(code);
        codeToId[code] = id;
    }

    cout << "Enter File Name With Edges:  \n";
    cin >> filename;

    ifstream inputfile2(filename);

    if (!inputfile2)
    {
        cout << "Error: Could not open " << filename << "!\n";
    }
    getline(inputfile2, line);
    while (getline(inputfile2, line))
    {
        stringstream ss(line);
        string code1, code2, driving, walking;
        getline(ss, code1, ',');
        getline(ss, code2, ',');
        getline(ss, driving, ',');
        getline(ss, walking, ',');

        int newid1 = codeToId[code1];
        int newid2 = codeToId[code2];
        int w = stoi(walking);

        if (driving != "X")
        {
            int d = stoi(driving);
            g.addBidirectionalEdge(newid1,newid2,d,w);
        }
        else
        {
            g.addBidirectionalEdge(newid1, newid2, INF, w);
        }
    }
}

void findRoute(Graph<int> &g)
{
    for (Vertex<int> *k : g.getVertexSet())
    {
        k->setVisited(false);
        for (Edge<int> *e : k->getAdj())
        {
            e->setSelected(false);
        }
    }

    int source, destination;
    string mode, avoidN, temp, avoidS, includeN, maxWT;

    cout << "Mode:";
    cin >> mode;
    cout << "Source:";
    cin >> source;
    cout << "Destination:";
    cin >> destination;

    if (mode == "driving")
    {
        cout << "AvoidNodes:";
        cin.ignore();
        getline(cin, avoidN);
        cout << "AvoidSegments:";
        getline(cin, avoidS);
        cout << "IncludeNode:";
        getline(cin, includeN);
        cout << endl;
        if (avoidN != "" || avoidS != "" || includeN != "")
        {
            if (includeN != "")
            {
                avoidNodesAndSegments(g, avoidN, avoidS);
                includeNode(g, source, destination, includeN);
            }
            else
            {
                avoidNodesAndSegments(g, avoidN, avoidS);
                restrictedRoute(g, source, destination);
            }
        }
        else
        {
            drivingMode(g, source, destination);
        }
    }
    else
    {
        if (mode == "driving-walking")
        {
            cout << "MaxWalkTime:";
            cin.ignore();
            getline(cin, maxWT);
            cout << "AvoidNodes:";
            getline(cin, avoidN);
            cout << "AvoidSegments:";
            getline(cin, avoidS);
            cout << endl;
            if (avoidN != "" || avoidS != ""){
                avoidNodesAndSegments(g, avoidN, avoidS);
                drivingWalkingMode(g, source, destination, maxWT);
            }else{
                drivingWalkingMode(g, source, destination, maxWT);
            }
        }
        else
        {
            cout << "Mode not valid!";
        }
    }
}

void batchMode(Graph<int> &g)
{
    for (Vertex<int> *k : g.getVertexSet())
    {
        k->setVisited(false);
        for (Edge<int> *e : k->getAdj())
        {
            e->setSelected(false);
        }
    }

    int source, destination;
    string mode, avoidN, temp, avoidS, includeN, maxWT, line, palavra;
    
    ifstream ss("input.txt");

    while(getline(ss, line)){
        stringstream xx(line);
        getline(xx, palavra, ':');
        if(palavra == "Mode"){
            getline(xx, palavra);
            mode = palavra;
        }
        if(palavra == "Source"){
            getline(xx, palavra);
            source = stoi(palavra);
        }
        if(palavra == "Destination"){
            getline(xx, palavra);
            destination = stoi(palavra);
        }
        if(palavra == "AvoidNodes"){
            getline(xx, palavra);
            avoidN = palavra;
        }
        if(palavra == "AvoidSegments"){
            getline(xx, palavra);
            avoidS = palavra;
        }
        if(palavra == "IncludeNode"){
            getline(xx, palavra);
            includeN = palavra;
        }
        if(palavra == "MaxWalkTime"){
            getline(xx, palavra);
            maxWT = palavra;
        }
    }
    if (mode == "driving")
    {
        if (avoidN != "" || avoidS != "" || includeN != "")
        {
            if (includeN != "")
            {
                avoidNodesAndSegments(g, avoidN, avoidS);
                includeNodeBM(g, source, destination, includeN);
            }
            else
            {
                avoidNodesAndSegments(g, avoidN, avoidS);
                restrictedRouteBM(g, source, destination);
            }
        }
        else
        {
            drivingModeBM(g, source, destination);
        }
    }
    else
    {
        if (mode == "driving-walking")
        {
            if (avoidN != "" || avoidS != ""){
                avoidNodesAndSegments(g, avoidN, avoidS);
                drivingWalkingModeBM(g, source, destination, maxWT);
            }else{
                drivingWalkingModeBM(g, source, destination, maxWT);
            }
        }
    }
}

void drivingModeBM(Graph<int> &g, const int &source, const int &destination)
{
    ofstream outputFile("output.txt");
    vector<int> path1, path2;
    dijkstra(&g, source,0);
    path1 = getPath(&g, source, destination,0);
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
    dijkstra(&g, source,0);
    path2 = getPath(&g, source, destination,0);
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
    dijkstra(&g, source,0);
    path1 = getPath(&g, source, nod,0);
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
        dijkstra(&g, nod,0);
        path2 = getPath(&g, nod, destination,0);
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
    vector<int> bestDrivingPath, bestWalkingPath;
    double bestTotalTime = INF, bestWalkingTime = -1;
    int bestParkingNode = -1;
    int maxWalkingTime = stoi(maxWT);
    ofstream outputFile("output.txt");

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

    // Step 5: Output the result
    outputFile << "Source:" << source << endl;
    outputFile << "Destination:" << destination << endl;

    if (bestParkingNode == -1) {
        outputFile << "Message: No possible route with max. walking time of " << maxWT << " minutes." << endl;
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
    dijkstra(&g, source,0);
    path = getPath(&g, source, destination,0);
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