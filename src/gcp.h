#ifndef GCP_H
#define GCP_H

#include "graph.h"

using ColoringFunctionT = unsigned (*)(Graph &G);

void timer(const ColoringFunctionT &coloring_fn, Graph &G);

unsigned first_fit(Graph &G);

unsigned welsh_powel(Graph &G);

unsigned ldo(Graph &G);

unsigned ido(Graph &G);

unsigned dsatur(Graph &G);

unsigned rlf(Graph &G);

#endif