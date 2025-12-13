#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "histo.h"


static int count_nodes(const AVL* root){

	if(root == NULL){
		return 0;
		}
	return 1 + count_nodes(root->left) + count_nodes(root->right);
	}
	
	

static void avl_to_tab(AVL* root, Usine* tab, int* index){

	if (root == NULL){
		return;
		}
		
	
	avl_to_tab(root->right, tab, index);
		
	tab[*index] = root->data;
	(*index)++;
		
	avl_to_tab(root->left, tab, index);
		
	}
	
	
static int dat_file(const char *path, const Usine *tab, int n, HistoMode mode) {


	FILE* f = fopen(path, "w");
	
	if (f == NULL){
		perror("fopen");
		return 1;
	}
		
	switch(mode){
	
		case H_MAX:
			fprintf(f, "Identifiant ; Volume Max (1000 m3/an)\n");
			break;
			
		case H_SRC:
			fprintf(f, "Identifiant ; Volume Capté (1000 m3/an)\n");
			break;
			
		case H_REAL:
			fprintf(f, "Identifiant ; Volume Traité (1000 m3/an)\n");
			break;
			
		}
			
			
	for (int i = 0; i < n; i++) {
        long value = 0;

        if (mode == H_MAX)
            value = arr[i].capacite_max;
        else if (mode == H_SRC)
            value = arr[i].volume_src;
        else if (mode == H_REAL)
            value = arr[i].volume_real;

        fprintf(f, "%s;%ld\n", arr[i].id, value);
    }

    fclose(f);
    return 0;
}
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
