#include "Graph.h"
#include "Epidemic.h"

int main(){
    bool temp[] = {0,1,0,1,0,1};
    vector<bool> bitspray(temp, temp + sizeof(temp) / sizeof(bool));
    for (int i = 0; i < bitspray.size(); i++){
        cout << bitspray.at(i)  << " ";
    }
    cout<<endl;

    Graph network;
    network.initialize(4, bitspray);
    network.printAdjLists();
    return (0);
}