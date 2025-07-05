#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "gcp.h"
#include "graph.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Wrong args" << endl;
        return 1;
    }

    ifstream in_file;
    in_file.open(argv[1]);
    Graph G(in_file);
    auto sort_time_consumption = G.sortDegrees();

    cout << " -=- First Fit -=- " << endl;
    timer(first_fit, G, 0);

    cout << " -=- Welsh Powel -=- " << endl;
    timer(welsh_powel, G, sort_time_consumption);

    in_file.close();
    return 0;
}
