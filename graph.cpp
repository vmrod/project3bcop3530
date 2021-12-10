#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <list>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>
#include <set>
#include <queue>
#include <chrono>
using namespace std;
using namespace std::chrono;

class WikiGraph
{
   private:
        // Graph Structure is Adjacency List using an Ordered Map of Lists
        unordered_map<int, list<int>> data;
        unsigned int numVertices = 1791489;

        vector<int> BFS_path;
        vector<int> DFS_path;
    public:
        string bfs_time;
        string iddfs_time;

        void insertEdge(int from, int to);  
        bool isEdge(int from, int to);   
        vector<string> getAdjacent(int vertex); 
        void printGraph();
        int graphVerts();
        vector<int> getDFSpath();
        vector<int> getBFSpath();
        void ClearPaths();
        void PerformSearches(unsigned int from, unsigned int to);
        vector<vector<string>> getPathInfo();

        void BFS(unsigned int from, unsigned int to);
        bool DFS(unsigned int from, unsigned int to, unsigned int limit);
        void IDDFS(unsigned int from, unsigned int to); 
};

void WikiGraph::insertEdge(int from, int to) 
{
    data[from].push_back(to);

    if(!data.count(to)) {
        data[to] = {};
    }

}

vector<string> WikiGraph::getAdjacent(int vertex) 
{
    vector<string> vertices;
    for(auto iter = data[vertex].begin(); iter != data[vertex].end(); iter++) {
        vertices.push_back(to_string(*iter));
        sort(vertices.begin(), vertices.end());
    }
    
    return vertices;
}

bool WikiGraph::isEdge(int from, int to) 
{
    bool edge = false;
    for(auto iter = data[from].begin(); iter != data[from].end(); iter++) {
        if(*iter == to) {
            edge = true;
            return edge;
        }
    }
    
    return edge;
}

void WikiGraph::printGraph()
{
    for (auto iter = data.begin(); iter != data.end(); ++iter) {
        cout << iter->first;
        for(auto iter_list = iter->second.begin(); iter_list != iter->second.end(); ++iter_list) {
            cout << " " << *iter_list;   
        } 
        cout << "" << endl;
    }
    
}

void WikiGraph::BFS(unsigned int from, unsigned int to) {
    queue<int> q;
    vector<bool> visited(numVertices);
    vector<int> distance(numVertices);
    vector<int> parent(numVertices);

    q.push(from);
    visited[from] = true;
    parent[from] = -1;

    while(!q.empty()) {
        unsigned int vertex = q.front();
        q.pop();

        if(!data.at(vertex).empty()) {
            for (auto i = data.at(vertex).begin(); i != data.at(vertex).end(); i++) {
                if (!visited[*i]) {
                    visited[*i] = true;
                    q.push(*i);
                    distance.at(*i) = distance.at(vertex) + 1;
                    parent.at(*i) = vertex;
                }
            }
        }
    }

    if(visited.at(to)) {
        for (unsigned int i = to; i != -1; i = parent.at(i)) {
           BFS_path.push_back(i); 
        }

        reverse(BFS_path.begin(), BFS_path.end());
        return;
    }

    cout << "No path to " << to << endl;
    return;

}

bool WikiGraph::DFS(unsigned int from, unsigned int to, unsigned int limit) {
    DFS_path.push_back(from);
    if(from == to) {
        return true;
    }
    if(limit <= 0) {
        return false;
    }

    for (auto iter = data[from].begin() ; iter != data[from].end(); ++iter) {
        if(DFS(*iter, to, limit - 1) == true) {
            return true;
        }
        DFS_path.pop_back();
    }
    
    return false;
}

void WikiGraph::IDDFS(unsigned int from, unsigned int to) {
    cout << "entered function" << endl;
    for (unsigned int i = 0; i < numVertices; i++) {
        cout << "depth : " << i << endl;
        if(DFS(from, to, i) == true) {
            return;
        }
        DFS_path.clear();
    }

    cout << "No Path Found" << endl;
}

int WikiGraph::graphVerts() {
    return this->numVertices;
} 

vector<int> WikiGraph::getDFSpath() {
    return DFS_path;
}

vector<int> WikiGraph::getBFSpath() {
    return BFS_path;
}

void WikiGraph::ClearPaths() {
    BFS_path.clear();
    DFS_path.clear();
}

void WikiGraph::PerformSearches(unsigned int from, unsigned int to) {
    cout << "Starting BFS: ";
    auto begin = high_resolution_clock::now();
    BFS(from, to);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - begin);
    this->bfs_time = to_string((float)duration.count() / 1000);

    cout << "Completed in " << this->bfs_time << "s" << endl;
    for(unsigned int i = 0; i < BFS_path.size(); i++) {
        cout << BFS_path.at(i) << " ";
    }
    cout << "" << endl;

    
    cout << "Starting IDDFS : ";
    auto begin_2 = high_resolution_clock::now();
    IDDFS(from, to);
    auto stop_2 = high_resolution_clock::now();
    auto duration_2 = duration_cast<milliseconds>(stop_2 - begin_2);
    this->iddfs_time = to_string((float)duration_2.count() / 1000);
        
    cout << "Completed in " << this->iddfs_time << "s" << endl;
    for(unsigned int i = 0; i < DFS_path.size(); i++) {
        cout << DFS_path.at(i) << " ";
    }
    cout << "" << endl;

}

vector<vector<string>> WikiGraph::getPathInfo() {
    vector<vector<string>> pathInfo;
    vector<string> timeInfo;
    
    // Insert Times into timeInfo, Then to pathInfo (0)
    timeInfo.push_back(bfs_time);
    timeInfo.push_back(iddfs_time);
    pathInfo.push_back(timeInfo);

    // Convert Shortest Path to Int, Then Insert Into pathInfo (1)
    vector<string> BFS_pathString;
    for(unsigned int i = 0; i < BFS_path.size(); i++) {
        BFS_pathString.push_back(to_string(BFS_path.at(i)));
        //cout << BFS_pathString.at(i) << " ";
    }
    //cout << "" << endl;
    pathInfo.push_back(BFS_pathString);

    // Get Adjacency For Every Node in Shortest Path, Insert Each as Vector into pathInfo
    for (unsigned int i = 0; i < BFS_path.size(); i++) {
        pathInfo.push_back(getAdjacent(BFS_path.at(i)));
    }

    return pathInfo;
}

void getDataCSV(string file, WikiGraph& graph) {
    ifstream inFile;
    inFile.open(file);
    if (inFile.is_open()) {
        string line;
        getline(inFile,line);
        int x = 0;
        while (getline(inFile,line)) {
            string token;
            stringstream stream(line);
            // if (x < 3) {
            //     x++;
            //     continue;
            // }
            int fromNode;
            int toNode;
            string tempFromNode;
            string tempToNode;

            getline(stream,tempFromNode,' ');
            fromNode = stoi(tempFromNode);
            getline(stream,tempToNode,' ');
            toNode = stoi(tempToNode);

            graph.insertEdge(fromNode, toNode);
            
        }
    }
    else {
        cout << "file cannot open" << endl;
    }
    
}

int main(int argc, char** argv) {
    WikiGraph graph;
    getDataCSV("adjlist.txt", graph);
    cout << "OPENED" << endl;

    string one = argv[1];
    string two = argv[2];

    cout << one << " " << two << endl;

    unsigned int start = stoi(one);
    unsigned int end = stoi(two);

    graph.PerformSearches(start, end);
    vector<vector<string>> info = graph.getPathInfo();

    string comp = "";

    for(unsigned int i = 0; i < info.size(); i++) {
            for (unsigned int j = 0; j < info.at(i).size(); j++) {
                comp += info.at(i).at(j) + " ";
            }
            comp += "|";
        }

    std::ofstream ofs;
    ofs.open("interface.txt", std::ofstream::out | std::ofstream::trunc);
    const char *c = comp.c_str();
    ofs.write(c, comp.size());
    ofs.close();

    return 0;
}