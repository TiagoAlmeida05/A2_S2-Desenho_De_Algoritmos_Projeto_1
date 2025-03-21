#include <iostream>

using namespace std;

void displayMenu();
void loadGraphFromFile();
void findRoute();
void batchMode();

int main() {
    char choice;

    do{
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch(choice){
            case '1':
                loadGraphFromFile();
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

void loadGraphFromFile(){

}

void findRoute(){

}

void batchMode(){

}