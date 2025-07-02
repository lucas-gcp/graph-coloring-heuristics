#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "graph.h"

using namespace std;

Graph::Graph(ifstream &in_file) {
    string line, buffer;
    do {
        getline(in_file, line);
    } while(line[0] != 'p');
    
    stringstream info(line);
    
    info >> buffer >> buffer >> n_vertices >> n_edges;
    cout << n_vertices << " " << n_edges << endl;;
    avg_d = n_edges/n_vertices;

    neighbors.assign(n_vertices, vector<unsigned>());
    degrees.assign(n_vertices, 0);
    
    for (unsigned i = 0; i < n_vertices; i++)
        neighbors[i].reserve(avg_d);

    char e;
    unsigned u, v;
    for (unsigned i = 0; i < n_edges; i++) {
        in_file >> e >> u >> v;
        neighbors[u-1].push_back(v-1);
        neighbors[v-1].push_back(u-1);
        degrees[u-1]++;
        degrees[v-1]++;
    }
}