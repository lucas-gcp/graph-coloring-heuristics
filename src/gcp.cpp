#include "gcp.h"

#include <chrono>
#include <forward_list>
#include <iomanip>
#include <iostream>
#include <vector>

#include "graph.h"

using namespace std;

void delete_neighbors(Graph &G, forward_list<unsigned>::iterator vertex, forward_list<unsigned> &uncolored_vertices);

void sorted_insertion(VertexDegree vertex, forward_list<VertexDegree> &U);

forward_list<VertexDegree>::iterator max_degree_in_subgraph(Graph &G, forward_list<VertexDegree> &origin, forward_list<VertexDegree> &dest);

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
        // delete_neighbors(G, uncolored.begin(), uncolored);
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
            if (coloring[G.degrees[i].index] == -1) {
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

unsigned welsh_powel_2(Graph &G) {
    int active_color = 0;
    vector<int> coloring(G.n_vertices, -1);

    vector<VertexDegree> V_line(G.degrees);
    V_line.reserve(G.n_vertices);

    while (!V_line.empty()) {
        // Step 2: The uncolored vertex that has the largest degree in the degree set 𝐷𝑒𝑔(𝑣𝑖) is selected for coloring
        VertexDegree uncolored_vertex = V_line[0];

        // Step 3: The selected vertex is colored with active color
        coloring[uncolored_vertex.index] = active_color;
        V_line.erase(V_line.begin());

        // Step 3: After that, find the uncolored vertices from adjacency matrix which are not adjacent vertices of the colored vertex and these vertices are added to the 𝑉′ set ( 𝑉′ = {𝑣′1, 𝑣′2, … . , 𝑣′𝑛} ).
        for (int neighbor_idx : G.neighbors[uncolored_vertex.index]) {
            int V_line_size = V_line.size();

            for (int i = 0; i < V_line_size; i++) {
                if (V_line[i].index == neighbor_idx) {
                    V_line.erase(V_line.begin() + i);
                    break;
                }
            }
        }

        // The uncolored vertex that has the largest degree in the 𝑉 is selected for coloring. This vertex is colored with active color. After that, the adjacent vertices of the this vertex deleted from 𝑉′. This step is repeated until all vertices colored in the set of 𝑉′.
        while (!V_line.empty()) {
            VertexDegree largest_degree = V_line[0];
            coloring[largest_degree.index] = active_color;
            V_line.erase(V_line.begin());

            for (int neighbor_idx : G.neighbors[largest_degree.index]) {
                int V_line_size = V_line.size();

                for (int i = 0; i < V_line_size; i++) {
                    if (V_line[i].index == neighbor_idx) {
                        V_line.erase(V_line.begin() + i);
                        break;
                    }
                }
            }
        }

        // Reconstruct V_line as a fucking joke
        for (int i = 0; i < G.degrees.size(); i++) {
            int i_index = G.degrees[i].index;

            if (coloring[i_index] == -1) {
                V_line.push_back(G.degrees[i]);
            }
        }

        active_color++;
    }

    cout << endl;

    return active_color;
}

unsigned ldo(Graph &G) {
    int num_colors = 1;
    vector<int> coloring(G.n_vertices, -1);
    coloring[G.degrees[0].index] = 0;

    bool usable_color;
    for (unsigned u = 1; u < G.n_vertices; u++) {
        unsigned index = G.degrees[u].index;
        for (int i = 0; i < num_colors; i++) {
            usable_color = true;
            for (auto v : G.neighbors[index]) {
                if (coloring[v] == i) {
                    usable_color = false;
                    break;
                }
            }
            if (usable_color) {
                coloring[index] = i;
                break;
            }
        }
        if (!usable_color) {
            coloring[index] = num_colors;
            num_colors++;
        }
    }
    return num_colors;
}

unsigned ido(Graph &G) {
    int num_colors = 0;
    forward_list<VertexDegree> uncolored_vertices;
    vector<unsigned> colored_neighbors(G.n_vertices);
    vector<int> coloring(G.n_vertices, -1);

    for (int i = G.n_vertices - 1; i >= 0; i--) {
        uncolored_vertices.push_front(G.degrees[i]);
    }

    while(!uncolored_vertices.empty()) {
        auto curr = uncolored_vertices.begin();
        auto prev = uncolored_vertices.before_begin();
        
        auto selected = curr;
        auto before_selected = prev;

        while (curr != uncolored_vertices.end()) {
            if (colored_neighbors[curr->index] > colored_neighbors[selected->index]) {
                selected = curr;
                before_selected = prev;
            }
            curr++;
            prev++;
        }

        bool usable_color = false;
        for (int i = 0; i < num_colors; i++) {
            usable_color = true;
            for (auto neighbor : G.neighbors[selected->index]) {
                if (coloring[neighbor] == i) {
                    usable_color = false;
                    break;
                }
            }
            if (usable_color) {
                coloring[selected->index] = i;
                break;
            }
        }
        if (!usable_color) {
            coloring[selected->index] = num_colors;
            num_colors++;
        }

        for (auto u : G.neighbors[selected->index]) {
            colored_neighbors[u]++;
        }

        uncolored_vertices.erase_after(before_selected);
    }

    return num_colors;
}

unsigned dsatur(Graph &G) {
    int num_colors = 0;
    forward_list<VertexDegree> uncolored_vertices;
    vector<unsigned> num_neighboring_colors(G.n_vertices);
    vector<int> coloring(G.n_vertices, -1);

    for (int i = G.n_vertices - 1; i >= 0; i--) {
        uncolored_vertices.push_front(G.degrees[i]);
    }

    while(!uncolored_vertices.empty()) {
        auto curr = uncolored_vertices.begin();
        auto prev = uncolored_vertices.before_begin();
        
        auto selected = curr;
        auto before_selected = prev;

        while (curr != uncolored_vertices.end()) {
            if (num_neighboring_colors[curr->index] > num_neighboring_colors[selected->index]) {
                selected = curr;
                before_selected = prev;
            }
            curr++;
            prev++;
        }

        bool usable_color = false;
        for (int i = 0; i < num_colors; i++) {
            usable_color = true;
            for (auto neighbor : G.neighbors[selected->index]) {
                if (coloring[neighbor] == i) {
                    usable_color = false;
                    break;
                }
            }
            if (usable_color) {
                coloring[selected->index] = i;
                break;
            }
        }
        if (!usable_color) {
            coloring[selected->index] = num_colors;
            num_colors++;
            for (auto u : G.neighbors[selected->index])
                num_neighboring_colors[u]++;
        }

        uncolored_vertices.erase_after(before_selected);
    }

    return num_colors;
}

unsigned rlf(Graph &G) {
    // 1. Set C := ∅, V ′ := V , U := ∅, q := 0.
    unsigned num_colored_vertices = 0;
    unsigned num_colors = 0;

    forward_list<VertexDegree> V_;
    forward_list<VertexDegree> U;
    for (int i = G.n_vertices - 1; i >= 0; i--)
        V_.push_front(G.degrees[i]);

        
    while (true) {
        auto before_k = max_degree_in_subgraph(G, V_, V_);
        auto k = next(before_k);
        num_colors++;

        while (true) {
            // cout << (k == V_.end()) << endl;
            auto curr = V_.begin();
            auto prev = V_.before_begin();
            while (curr != V_.end()) {
                if (G.adj_matrix[k->index][curr->index]) {
                    sorted_insertion(*curr, U);
                    V_.erase_after(prev);
                    curr = next(prev);
                } else {
                    curr++;
                    prev++;
                }
            }
            V_.erase_after(before_k);
            num_colored_vertices++;

            if (!V_.empty()) {
                max_degree_in_subgraph(G, V_, U);
                cout << (before_k == V_.end()) << endl;
                k = next(before_k);
                continue;
            }

            // cout << num_colors << endl;

            if (num_colored_vertices == G.n_vertices)
                return num_colors;

            for (auto u : U)
                V_.push_front(u);

            U.clear();
            cout << "=2=" << endl;
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

void sorted_insertion(VertexDegree vertex, forward_list<VertexDegree> &U) {
    if (U.empty()) {
        U.push_front(vertex);
        return;
    }
    
    forward_list<VertexDegree>::iterator u, prev;
    for (u = U.begin(), prev = U.before_begin(); u != U.end(); u++, prev++) {
        if (u->degree > vertex.degree) {
            break;
        }
    }
    U.emplace_after(prev, vertex);
}

forward_list<VertexDegree>::iterator
max_degree_in_subgraph(Graph &G,
                       forward_list<VertexDegree> &origin,
                       forward_list<VertexDegree> &dest)
{
    int max_degree = -1;
    forward_list<VertexDegree>::iterator before_k;
    for (auto v = origin.begin(), v_prev = origin.before_begin(); v != origin.end(); v++, v_prev++) {
        int curr_degree = 0;
        for (auto u : dest) {
            if (G.adj_matrix[v->index][u.index])
                curr_degree++;
        }
        if (curr_degree > max_degree) {
            max_degree = curr_degree;
            // cout << k->index << endl;
            before_k = v_prev;
        }
    }
    return before_k;
}