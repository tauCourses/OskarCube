#include <iostream>
#include "OskarCube.h"

int main(int argc, char** argv) {
    try {
        position origin = {std::stoi(argv[1]),std::stoi(argv[2]), std::stoi(argv[3])};
        position dest = {std::stoi(argv[4]),std::stoi(argv[5]), std::stoi(argv[6])};
        if(origin == dest)
            return 0;
        OskarCube cube(argv[7], origin, dest);
        if(cube.trySolve())
            cube.getCommands();
        else
            cout << "no solution found!" << endl;
    }
    catch (const char* c){
        cout << c << endl;
        return -1;
    }
    return 0;
}