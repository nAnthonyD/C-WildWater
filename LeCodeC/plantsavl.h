#ifndef STDIO
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<time.h>
#define STDIO
#endif

#ifndef STRUCTURES
#include "managestructures.h"
#define STRUCTURES
#endif

#ifndef UTILS
#include "utils.h"
#define UTILS
#endif


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

Avl_Plant* createAvlPlant(Plant* plant){
	Avl_Plant* newone = malloc(sizeof(Avl_Plant));
	if (newone == NULL){
		exit(1);
	}
	newone->current = plant;
	newone->ls = NULL;
	newone->rs = NULL;
	newone->balance = 0;
	return newone;
}

/*
Node_Avl* createNodeAvl(phrase){
    Node_Avl* newone = malloc(sizeof(Node_Avl));
    char** colons = malloc(sizeof(char*)*5);
    if (newone == NULL || colons == NULL){
        exit(1);
    }
    colons = getcolons(phrase);
    if(getlinetype(phrase) == "usine"){
        
    }
    else {
        strcpy(newone->id,colons[2]);
        newone->val1 = atof(colons[3])/1000; 
        newone->val2 = 0.0;
        newone->val3 = 0.0; 
    }
    freecolons(colons);
    return newone;
}

Avl* rotateLeft(Avl* a) {
    Avl* pivot = a->rs;
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


Avl* rotateRight(Avl* a) {
    Avl* pivot = a->ls;
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


Avl* doubleRotateLeft(Avl* root) {
    root->rs = rotateRight(root->rs);
    return rotateLeft(root);
}

Avl* doubleRotateRight(Avl* root) {
    root->ls = rotateLeft(root->ls);
    return rotateRight(root);
}
*/
/*
Avl* insertAvl(Avl* root, Node_Avl* val,int* heightchanged){
	if (root == NULL){
		*heightchanged = 1;
		return createAvlPlant(val);
	}
	else{
		if (beforeinorderid(val->id,root->current->id) == 1){
			root->ls = insertAvl(root->ls,val,heightchanged);
			*heightchanged = -1;
		}
		else if (beforeinorderid(val->id,root->current->id) == -1){
			root->rs = insertAvl(root->rs,val,heightchanged);
			*heightchanged = 1;
		}
		else{
			*heightchanged = 0;
			return root;
		}
	}
	if (*heightchanged != 0){
		root->balance += *heightchanged;
		root = balanceAvl(root);
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
}*/

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
	return root;
}


StorageNode* linkStorageNode(StorageNode* head, StorageNode* newone){
	if (head == NULL){
		return newone;
	}
	else{
		StorageNode* current = head;
		while (current->next != NULL){
			current = current->next;
		}
		current->next = newone;
		return head;
	}
}

DistributionNode* linkDistNode(DistributionNode* head, DistributionNode* newone){
	if (head == NULL){
		return newone;
	}
	else{
		DistributionNode* current = head;
		while (current->next != NULL){
			current = current->next;
		}
		current->next = newone;
		return head;
	}
}

StorageNode* insertInStorageNode(StorageNode* root, DistributionNode* distribution){
	if (root == NULL){
		exit(1);
	}
	else{
		root->head = linkDistNode(root->head,distribution);
		return root;
	}
}

DistributionNode* insertInDistributionNode(DistributionNode* root, DistributionNode* distribution){
	if (root == NULL){
		exit(1);
	}
	else{
		root->head = linkDistNode(root->head,distribution);
		return root;
	}
}

PlantTree* createPlantTree(Plant* plant){
	PlantTree* newone = malloc(sizeof(PlantTree));
	if (newone == NULL){
		exit(1);
	}
	newone->root = plant;
	newone->head = NULL;
	return newone;
}

PlantTree* insertPlantTree(PlantTree* root, StorageNode* storage){
	if (root == NULL){
		exit(1);
	}
	else{
		root->head = linkStorageNode(root->head,storage);
		return root;
	}
}

StorageNode* insertStorageNode(StorageNode* root, DistributionNode* distribution){
	if (root == NULL){
		exit(1);
	}
	else{
		root->head = linkDistNode(root->head,distribution);
		return root;
	}
}

void browsedistributiontree(DistributionNode* distnode, float* ptotalleakage, float flow){
	DistributionNode* currentdist = distnode;
	int sons = 0;
	float totalflow = 0;
	while (currentdist != NULL){
		sons++;
		currentdist = currentdist->next;
	}
	currentdist = distnode;
	while (currentdist != NULL){
		totalflow = flow/sons;
		currentdist->flow = totalflow - (totalflow * currentdist->leakage_rate);
		currentdist->leaked_volume = totalflow * currentdist->leakage_rate;
		(*ptotalleakage) += currentdist->leaked_volume;
		browsedistributiontree(currentdist->head,ptotalleakage, currentdist->flow);
		currentdist = currentdist->next;
	}
}

void browsestoragetree(StorageNode* storagenode, float* ptotalleakage){
    DistributionNode* currentdist = storagenode->head;
	int sons = 0;
	float totalflow = storagenode->flow;
	storagenode->leaked_volume = totalflow * storagenode->leakage_rate;
	(*ptotalleakage) += storagenode->leaked_volume;
	float realflow = totalflow - storagenode->leaked_volume;
	while (currentdist != NULL){
		sons++;
        currentdist = currentdist->next;
    }
	currentdist = storagenode->head;
	//printf(" Leaked volume at storage %s: %f , total leakage so far: %f, realflow: %f, sons: %d\n",storagenode->id,storagenode->leaked_volume,*ptotalleakage, realflow, sons);
	browsedistributiontree(currentdist,ptotalleakage,realflow);
}

void browseplanttree(PlantTree* planttree, float* ptotalleakage){
	StorageNode* currentstorage = planttree->head;

	*ptotalleakage = 0;
	int sons = 0;
	while (currentstorage != NULL){
		sons++;
		currentstorage = currentstorage->next;
	}
	currentstorage = planttree->head;
	while (currentstorage != NULL){
		currentstorage->flow = planttree->root->pro_vol / sons;
		browsestoragetree(currentstorage,ptotalleakage);
		currentstorage = currentstorage->next;
	}
}

void freePlantTree(PlantTree* planttree){
	if (planttree != NULL){
		StorageNode* current = planttree->head;
		while (current != NULL){
			StorageNode* next = current->next;
			DistributionNode* dcurrent = current->head;
			while (dcurrent != NULL){
				DistributionNode* dnext = dcurrent->next;
				free(dcurrent);
				dcurrent = dnext;
			}
			free(current);
			current = next;
		}
		free(planttree);
	}
}