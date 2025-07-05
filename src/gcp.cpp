#include "gcp.h"

#include <chrono>
#include <forward_list>
#include <iomanip>
#include <iostream>
#include <vector>

#include "graph.h"

using namespace std;

void timer(const ColoringFunctionT &coloring_fn, Graph &G, double sort_time_consumption) {
    auto t_start = chrono::high_resolution_clock::now();
    auto num_colors = coloring_fn(G);
    auto t_end = chrono::high_resolution_clock::now();

    cout << num_colors << endl;

    cout << fixed << setprecision(3) << chrono::duration<double, milli>(t_end - t_start).count() + sort_time_consumption << endl;
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
    int num_colors = 0;
    forward_list<unsigned> uncolored;

    for (int i = G.n_vertices - 1; i >= 0; i--) {
        uncolored.push_front(G.degrees[i].index);
    }

    // for (auto u : uncolored) {
    //     auto uncolored_vertex = *uncolored.begin();

    //     for (auto v : uncolored) {
    //         if (G.adj_matrix[u][v] == 1) {
    //             // uncolored.remove(1);

    //             // v.remove()
    //         }
    //     }

    //     num_colors++;
    // }

    return num_colors;
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