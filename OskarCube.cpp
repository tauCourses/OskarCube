#include "OskarCube.h"

OskarCube::OskarCube(string fileName, position origin, position destination) : origin(origin), destination(destination)
{
    ifstream fin;
    fin.open(fileName, ifstream::in);
    if (!fin.good()) {
        cout << "unable to open file" << endl;
        throw fileName;
    }
    this->size = this->getQubeSize(fin);

    this->xy = this->getFace(fin,size.x,size.y); //get xy table
    getEmptyLine(fin);
    this->yz = this->getFace(fin,size.y,size.z); //get yz table
    getEmptyLine(fin);
    this->zx = this->getFace(fin,size.z,size.x); //get zx table
}

position OskarCube::getQubeSize(ifstream &fin) {
    string line;
    getline(fin, line);
    stringstream iss(line);
    int x,y,z;
    iss >> x;
    iss >> y;
    iss >> z;
    return {x,y,z};
}

void OskarCube::getEmptyLine(ifstream &fin)
{
    static auto isspaceLambda = [](unsigned char const c) { return std::isspace(c); };
    string line;
    getline(fin, line);
    if(!all_of(line.begin(),line.end(),isspaceLambda))
        throw "parse file failed - no empty line between two tables";
}

vector<vector<bool>> OskarCube::getFace(ifstream &fin, int first, int second) { //parse 2d boolean table
    vector<vector<bool>> face(static_cast<unsigned long>(second), vector<bool>(static_cast<unsigned long>(first)));
    string line;
    for(int i=0;i<second;i++)
    {
        getline(fin, line);
        if(line.empty())
            throw "parse file failed - puzzle size not fitting actual size";
        stringstream iss(line);
        istream_iterator<string> begin(iss), end;
        vector<string> vstrings(begin, end);

        if(vstrings.size() != first)
            throw "parse file failed - puzzle size not fitting actual size";
        for(int j=0; j<first; j++)
            face[i][j] = (vstrings[j] == "0");
    }

    return face;
}

bool OskarCube::trySolve() { // start BFS scanning from origin to destination. teminate when destination is found.

    if(!validPosition(origin) || !validPosition(destination))
        throw "origin or destination is not valid position";
    this->nodesMap[origin] = Node(origin); // create first node in the origin
    this->queue.push_back(&(this->nodesMap[origin])); //enter into the queue

    while(!queue.empty())
    {
        Node *s = queue.front();
        queue.pop_front();
        if(s->pos == destination)
            return true;

        this->addNode(s, {s->pos.x-1, s->pos.y, s->pos.z});
        this->addNode(s, {s->pos.x+1, s->pos.y, s->pos.z});
        this->addNode(s, {s->pos.x, s->pos.y-1, s->pos.z});
        this->addNode(s, {s->pos.x, s->pos.y+1, s->pos.z});
        this->addNode(s, {s->pos.x, s->pos.y, s->pos.z-1});
        this->addNode(s, {s->pos.x, s->pos.y, s->pos.z+1});
    }
    return false;
}

void OskarCube::getCommands() { //follows the track from destinationto origin with Node's father field. export to cout
    stack<int> commands;
    Node *current = &(this->nodesMap[this->destination]);
    while(!(current->pos == this->origin))
    {
        //cout << current->pos.x << " " << current->pos.y << " " << current->pos.z << endl;
        commands.push(getFatherCommand(current));
        current = current->father;
    }
    while(!commands.empty())
    {
        cout << commands.top() << " ";
        commands.pop();
    }
}

int OskarCube::getFatherCommand(Node *pNode) {
    if(pNode->father->pos.x != pNode->pos.x)
        return 0 + (pNode->father->pos.x == pNode->pos.x + 1);
    if(pNode->father->pos.y != pNode->pos.y)
        return 2 +  (pNode->father->pos.y == pNode->pos.y + 1);
    if(pNode->father->pos.z != pNode->pos.z)
        return 4 + (pNode->father->pos.z == pNode->pos.z + 1);

    cout << "problem!!!" << endl;
    return -1;
}

void OskarCube::addNode(Node* n, position p) {
    if(!validPosition(p)) //if position is not valid - return
        return;
    auto search = nodesMap.find(p);
    if(search != nodesMap.end()) //if node already exist - we already scan it in BFS no need to do anything
        return;
    nodesMap[p] = Node(p); //create a new Node with the new position in the unordermap
    Node* t = &(this->nodesMap[p]); //get node pointer
    queue.push_back(t); //add pointer to BFS queue
    t->father = n; //set node father the last node
}

bool OskarCube::validPosition(position p) { //check if a position is valid - inside the cube and not breaking any constrain
    return p.x>=0 && p.x < this->size.x &&
            p.y>=0 && p.y < this->size.y &&
            p.z>=0 && p.z < this->size.z &&
            xy[p.y][p.x] && yz[p.z][p.y] && zx[p.x][p.z];
}

std::size_t position::positionHasher::operator()(const position &c) const { //position hash function. for unorder map
    size_t res = hash<int>()(c.x);
    res = res * 31 + hash<int>()(c.y);
    res = res * 31 + hash<int>()(c.z);

    return res;
}

Node::Node(position pos) : pos(pos){
}

bool position::operator==(const position &rhs) const { //compare between two positions
    return this->x==rhs.x && this->y == rhs.y && this->z == rhs.z;
}
