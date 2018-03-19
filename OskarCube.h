#ifndef OSKAR_OSKARCUBE_H
#define OSKAR_OSKARCUBE_H

#include <vector>
#include <list>
#include <memory>
#include <stack>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <regex>

using namespace std;
struct position
{
    int x,y,z;
    struct positionHasher {
        std::size_t operator()(const position &c) const;
    };
    bool operator==(const position& rhs)const;
};

class Node {
public:
    explicit Node(position pos);
    Node() = default;
    position pos{};
    Node *father = nullptr;
};

class OskarCube {
private:
    unordered_map<position, Node, position::positionHasher> nodesMap;
    list<Node*> queue;
    vector<vector<bool>> xy,yz,zx;
    position size;
    position origin, destination;

    void addNode(Node* n, position p);
    void getEmptyLine(ifstream &fin);
    position getQubeSize(ifstream& fin);
    vector<vector<bool>> getFace(ifstream& fin, int first, int second);
    bool validPosition(position p);

    int getFatherCommand(Node *pNode);

public:
    explicit OskarCube(string fileName, position origin, position destination);
    bool trySolve(); //run BFS, return true if destination found
    void getCommands(); //print commands for the path from origin to destination
};


#endif //OSKAR_OSKARCUBE_H
