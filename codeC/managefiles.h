#ifndef STDIO
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<time.h>
#define STDIO
//#define FICHIER "c-wildwater_v0(1).dat"
#endif

/* To Do: 
Balance the Avl tree when inserting plants
Functions to browse the file and set the plant's collected volume and processed volume
*/

typedef struct _plant{
	char id[40];
	float max_cap;
	float col_vol;
	float pro_vol;
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
	//free(ptr);
	return colons;
}

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

Avl_Plant* rotateLeft(Avl_Plant* a) {
    Avl_Plant* pivot = a->rs;
    a->rs = pivot->ls;
    pivot->ls = a;
    int eq_a = a->balance;
    int eq_p = pivot->balance;
    a->balance = eq_a - max(eq_p,0) - 1;
    int val1 = min(eq_a - 2, eq_a + eq_p - 2);
    pivot->balance = min(val1, eq_p - 1);
    a = pivot;
    return a;
}

Avl_Plant* rotateRight(Avl_Plant* a) {
    Avl_Plant* pivot = a->ls;
    a->ls = pivot->rs;
    pivot->rs = a;
    int eq_a = a->balance;
    int eq_p = pivot->balance;
 	a->balance = eq_a - min(eq_p,0) + 1;
    int val1 = max(eq_a + 2, eq_a + eq_p + 2);
    pivot->balance = max(val1, eq_p + 1);
    a = pivot;
    return a;
}

Avl_Plant* doubleRotateLeft(Avl_Plant* root) {
    root->rs = rotateRight(root->rs);
    return rotateLeft(root);
}

Avl_Plant* doubleRotateRight(Avl_Plant* root) {
    root->ls = rotateLeft(root->ls);
    return rotateRight(root);
}

Avl_Plant* balanceAvlPlant(Avl_Plant* a) {
    if (a->balance >= 2) {
        if (a->rs->balance >= 0) {
            return rotateLeft(a);
        } else {
            return doubleRotateLeft(a);
        }
    } else if (a->balance <= -2) {
        if (a->ls->balance <= 0) {
            return rotateRight(a);
        } else {
            return doubleRotateRight(a);
        }
    }
    return a;
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
			newone->max_cap = atof(cap)/1000;
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
	char** colons = malloc (sizeof(char*)*5);
	colons = getcolons(string);
	int valsvide[5];
	
	if (colons == NULL){
		exit(1);
	}

	for (int i = 0; i < 5;i++){
		if (colons[i][0] == '-'){
			valsvide[i] = 1;
		}
		else{
			valsvide[i] = 0;
		}
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

Avl_Plant* insertAvlPlant(Avl_Plant* root, Plant* plant,int* heightchanged){
	if (root == NULL){
		*heightchanged = 1;
		return createAvlPlant(plant);
	}
	else{
		if (beforeinorderid(plant->id,root->current->id) == 1){
			root->ls = insertAvlPlant(root->ls,plant,heightchanged);
			*heightchanged = -1;
		}
		else if (beforeinorderid(plant->id,root->current->id) == -1){
			root->rs = insertAvlPlant(root->rs,plant,heightchanged);
			*heightchanged = 1;
		}
		else{
			*heightchanged = 0;
			return root;
		}
	}
	if (*heightchanged != 0){
		root->balance += *heightchanged;
		root = balanceAvlPlant(root);
		if (root->balance == 0){
			*heightchanged = 0;
		}
		else{
			*heightchanged = 1;
		}
	}
	if (root->balance > 1 || root->balance < -1){
		printf("unbalanced tree detected at plant id: %s, %d\n",root->current->id,root->balance);
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

void testAvlPlantInorder(Avl_Plant* root){
	char id[40];
	Avl_Plant* test;
	if (root != NULL){

		testAvlPlantInorder(root->ls);
		test = searchAvlPlantById(root,root->current->id);
		if (test == NULL){
			printf("not found");
		}
		//printf("Plant id: %s , max capacity: %f\n",root->current->id,root->current->max_cap);
		testAvlPlantInorder(root->rs);
	}
}

Avl_Plant* getAllPlantsFromFile(FILE* file){
	Avl_Plant* root = NULL;
	int count[4] = {0,0,0,0}; //source, usine, stockage, distribution
	int* heightchanged = malloc(sizeof(int));
	*heightchanged = 0;
	if (file == NULL){
		exit(1);
	}
	else{
		char* phrase = malloc(sizeof(char)*200);
		while (fgets(phrase,199,file) != NULL)
		{
			//printf(" %s ",phrase);
			char type[20];
			strcpy(type,getlinetype(phrase));
			if (strcmp(type,"usine") == 0){
				//printf("usine detectee\n");
				Plant* plant = createPlant(phrase);
				root = insertAvlPlant(root,plant, heightchanged);
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
		printf("\n%s\n",phrase);
		char** colons = getcolons(phrase);
		for(int i = 0; i < 5; i++){
			printf("\n%s\n",colons[i]);
		}
		free(phrase);
		
	}
	testAvlPlantInorder(root);
	Avl_Plant* test = searchAvlPlantById(root,"Plant #KV200751J");
	if (test != NULL){
		
		printf("Plant found: %s with capacity %f\n",test->current->id,test->current->max_cap);
		for (int i = 0; i < 4; i++){
				printf("type: %d , %d\n",i,count[i]);
			}
	}
	else{
		printf("Plant not found\n");
	}
	return root;
}

void getPlantColAndProcVolume(FILE* file, Avl_Plant* root){
	rewind(file);
	if (file == NULL){
		exit(1);
	}
	else{
		char* phrase = malloc(sizeof(char)*200);
		char** colons;
		float col_vol, proc_vol;
		float leakage_rate;
		Avl_Plant* searchedplant;
		if (phrase == NULL){
			exit(1);
		}
		while (fgets(phrase,199,file) != NULL)
		{
			colons = getcolons(phrase);
			char type[20];
			strcpy(type,getlinetype(phrase));
			if (strcmp(type,"source") == 0){
				searchedplant = searchAvlPlantById(root,colons[2]);
				if (searchedplant != NULL){
					col_vol = atof(colons[3])/1000;
					leakage_rate = (atof(colons[4])/100);
					proc_vol = col_vol * (1 - leakage_rate);
					searchedplant->current->col_vol = col_vol;
					searchedplant->current->pro_vol = proc_vol;
				}
			}
			for(int i=0; i<5; i++){
				free(colons[i]);
			}			
			free(colons);

		}
		free(phrase);
		Avl_Plant* test = searchAvlPlantById(root,"Unit #MO000769V");
		if (test != NULL){
			printf("Max Volume: %f, Collected volume: %f, Processed volume: %f\n",test->current->max_cap,test->current->col_vol,test->current->pro_vol);
		}
		else{
			printf("Plant not found\n");
		}
	}
}
