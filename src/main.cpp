#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "gcp.h"
#include "graph.h"

using namespace std;
namespace fs = filesystem;

using ColoringFunctionT = unsigned (*)(Graph &G);
void timer(const ColoringFunctionT &coloring_fn, Graph &G, double sort_time_consumption, int &num_colors, double &time);

void run_graph_benchmark(fs::path graph_filepath, ofstream &benchmark_csv);

int main(int argc, char* argv[]) {
    if (argc > 2) {
        cout << "Wrong args" << endl;
        return 1;
    }

    fs::path bin_path(argv[0]);
    fs::path root_dir = fs::weakly_canonical(bin_path.parent_path()).parent_path();
    fs::path benchmark_dir = root_dir / "benchmark";
    fs::path out_dir = root_dir / "out";

    if(!fs::exists(out_dir))
        fs::create_directory(out_dir);

    if (argc == 2) {
        ofstream benchmark_csv(out_dir / "single-runs.csv", ofstream::app);
        fs::path graph_filepath = argv[1];

        run_graph_benchmark(graph_filepath, benchmark_csv);
        
        return 0;
    }

    ofstream benchmark_csv(out_dir / "benchmark.csv");

    benchmark_csv << "graph_name,vertices,edges,"
                  << "ff_sol,ff_time,"
                  << "wp_sol,wp_time,"
                  << "ldo_sol,ldo_time,"
                  << "ido_sol,ido_time,"
                  << "dsatur_sol,dsatur_time,"
                  << "rlf_sol,rlf_time" << endl;

    for (auto const& graph_entry : fs::directory_iterator{benchmark_dir}) {
        run_graph_benchmark(graph_entry.path(), benchmark_csv);
    }

    benchmark_csv.close();

    return 0;
}

void run_graph_benchmark(fs::path graph_filepath, ofstream &benchmark_csv) {
    string::iterator name_end, name_start;
    string graph_filename = graph_filepath.filename().string();
    for (name_end = graph_filename.end(); *name_end != '.'; name_end--);
    
    string graph_name(graph_filename.begin(), name_end);
    cout << "\n=================" << endl;
    cout << graph_name << endl;

    ifstream in_file;
    in_file.open(graph_filepath);
    Graph G(in_file);
    auto sort_time_consumption = G.sortDegrees();

    int num_colors;
    double time;

    benchmark_csv << graph_name << "," << G.n_vertices << "," << G.n_edges << ","; 

    // cout << "First Fit: ";
    // timer(first_fit, G, 0, num_colors, time);
    // benchmark_csv << num_colors << "," << time << ",";

    // cout << "Welsh Powel: ";
    // timer(welsh_powel, G, sort_time_consumption, num_colors, time);
    // benchmark_csv << num_colors << "," << time << ",";

    // cout << "Largest Degree Ordering: ";
    // timer(ldo, G, sort_time_consumption, num_colors, time);
    // benchmark_csv << num_colors << "," << time << ",";

    // cout << "Incidence Degree Ordering: ";
    // timer(ido, G, sort_time_consumption, num_colors, time);
    // benchmark_csv << num_colors << "," << time << ",";

    // cout << "Incidence Degree Ordering 2: ";
    // timer(ido1, G, sort_time_consumption, num_colors, time);
    // benchmark_csv << num_colors << "," << time << ",";

    cout << "Degree of Saturation: ";
    timer(dsatur, G, sort_time_consumption, num_colors, time);
    benchmark_csv << num_colors << "," << time << ",";

    cout << "Degree of Saturation 2: ";
    timer(dsatur1, G, sort_time_consumption, num_colors, time);
    benchmark_csv << num_colors << "," << time << ",";

    // cout << "Recursive Largest First: ";
    // timer(rlf, G, sort_time_consumption, num_colors, time);
    // benchmark_csv << num_colors << "," << time << ",";

    benchmark_csv << endl;
    
    in_file.close();
}

void timer(const ColoringFunctionT &coloring_fn, Graph &G, double sort_time_consumption, int &num_colors, double &time) {
    auto t_start = chrono::high_resolution_clock::now();
    num_colors = coloring_fn(G);
    auto t_end = chrono::high_resolution_clock::now();

    time = chrono::duration<double, milli>(t_end - t_start).count() + sort_time_consumption;

    cout << num_colors << " ";

    cout << fixed << setprecision(3) << time << endl;
}
