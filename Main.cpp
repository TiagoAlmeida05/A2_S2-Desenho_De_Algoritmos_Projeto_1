#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include "Graph.h"
#include "functions.h"
#include "driving.h"
#include "batchMode.h"
#include <vector>

using namespace std;

/**
 * @file Main.cpp
 * @brief A program for route planning and analysis, including loading graph data, finding routes, 
 * and processing routes in batch mode.
 * 
 * The program allows users to load a graph of nodes and edges, choose a route mode (driving, walking, or mixed),
 * and compute the shortest path based on various parameters like avoiding certain nodes or segments, and 
 * maximum walking time. The program also supports batch processing from an input file.
 *
*/

/**
 * @brief Displays the main menu for the user to choose an option.
 */
void displayMenu();

/**
 * @brief Loads a graph from a file, reading vertices and edges.
 * @param g The graph object to populate with vertices and edges.
 */
void loadGraphFromFile(Graph<int> &g);

/**
 * @brief Finds and computes the route based on the user's input.
 * @param g The graph to search for the route.
 */
void findRoute(Graph<int> &g);

/**
 * @brief Executes the batch mode, reading the configuration from an input file and computing the route.
 * @param g The graph to process.
 */
void batchMode(Graph<int> &g);

/**
 * @brief Main program loop where the user interacts with the route planning tool.
 * @return int Status code (0 for success).
 */
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
