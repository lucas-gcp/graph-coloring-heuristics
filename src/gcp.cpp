#include "gcp.h"

#include <chrono>
#include <forward_list>
#include <iomanip>
#include <iostream>
#include <vector>

#include "graph.h"

using namespace std;

void delete_neighbors(Graph &G, forward_list<unsigned>::iterator vertex, forward_list<unsigned> &uncolored_vertices);

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
    vector<int> coloring(G.n_vertices, -1);

    for (int i = G.n_vertices - 1; i >= 0; i--) {
        uncolored.push_front(G.degrees[i].index);
    }

    while(!uncolored.empty()) {
        num_colors++;

        unsigned u = *uncolored.begin();
        // delete_neighbors(G, uncolored.begin(), uncolored);
        coloring[u] = num_colors-1;
        uncolored.pop_front();
        
        auto curr = uncolored.begin();
        auto prev = uncolored.before_begin();
        while (curr != uncolored.end()) {
            if (!G.adj_matrix[u][*curr]) {
                delete_neighbors(G, curr, uncolored);
                coloring[*curr] = num_colors-1;
                uncolored.erase_after(prev);
                curr = next(prev);
            } else {
                curr++;
                prev++;
            }
        }

        uncolored.clear();
        for (int i = G.n_vertices - 1; i >= 0; i--) {
            if (coloring[G.degrees[i].index] == -1){
                uncolored.push_front(G.degrees[i].index);
                // cout << G.degrees[i].degree << " ";
            }
        }

        // cout << "\nColor " << num_colors << endl;
        // for (auto i : uncolored)
        //     cout << i << " ";
        // cout << endl;
    }

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

void delete_neighbors(Graph &G, forward_list<unsigned>::iterator vertex, forward_list<unsigned> &uncolored_vertices) {
    auto curr = next(vertex);
    auto prev = vertex;
    while (curr != uncolored_vertices.end()) {
        if (G.adj_matrix[*vertex][*curr]) {
            uncolored_vertices.erase_after(prev);
            curr = next(prev);
        } else {
            curr++;
            prev++;
        }
    }
}