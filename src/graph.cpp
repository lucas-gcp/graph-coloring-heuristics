#include "graph.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

Graph::Graph(ifstream &in_file) {
    string line, buffer;
    do {
        getline(in_file, line);
    } while (line[0] != 'p');

    stringstream info(line);

    info >> buffer >> buffer >> n_vertices >> n_edges;
    cout << n_vertices << " " << n_edges << endl;
    avg_d = n_edges / n_vertices;

    neighbors.assign(n_vertices, vector<unsigned>());
    degrees.reserve(n_vertices);

    for (unsigned i = 0; i < n_vertices; i++) {
        VertexDegree vertexDegree;
        vertexDegree.index = i;
        vertexDegree.degree = 0;

        degrees.push_back(vertexDegree);
    }

    for (unsigned i = 0; i < n_vertices; i++)
        neighbors[i].reserve(avg_d);

    char e;
    unsigned u, v;
    for (unsigned i = 0; i < n_edges; i++) {
        in_file >> e >> u >> v;
        neighbors[u - 1].push_back(v - 1);
        neighbors[v - 1].push_back(u - 1);
        degrees[u - 1].degree++;
        degrees[v - 1].degree++;
    }
}

double Graph::sortDegrees() {
    auto t_start = chrono::high_resolution_clock::now();
    sort(degrees.begin(), degrees.end(), [](VertexDegree u, VertexDegree v) { return u.degree > v.degree; });
    auto t_end = chrono::high_resolution_clock::now();

    return chrono::duration<double, milli>(t_end - t_start).count();
}