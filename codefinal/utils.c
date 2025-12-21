#include "utils.h"

//min and max functions
int max(int a, int b){
	if (a >= b){
		return a;
	}
	else{
		return b;
	}
}

int min(int a, int b){
	if (a <= b){
		return a;
	}
	else{
		return b;
	}
}

/* function that receives a string and return multiple strings where each one contains one colon
from the string */
char** getcolons(char string[]){
	char** colons = malloc (sizeof(char*)*5);
	char buffer[200];
	char* ptr;
	strcpy(buffer,string);
	if (colons == NULL){
		exit(1);
	}
	for (int i = 0; i < 5;i++){
		if (i == 0){
			colons[i] = malloc(sizeof(char)* 40);
			ptr = strtok(buffer,";");
			strcpy(colons[i],ptr);
		}
		else{
			colons[i] = malloc(sizeof(char)* 40);
			ptr = strtok(NULL,";");
			strcpy(colons[i],ptr);
		}
	}
	return colons;
}

//simple function to easily free the colons
void freecolons(char** colons){
    for (int i = 0; i < 5;i++){
        free(colons[i]);
    }
    free(colons);
}

// Compares two IDs for AVL ordering based on the part after '#'
int beforeinorderid(char str1[], char str2[]){
	int pos1 = 0,pos2 = 0;
	int difference = 0;
	while(str1[pos1] != '#'){
		pos1++;
	}
	while(str2[pos2] != '#'){
		pos2++;
	}
	while(difference == 0 && str1[pos1] != '\0' && str2[pos2] != '\0'){
		if (str1[pos1] < str2[pos2]){
			return 1;
		}
		else if (str1[pos1] > str2[pos2]){
			return -1;
		}
		else{
			pos1++;
			pos2++;
		}
	}
	return 0;
}

// Determines the type of line (plant, source, storage, distribution) from CSV
char* getlinetype(char string[]){
	char** colons = malloc (sizeof(char*)*5);
	colons = getcolons(string);
	int valsvide[5];
	
	if (colons == NULL){
		exit(1);
	}

	for (int i = 0; i < 5;i++){
		if (colons[i] != NULL && colons[i][0] == '-'){
			valsvide[i] = 1;
		}
		else{
			valsvide[i] = 0;
		}
	}
	if (valsvide[0] == 1 && valsvide[1] == 0 && valsvide[2] == 0
	&& valsvide[3] == 0 && valsvide[4] == 0){
		for(int i=0; i<5; i++) if(colons[i]) free(colons[i]);
		free(colons);
		return "source";
	}
	else if (valsvide[0] == 1 && valsvide[1] == 0 && valsvide[2] == 1
	&& valsvide[3] == 0 && valsvide[4] == 1){
		//createPlant(string);
		for(int i=0; i<5; i++) if(colons[i]) free(colons[i]);
		free(colons);
		return "plant";
	}
	else if (valsvide[0] == 1 && valsvide[1] == 0 && valsvide[2] == 0
	&& valsvide[3] == 1 && valsvide[4] == 0){
		for(int i=0; i<5; i++) if(colons[i]) free(colons[i]);
		free(colons);
		return "storage";
	}
	else {
		for(int i=0; i<5; i++) if(colons[i]) free(colons[i]);
		free(colons);
		return "distribution";
	}
}

// Searches for a Plant by ID in the AVL tree
Avl_Plant* searchAvlPlantById(Avl_Plant* root, char id[]){
	if (root == NULL){
		return NULL;
	}
	 else{
        int found = beforeinorderid(id,root->current->id);
        if (found == 0){
            return root;
        }
        else{
            if (found > 0){
                return searchAvlPlantById(root->ls,id);
            }
            else if (found < 0){
                return searchAvlPlantById(root->rs,id);
            }
            else{
                return NULL;
            }
        }
    }
}