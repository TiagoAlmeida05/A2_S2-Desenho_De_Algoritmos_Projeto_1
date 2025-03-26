#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include "Graph.h"
#include "functions.h"
#include <vector>

using namespace std;

void loadGraphFromFile(Graph<int> &g);
void displayMenu();
void findRoute(Graph<int> &g);
void batchMode(Graph<int> &g);
void avoidNodesAndSegments(Graph<int> &g, string avoidN, string avoidS, const int &source, const int &destination);
void simplePathFinder(Graph<int> &g, const int &source, const int &destination);

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

    cout << "Enter File Name With Nods:  \n";
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
            g.addEdge(newid1, newid2, d, w);
            g.addEdge(newid2, newid1, d, w);
        }
        else
        {
            g.addEdge(newid1, newid2, INF, w);
            g.addEdge(newid2, newid1, INF, w);
        }
    }
}

void findRoute(Graph<int> &g)
{
    for (Vertex<int> *k : g.getVertexSet())
    {
        k->setVisited(false);
    }

    int source, destination, includeN;
    string mode, avoidN, temp, avoidS;
    vector<int> path1, path2;

    cout << "Mode:";
    cin >> mode;
    cout << "Source:";
    cin >> source;
    cout << "Destination:";
    cin >> destination;

    if (mode == "driving")
    {
        cout << "AvoidNods:";
        cin.ignore();
        getline(cin, avoidN);
        cout << "AvoidSegments:";
        cin.ignore();
        getline(cin, avoidS);
        cout << "IncludeNode:";
        cin >> includeN;
        cout << endl;
        if (avoidN != "" || avoidS != "")
        {
            avoidNodesAndSegments(g, avoidN, avoidS, source, destination);
        }else{
            simplePathFinder(g, source, destination);            
        }

    }
    else
    {
        if (mode == "driving-walking")
        {
        }
        else
        {
            cout << "Mode not valid!";
        }
    }
}

void batchMode(Graph<int> &g)
{
}

void avoidNodesAndSegments(Graph<int> &g, string avoidN, string avoidS, const int &source, const int &destination)
{
    string temp;
    stringstream ss(avoidN);
    vector<int> path;
        while (getline(ss, temp, ','))
        {
            Vertex<int> *h = g.findVertex(stoi(temp));
            h->setVisited(true);
        }
}

void simplePathFinder(Graph<int> &g, const int &source, const int &destination)
{
    vector<int> path1, path2;
    dijkstra(&g, source);
            path1 = getPath(&g, source, destination);
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
                    if (i > 0 && i < path1.size())
                    {
                        Vertex<int> *h = g.findVertex(path1[i]);
                        h->setVisited(true);
                    }
                }
                cout << '(' << dist1 << ')' << endl;
            }

            Vertex<int> *v2 = g.findVertex(destination);
            dijkstra(&g, source);
            path2 = getPath(&g, source, destination);
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

void includeNode(Graph<int> &g, const int &source, const int &destination, const int includeN)
{
    vector<int> path1, path2;
    dijkstra(&g, source);
            path1 = getPath(&g, source, includeN);
            Vertex<int> *v1 = g.findVertex(includeN);
            double dist1 = v1->getDist();
            for (size_t i = 0; i < path1.size(); i++)
                {
                    cout << path1[i];
                    if (i != path1.size() - 1)
                        cout << ",";
                    if (i > 0 && i < path1.size())
                    {
                        Vertex<int> *h = g.findVertex(path1[i]);
                        h->setVisited(true);
                    }
                }

            Vertex<int> *v2 = g.findVertex(destination);
            dijkstra(&g, includeN);
            path2 = getPath(&g, includeN, destination);
            double dist2 = v2->getDist();


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