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
void drivingMode(Graph<int> &g, const int &source, const int &destination);
void restrictedRoute(Graph<int> &g, const int &source, const int &destination);
void includeNode(Graph<int> &g, const int &source, const int &destination, string includeN);
void drivingWalkingMode(Graph<int> &g, const int &source, const int &destination, string maxWT);

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
        for (Edge<int> *e : k->getAdj())
        {
            e->setSelected(false);
        }
    }

    int source, destination;
    string mode, avoidN, temp, avoidS, includeN, maxWT;
    vector<int> path1, path2;

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
                avoidNodesAndSegments(g, avoidN, avoidS, source, destination);
                includeNode(g, source, destination, includeN);
            }
            else
            {
                avoidNodesAndSegments(g, avoidN, avoidS, source, destination);
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
                avoidNodesAndSegments(g, avoidN, avoidS, source, destination);
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
}

void avoidNodesAndSegments(Graph<int> &g, string avoidN, string avoidS, const int &source, const int &destination)
{
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

void drivingMode(Graph<int> &g, const int &source, const int &destination)
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
            if (i > 0 && i < path1.size() - 1)
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

void includeNode(Graph<int> &g, const int &source, const int &destination, string includeN)
{

    vector<int> path1, path2;
    int nod = stoi(includeN);
    dijkstra(&g, source);
    path1 = getPath(&g, source, nod);
    Vertex<int> *v1 = g.findVertex(nod);
    double dist1 = v1->getDist();
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
        dijkstra(&g, nod);
        path2 = getPath(&g, nod, destination);
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

void restrictedRoute(Graph<int> &g, const int &source, const int &destination)
{
    vector<int> path;
    dijkstra(&g, source);
    path = getPath(&g, source, destination);
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

void drivingWalkingMode(Graph<int> &g, const int &source, const int &destination, string maxWT){
    vector<int> path;
    int parkingProx, j = 0;
    double walkingTime= 0;
    dijkstra(&g, source);
    path = getPath(&g, source, destination);
    cout << "Source:" << source << endl
         << "Destination:" << destination << endl;
    cout << "DrivingRoute:";
    if (path.empty()){
        cout<<endl;
        cout<<"ParkingNode:"<<endl;
        cout<< "WalkingRoute:"<< endl;
        cout<< "TotalTime:"<<endl;
        cout<< "Message: No path was found";
    }
    else
    {
        for (size_t i = 0; i < path.size()-1; i++)
        {
            if(g.findVertex(path[i])->hasPark() == true){
                parkingProx = path[i];
            }     
        }
        for (size_t i = 0; i < path.size()-1; i++)
        {
            if(path[i] == parkingProx){
                j = 1;
            }
            if(j ==1){
                Vertex<int> *v = g.findVertex(path[i]);
                for (Edge<int> *e : v->getAdj()){
                    if(e->getDest()->getInfo() == path[i+1]){
                        walkingTime = walkingTime + e->getWWeight();
                    }
                }
            }
        }
        Vertex<int> *v1 = g.findVertex(parkingProx);
        double dist = v1->getDist();
        if(stoi(maxWT) < walkingTime){
            cout<<endl;
            cout<<"ParkingNode:"<<endl;
            cout<< "WalkingRoute:"<< endl;
            cout<< "TotalTime:"<<endl;
            cout<< "Message: No possible route with max. walking time of "<< maxWT << " minutes."<< endl;
        }else{
            for (size_t i = 0; i < path.size(); i++){
                if(path[i] == parkingProx){
                        cout << path[i];
                        cout << '(' << dist << ')' << endl;
                        cout << "ParkingNode:" << path[i] << endl;
                        cout << "WalkingRoute:";                        
                }
                cout << path[i];
                if (i != path.size() - 1)
                    cout << ",";
            }
            cout << "(" << walkingTime << ")"<< endl;
            cout << "TotalTime:" << walkingTime + dist<< endl;
        }       
    }
}