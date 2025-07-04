#include <vector>
#include <iostream>
#include "graph.h"
#include "gcp.h"

using namespace std;

int first_fit(Graph G) {
    int num_colors = 1;
    vector<int> coloring(G.n_vertices, -1);
    coloring[0] = 0;
    unsigned colored = 1;

    unsigned vertex = 1;
    int usable;
    while(colored != G.n_vertices) {
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
/*
int welsh_powel(Graph G) {

}

int ldo(Graph G) {

}

int ido(Graph G) {

}

int dsatur(Graph G) {

}

int rlf(Graph G) {

}
*/