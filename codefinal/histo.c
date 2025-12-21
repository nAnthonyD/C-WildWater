#include "histo.h"

int count_nodes(Avl_Plant* root){

	if(root == NULL){
		return 0;
		}
	return 1 + count_nodes(root->ls) + count_nodes(root->rs);
	}
	
	

void avl_to_tab_desc(const Avl_Plant* root, Plant* tab, int* index) {
    if (root == NULL) return;

    avl_to_tab_desc(root->rs, tab, index);

    tab[*index] = *(root->current);  
    (*index)++;

    avl_to_tab_desc(root->ls, tab, index);
}


	

int write_dat_file(const char* path, const Plant* tab, int n, HistoMode mode) {
    FILE* f = fopen(path, "w");
    if (f == NULL) {
        perror("fopen");
        return 1;
    }

    if (mode == H_MAX)  fprintf(f, "Identifier;Max volume (k.m3.year-1)\n");
    if (mode == H_SRC)  fprintf(f, "Identifier;Captured volume (k.m3.year-1)\n");
    if (mode == H_REAL) fprintf(f, "Identifier;Real treated volume (k.m3.year-1)\n");


    if (mode == H_ALL) {
        fprintf(f, "Identifier;Real treated (k.m3.year-1);Lost after capture (k.m3.year-1);Remaining possible (k.m3.year-1)\n");
    }

    for (int i = 0; i < n; i++) {
        if (mode != H_ALL) {
            float value = 0.0f;
            if (mode == H_MAX)  value = tab[i].max_cap;
            if (mode == H_SRC)  value = tab[i].col_vol;
            if (mode == H_REAL) value = tab[i].pro_vol;
            fprintf(f, "%s;%.3f\n", tab[i].id, value);
        }
        else {

            float real = tab[i].pro_vol;
            float lost = tab[i].col_vol - tab[i].pro_vol;
            float remaining = tab[i].max_cap - tab[i].col_vol;

            if (lost < 0) lost = 0;
            if (remaining < 0) remaining = 0;
            if (real < 0) real = 0;

            fprintf(f, "%s;%.3f;%.3f;%.3f\n", tab[i].id, real, lost, remaining);
        }
    }

    fclose(f);
    return 0;
}


int run_histo(const char* csv_path, HistoMode mode, const char* output_dat_path) {
    FILE* file = fopen(csv_path, "r");
    if (!file) {
        perror("fopen");
        return 1;
    }

    
    Avl_Plant* root = getAllPlantsFromFile(file);
    if (!root) {
        fclose(file);
        return 2;
    }

    
    getPlantColAndProcVolume(file, root);

   
    int n = count_nodes(root);
    if (n <= 0) {
        freeAvlPlant(root);
        fclose(file);
        return 3;
    }

    Plant* tab = malloc((size_t)n * sizeof(*tab));
    if (!tab) {
        freeAvlPlant(root);
        fclose(file);
        return 4;
    }

    int idx = 0;
    avl_to_tab_desc(root, tab, &idx);

    
    int err = write_dat_file(output_dat_path, tab, n, mode);

    free(tab);
    freeAvlPlant(root);
    fclose(file);
    return err;
}