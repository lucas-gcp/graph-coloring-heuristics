#ifndef GRAPH_H
#define GRAPH_H

#include <fstream>
#include <vector>

using namespace std;

class Graph {
    private:
        // unsigned n_vertices, n_edges;
        unsigned avg_d;

    public:
        Graph(ifstream &in_file);
        
        unsigned n_vertices, n_edges;
        vector<unsigned> degrees;
        vector<vector<unsigned>> neighbors;
        // vector<int> &get_neighbors(int v);

};

#endif