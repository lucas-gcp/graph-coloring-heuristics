#include "gcp.h"

#include <chrono>
#include <iostream>
#include <iomanip>
#include <vector>

#include "graph.h"

using namespace std;

void timer(const ColoringFunctionT &coloring_fn, Graph &G) {
    auto t_start = chrono::high_resolution_clock::now();
    auto num_colors = coloring_fn(G);
    auto t_end = chrono::high_resolution_clock::now();

    cout << num_colors << endl;

    cout << fixed << setprecision(3) << chrono::duration<double, milli>(t_end - t_start).count() << endl;
}

unsigned first_fit(Graph &G) {
    int num_colors = 1;
    vector<int> coloring(G.n_vertices, -1);
    coloring[0] = 0;
    unsigned colored = 1;

    unsigned vertex = 1;
    int usable;
    while (colored != G.n_vertices) {
        for (int i = 0; i < num_colors; i++) {
            usable = 1;
            for (auto neighbor : G.neighbors[vertex]) {
                if (coloring[neighbor] == i) {
                    usable = 0;
                    break;
                }
            }
            if (usable) {
                coloring[vertex] = i;
                break;
            }
        }
        if (!usable) {
            coloring[vertex] = num_colors;
            num_colors++;
        }
        colored++;
        vertex++;
    }

    return num_colors;
}

unsigned welsh_powel(Graph &G) {
    
    return -1;
}

/*
unsigned ldo(Graph &G) {

}

unsigned ido(Graph &G) {

}

unsigned dsatur(Graph &G) {

}

unsigned rlf(Graph &G) {

}
*/