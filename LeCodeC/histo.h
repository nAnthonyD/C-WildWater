#ifndef HISTO_H
#define HISTO_H

#include "managefiles.h"

typedef enum {
    H_MAX,
    H_SRC,
    H_REAL
} HistoMode;

int count_nodes(Avl_Plant* root);
void avl_to_tab_desc(const Avl_Plant* root, Plant* tab, int* index);
int write_dat_file(const char* path, const Plant* tab, int n, HistoMode mode);
int run_histo(const char* csv_path, HistoMode mode, const char* output_dat_path);

#endif // HISTO_H