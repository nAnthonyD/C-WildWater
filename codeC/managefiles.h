#ifndef STDIO
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#define STDIO
//#define FICHIER "c-wildwater_v0(1).dat"
#endif

/* To Do: 
Balance the Avl tree when inserting plants
Functions to browse the file and set the plant's collected volume and processed volume
*/

typedef struct _plant{
	char id[40];
	int max_cap;
	int col_vol;
	int pro_vol;
}Plant;

typedef struct _avl_plant{
	Plant* current;
	struct _avl_plant* ls;
	struct _avl_plant* rs;
	int balance;
}Avl_Plant;

typedef struct _linked_plant{
	Plant* current;
	struct _linked_plant* next;
}Linked_Plant;

Avl_Plant* createAvlPlant(Plant* plant){
	Avl_Plant* newone = malloc (sizeof(Avl_Plant));
	if (newone == NULL){
		exit(1);
	}
	newone->current = plant;
	newone->ls = NULL;
	newone->rs = NULL;
	newone->balance = 0;
	return newone;
}

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
	/*while(str1[pos] != '\0' && str2[pos] != '\0'){
		if (str1[pos] < str2[pos]){
			return 1;
		}
		else if (str1[pos] > str2[pos]){
			return 0;
		}
		pos++;
	}
	if (str1[pos] == '\0' && str2[pos] != '\0'){
		return 1;
	}
	else{
		return 0;
	}*/
}


void stringtotable(char source[],char destiny[],int size){
	int pos = 0;
	while(source[pos] != '\n' && source[pos] != '\0' && pos < size){
		destiny[pos] = source[pos];
	}	
}

Plant* createPlant(char string[]){
	Plant* newone = malloc(sizeof(Plant));
	if (newone == NULL){
		exit(1);
	}
	int pos = 0;
	char cap[40];
	for (int i = 0; i <= 5;i++){
		if (i == 1){
		int j = 0;
			while(string[pos] != ';' && string[pos] != '\n' && string[pos] != '\0'){
					newone->id[j] = string[pos];
					//printf("%c",string[pos]);
					j++;
					pos++;
			}
		}
		else if (i == 3){
			int j = 0;
			while(string[pos] != ';' && string[pos] != '\n' && string[pos] != '\0'){
						cap[j] = string[pos];
						j++;
						pos++;
				}
			newone->max_cap = atoi(cap);
		}
		if (string[pos] == '\n' || string[pos] == '\0'){
			i = 6;
		}
		while(string[pos] != ';'){
			pos++;
		}
		pos++;
	}
	return newone;
}

char* getlinetype(char string[]){
	int pos = 0;
	char aswer[200];
	int valsvide[5];
	
	for (int i = 0; i <= 5;i++){
		if (string[pos] == '-'){
			valsvide[i] = 1;
		}
		else{
			valsvide[i] = 0;
		}
		if (string[pos] == '\n' || string[pos] == '\0' ){
			i = 6;
		}
		while(string[pos] != ';' && string[pos] != '\n' && string[pos] != '\0'){
			pos++;
		}
		pos++;
	}
	if (valsvide[0] == 1 && valsvide[1] == 0 && valsvide[2] == 0
	&& valsvide[3] == 0 && valsvide[4] == 0){
		return "source";
	}
	else if (valsvide[0] == 1 && valsvide[1] == 0 && valsvide[2] == 1
	&& valsvide[3] == 0 && valsvide[4] == 1){
		//createPlant(string);
		return "usine";
	}
	else if (valsvide[0] == 1 && valsvide[1] == 0 && valsvide[2] == 0
	&& valsvide[3] == 1 && valsvide[4] == 0){
		return "stockage";
	}
	else {
		return "distribution";
	}
}


Avl_Plant* insertAvlPlant(Avl_Plant* root, Plant* plant){
	if (root == NULL){
		return createAvlPlant(plant);
	}
	else{
		if (beforeinorderid(plant->id,root->current->id) == 1){
			root->ls = insertAvlPlant(root->ls,plant);
		}
		else{
			root->rs = insertAvlPlant(root->rs,plant);
		}
	}
	return root;
}

Avl_Plant* searchAvlPlantById(Avl_Plant* root, char id[]){
	if (root == NULL){
		return NULL;
	}
	else{
		if (strcmp(root->current->id,id) == 0){
			return root;
		}
		else{
			if (beforeinorderid(id,root->current->id) == 1){
				return searchAvlPlantById(root->ls,id);
			}
			else{
				return searchAvlPlantById(root->rs,id);
			}
		}
	}
}

Avl_Plant* getAllPlantsFromFile(FILE* file){
	Avl_Plant* root = NULL;
	int count[5] = {0,0,0,0,0};
	if (file == NULL){
		return NULL;
	}
	else{
		char* phrase = malloc(sizeof(char)*200);
		while (feof(file) == 0)
		{
			fgets(phrase,199,file);
			//printf(" %s ",phrase);
			char type[20];
			strcpy(type,getlinetype(phrase));
			if (strcmp(type,"usine") == 0){
				//printf("usine detectee\n");
				Plant* plant = createPlant(phrase);
				root = insertAvlPlant(root,plant);
				count[1]++;
			}
			if (strcmp(type,"source") == 0){
				count[0]++;
			}
			if (strcmp(type,"stockage") == 0){
				count[2]++;
			}
			if (strcmp(type,"distribution") == 0){
				count[3]++;
			}
			
		}
		free(phrase);
		for (int i = 0; i < 5; i++){
				printf("type: %d , %d\n",i,count[i]);
			}
	}
	return root;
}
