#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include "Graph.h"
#include "functions.h"
#include <vector>

using namespace std;

void loadGraphFromFile(Graph <int>& g){
    string filename;
    string line;
    map<string, int> codeToId;

    cout << "Enter File Name With Nods:  \n";
    cin >> filename;

    ifstream inputfile1(filename);

    if(!inputfile1){
        cout <<"Error: Could not open " << filename << "!\n";
    }
    getline(inputfile1, line);
    while (getline(inputfile1, line)){
        stringstream ss(line);
        string location, idst, code, parkingst;
        
        getline(ss, location, ',');
        getline(ss, idst, ',');
        getline(ss, code, ',');
        getline(ss, parkingst, ',');
        
        int id = stoi(idst);
        bool parking = (parkingst == "1");

        g.addVertex(id);
        Vertex<int>* v =g.findVertex(id);
        v->setPark(parking);
        v->setCode(code);
        codeToId[code]=id;
    }

    cout << "Enter File Name With Edges:  \n";
    cin >> filename;

    ifstream inputfile2(filename);


    if(!inputfile2){
        cout <<"Error: Could not open " << filename << "!\n";
    }
    getline(inputfile2, line);
    while (getline(inputfile2, line)){
        stringstream ss(line);
        string code1, code2, driving, walking;
        getline(ss, code1, ',');
        getline(ss, code2, ',');
        getline(ss, driving, ',');
        getline(ss, walking, ',');

        int newid1= codeToId[code1];
        int newid2= codeToId[code2];
        int w= stoi(walking);

        if(driving!="X") {
            int d= stoi(driving);
            g.addEdge(newid1,newid2,d,w);
            g.addEdge(newid2,newid1,d,w);
        }
        else {
            g.addEdge(newid1,newid2,INF,w);
            g.addEdge(newid2,newid1,INF,w);
        }
        
    }
}
void displayMenu();
void findRoute(Graph<int> &g);
void batchMode(Graph<int> &g);

int main() {
    char choice;
    Graph <int> g;
    do{
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch(choice){
            case '1':
                loadGraphFromFile(g);
                cout << "Graph has " << g.getNumVertex() << " vertices." << endl;
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

void displayMenu(){
    cout << "\n===== Route Planning Analysis Tool =====\n";
    cout << "1. Load Graph from File\n";
    cout << "2. Find Shortest Route\n";
    cout << "3. Batch Mode\n";
    cout << "4. Exit\n";
}



void findRoute(Graph<int> &g){
    string mode, source, destination;
    vector<int> path;

    cout << "Mode:";
    cin >> mode;
    cout << "Source:";
    cin >> source;
    cout << "Destination:";
    cin >> destination;
    int s= stoi(source);
    int d= stoi(destination);
    if(mode == "driving"){
        dijkstra(&g,s);
        path = getPath(&g, s, d);
        Vertex<int>* v = g.findVertex(d);
        double dist= v->getDist();
        if (path.empty()) cout << "none";
    else {
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i != path.size() - 1) cout << ","; 
        }
        cout << '('<<dist<<')'<<endl;
    }
    }else{
        if(mode == "driving-walking"){

        }else{
            cout << "Mode not valid!";
        }
    }
    
}

void batchMode(Graph<int> &g){

}