#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include "graph.h"
#include "gcp.h"

using namespace std;

int main(int argc, char* argv[]) {
    
    ifstream in_file;
    if (argc != 2) {
        cout << "Wrong args" << endl;
        return 1;
    }

    in_file.open(argv[1]);
    Graph G(in_file);
    
    auto t_start = chrono::high_resolution_clock::now();
    
    cout << first_fit(G) << endl;

    auto t_end = chrono::high_resolution_clock::now();
    cout << fixed << setprecision(3) << chrono::duration<double, milli>(t_end - t_start).count() << endl;

    in_file.close();
    return 0;
}
