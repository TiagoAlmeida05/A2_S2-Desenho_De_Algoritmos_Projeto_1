#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include "Graph.h"

using namespace std;

void loadGraphFromFile(Graph <int> &g){
    string filename;
    string line;
    map<string, int> codeToId;

    cout << "Enter File Name With Nods:  \n";
    cin >> filename;

    ifstream inputfile(filename);

    if(!inputfile){
        cout <<"Error: Could not open " << filename << "!\n";
    }
    getline(inputfile, line);
    while (getline(inputfile, line)){
        stringstream ss(line);
        string location, idst, code, parkingst;
        
        getline(ss, location, ',');
        getline(ss, idst, ',');
        getline(ss, code, ',');
        getline(ss, parkingst, ',');
        
        int id = stoi(idst);
        bool parking = (parkingst == "1");
        g.addVertex(id);
        Vertex<int>* v = g.findVertex(id);

        v->setPark(parking);
        v->setCode(code);
        codeToId[code] = id;
    }
    cout << "Nods added!\n";
    cout << "Enter File Name With Nods:  \n";
    cin >> filename;
    

}
void displayMenu();
void findRoute();
void batchMode();

int main() {
    Graph <int> g;
    char choice;

    do{
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch(choice){
            case '1':
                loadGraphFromFile(g);
                break;
            case '2':
                findRoute();
                break;
            case '3':
                batchMode();
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

void findRoute(){

}

void batchMode(){

}