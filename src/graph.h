#ifndef GRAPH_H
#define GRAPH_H

#include <fstream>
#include <vector>

using namespace std;

struct {
    unsigned index;
    unsigned degree;
} typedef VertexDegree;

class Graph {
   private:
    // unsigned n_vertices, n_edges;
    unsigned avg_d;

   public:
    Graph(ifstream &in_file);

    double sortDegrees();

    unsigned n_vertices, n_edges;
    vector<vector<unsigned>> adj_matrix;
    vector<vector<unsigned>> neighbors;
    vector<VertexDegree> degrees;

    double sort_time;
};

#endif