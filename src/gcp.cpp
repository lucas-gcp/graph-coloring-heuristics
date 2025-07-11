#include "gcp.h"

#include <forward_list>
#include <iostream>
#include <list>
#include <vector>

#include "graph.h"

using namespace std;

void delete_neighbors(Graph &G, forward_list<unsigned>::iterator vertex, forward_list<unsigned> &uncolored_vertices);

void sorted_insertion(VertexDegree vertex, list<VertexDegree> &U);

list<VertexDegree>::iterator max_degree_in_subgraph(Graph &G, list<VertexDegree> &origin, list<VertexDegree> &dest);

unsigned first_fit(Graph &G) {
    int num_colors = 1;
    vector<int> coloring(G.n_vertices, -1);
    coloring[0] = 0;
    unsigned n_colored = 1;

    unsigned vertex = 1;
    bool usable_color;
    while (n_colored != G.n_vertices) {
        for (int i = 0; i < num_colors; i++) {
            usable_color = true;
            for (auto neighbor : G.neighbors[vertex]) {
                if (coloring[neighbor] == i) {
                    usable_color = false;
                    break;
                }
            }
            if (usable_color) {
                coloring[vertex] = i;
                break;
            }
        }
        if (!usable_color) {
            coloring[vertex] = num_colors;
            num_colors++;
        }
        n_colored++;
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

    while (!uncolored.empty()) {
        num_colors++;

        unsigned u = *uncolored.begin();
        coloring[u] = num_colors - 1;
        uncolored.pop_front();

        auto curr = uncolored.begin();
        auto prev = uncolored.before_begin();
        while (curr != uncolored.end()) {
            if (!G.adj_matrix[u][*curr]) {
                delete_neighbors(G, curr, uncolored);
                coloring[*curr] = num_colors - 1;
                uncolored.erase_after(prev);
                curr = next(prev);
            } else {
                curr++;
                prev++;
            }
        }

        uncolored.clear();
        for (int i = G.n_vertices - 1; i >= 0; i--) {
            if (coloring[G.degrees[i].index] == -1)
                uncolored.push_front(G.degrees[i].index);
        }
    }

    return num_colors;
}

unsigned ldo(Graph &G) {
    vector<int> coloring(G.n_vertices, -1);
    list<VertexDegree> uncolored;
    for (VertexDegree v : G.degrees)
        uncolored.push_back(v);

    int num_colors = 0;

    while (!uncolored.empty()) {
        auto curr = uncolored.begin();
        bool appropriate_color = false;
        for (int i = 0; i < num_colors; i++) {
            appropriate_color = true;
            for (auto v : G.neighbors[curr->index]) {
                if (coloring[v] == i) {
                    appropriate_color = false;
                    break;
                }
            }
            if (appropriate_color) {
                coloring[curr->index] = i;
                break;
            }
        }
        if (!appropriate_color) {
            coloring[curr->index] = num_colors;
            num_colors++;
        }
        uncolored.erase(curr);
    }

    return num_colors;
}

unsigned ido(Graph &G) {
    vector<int> coloring(G.n_vertices, -1);
    vector<int> colored_neighbors_count(G.n_vertices, 0);

    list<VertexDegree> uncolored(G.degrees.begin(), G.degrees.end());

    int num_colors = 1;

    coloring[(uncolored.begin())->index] = 0;
    for (auto u : G.neighbors[(uncolored.begin())->index])
        colored_neighbors_count[u]++;

    uncolored.erase(uncolored.begin());

    while (!uncolored.empty()) {
        list<VertexDegree>::iterator max_vertex;
        int max_neighbors = -1;
        for (auto u = uncolored.begin(); u != uncolored.end(); u++) {
            if (colored_neighbors_count[u->index] > max_neighbors) {
                max_vertex = u;
                max_neighbors = colored_neighbors_count[u->index];
            }
        }

        bool appropriate_color;
        for (int i = 0; i < num_colors; i++) {
            appropriate_color = true;
            for (auto u : G.neighbors[max_vertex->index]) {
                if (coloring[u] == i) {
                    appropriate_color = false;
                    break;
                }
            }
            if (appropriate_color) {
                coloring[max_vertex->index] = i;
                break;
            }
        }
        if (!appropriate_color) {
            coloring[max_vertex->index] = num_colors;
            num_colors++;
        }

        for (auto u : G.neighbors[max_vertex->index])
            colored_neighbors_count[u]++;

        uncolored.erase(max_vertex);
    }
    return num_colors;
}

unsigned dsatur(Graph &G) {
    list<VertexDegree> uncolored_vertices(G.degrees.begin(), G.degrees.end());
    vector<unsigned> num_neighboring_colors(G.n_vertices, 0);
    vector<int> coloring(G.n_vertices, -1);

    int num_colors = 1;

    coloring[(uncolored_vertices.begin())->index] = 0;
    for (auto u : G.neighbors[(uncolored_vertices.begin())->index])
        num_neighboring_colors[u]++;

    uncolored_vertices.erase(uncolored_vertices.begin());

    while (!uncolored_vertices.empty()) {
        list<VertexDegree>::iterator max_vertex = uncolored_vertices.begin();
        unsigned max_neighbors = -1;
        for (auto u = uncolored_vertices.begin(); u != uncolored_vertices.end(); u++) {
            if (num_neighboring_colors[u->index] > max_neighbors) {
                max_vertex = u;
                max_neighbors = num_neighboring_colors[u->index];
            }
        }

        bool appropriate_color;
        int i;
        for (i = 0; i < num_colors; i++) {
            appropriate_color = true;
            for (auto u : G.neighbors[max_vertex->index]) {
                if (coloring[u] == i) {
                    appropriate_color = false;
                    break;
                }
            }
            if (appropriate_color) {
                break;
            }
        }
        if (!appropriate_color) {
            num_colors++;
            for (auto u : G.neighbors[max_vertex->index])
                num_neighboring_colors[u]++;
        } else {
            bool adjacent_color;
            for (auto u : G.neighbors[max_vertex->index]) {
                adjacent_color = false;
                for (auto v : G.neighbors[u]) {
                    if (coloring[v] == i) {
                        adjacent_color = true;
                        break;
                    }
                }
                if (!adjacent_color)
                    num_neighboring_colors[u]++;
            }
        }
        coloring[max_vertex->index] = i;
        uncolored_vertices.erase(max_vertex);
    }
    return num_colors;
}

unsigned rlf(Graph &G) {
    unsigned num_colored_vertices = 0;
    unsigned num_colors = 0;

    list<VertexDegree> V_;
    list<VertexDegree> U;
    for (int i = G.n_vertices - 1; i >= 0; i--)
        V_.push_front(G.degrees[i]);

    while (true) {
        auto k = max_degree_in_subgraph(G, V_, V_);
        num_colors++;

        while (true) {
            auto curr = V_.begin();

            while (curr != V_.end()) {
                if (G.adj_matrix[k->index][curr->index] == 1) {
                    sorted_insertion(*curr, U);
                    V_.erase(prev(++curr));
                } else {
                    curr++;
                }
            }

            V_.erase(k);
            num_colored_vertices++;

            if (!V_.empty()) {
                k = max_degree_in_subgraph(G, V_, U);
                continue;
            }

            if (num_colored_vertices == G.n_vertices)
                return num_colors;

            for (auto u : U)
                V_.push_front(u);

            U.clear();
            break;
        }
    }
}

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

void sorted_insertion(VertexDegree vertex, list<VertexDegree> &U) {
    if (U.empty()) {
        U.push_front(vertex);
        return;
    }

    list<VertexDegree>::iterator u;
    for (u = U.begin(); u != U.end(); u++) {
        if (u->degree > vertex.degree) {
            break;
        }
    }

    U.emplace(u, vertex);
}

list<VertexDegree>::iterator max_degree_in_subgraph(Graph &G,
                                                    list<VertexDegree> &origin,
                                                    list<VertexDegree> &dest) {
    int max_degree = -1;
    list<VertexDegree>::iterator k;

    for (auto v = origin.begin(); v != origin.end(); v++) {
        int curr_degree = 0;

        for (auto u : dest) {
            if (G.adj_matrix[v->index][u.index])
                curr_degree++;
        }

        if (curr_degree > max_degree) {
            max_degree = curr_degree;
            k = v;
        }
    }

    return k;
}