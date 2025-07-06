import os
import pandas as pd
import matplotlib.pyplot as plt

root_dir = os.path.dirname(os.path.abspath(__file__))

benchmark = pd.read_csv(os.path.join(root_dir, "out", "benchmark.csv"))

pt_cols = {
    'graph_name': 'Grafo',
    'vertices': 'Vértices',
    'edges': 'Arestas',
    'ff_sol': 'FF K',
    'ff_time': 'FF Tempo (ms)',
    'wp_sol': 'WP K',
    'wp_time': 'WP Tempo (ms)',
    'ldo_sol': 'LDO K',
    'ldo_time': 'LDO Tempo (ms)',
    'ido_sol': 'IDO K',
    'ido_time': 'IDO Tempo (ms)',
    'dsatur_sol': 'DSATUR K',
    'dsatur_time': 'DSATUR Tempo (ms)',
    'rlf_sol': 'RLF K',
    'rlf_time': 'RLF Tempo (ms)',
}

latex_benchmark = benchmark.rename(columns=pt_cols)

print(latex_benchmark)

with open(os.path.join(root_dir, "manuscript", "results_tab.tex"), 'w') as f:
    f.write(latex_benchmark.style.format(precision=2).hide(axis="index").to_latex())
