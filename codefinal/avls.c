#include "avls.h"

// Parses a CSV line to create a Plant structure, extracting id and max capacity
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

//simple avl manipulation functions
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

// Links a StorageNode to the end of a linked list
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

// Links a DistributionNode to the end of a linked list
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

// Adds a distribution node as a child of a storage node
StorageNode* insertInStorageNode(StorageNode* root, DistributionNode* distribution){
	if (root == NULL){
		exit(1);
	}
	else{
		root->head = linkDistNode(root->head,distribution);
		return root;
	}
}

// Adds a distribution node as a child of another distribution node
DistributionNode* insertInDistributionNode(DistributionNode* root, DistributionNode* distribution){
	if (root == NULL){
		exit(1);
	}
	else{
		root->head = linkDistNode(root->head,distribution);
		return root;
	}
}

// Creates a PlantTree structure for a plant
PlantTree* createPlantTree(Plant* plant){
	PlantTree* newone = malloc(sizeof(PlantTree));
	if (newone == NULL){
		exit(1);
	}
	newone->root = plant;
	newone->head = NULL;
	return newone;
}

// Adds a storage node to the plant tree
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

/* the next 3 functions basically browse a plant's tree in oder to calculate the total leakeage*/

void browsedistributiontree(DistributionNode* distnode, float* ptotalleakage, float flow, char biggestleakageid[], char biggestleak_parent[], float* biggestleakage){
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
		if (currentdist->leaked_volume > *biggestleakage){
			*biggestleakage = currentdist->leaked_volume;
			strcpy(biggestleakageid,currentdist->id);
			strcpy(biggestleak_parent, currentdist->parentid);
		}
		browsedistributiontree(currentdist->head,ptotalleakage, currentdist->flow,biggestleakageid,biggestleak_parent,biggestleakage);
		currentdist = currentdist->next;
	}
}

// Browses storage tree to calculate leakage for storage and its distributions
void browsestoragetree(StorageNode* storagenode, float* ptotalleakage, char biggestleakageid[], char biggestleak_parent[], float* biggestleakage){
    DistributionNode* currentdist = storagenode->head;
	int sons = 0;
	float totalflow = storagenode->flow;
	storagenode->leaked_volume = totalflow * storagenode->leakage_rate;
	(*ptotalleakage) += storagenode->leaked_volume;
	float realflow = totalflow - storagenode->leaked_volume;
	if (storagenode->leaked_volume > *biggestleakage){
		*biggestleakage = storagenode->leaked_volume;
		strcpy(biggestleakageid,storagenode->id);
		strcpy(biggestleak_parent, "Plant");
	}
	while (currentdist != NULL){
		sons++;
        currentdist = currentdist->next;
    }
	currentdist = storagenode->head;
	browsedistributiontree(currentdist,ptotalleakage,realflow,biggestleakageid,biggestleak_parent,biggestleakage);
}

// Browses the entire plant tree to calculate total leakage and find biggest leak
void browseplanttree(PlantTree* planttree, float* ptotalleakage, char biggestleakageid[], char biggestleak_parent[], float* biggestleakage){
	StorageNode* currentstorage = planttree->head;
	*biggestleakage = -1;
	*ptotalleakage = 0;
	int sons = 0;
	while (currentstorage != NULL){
		sons++;
		currentstorage = currentstorage->next;
	}
	currentstorage = planttree->head;
	while (currentstorage != NULL){
		currentstorage->flow = planttree->root->pro_vol / sons;
		browsestoragetree(currentstorage,ptotalleakage,biggestleakageid,biggestleak_parent,biggestleakage);
		currentstorage = currentstorage->next;
	}
	
	if (strcmp(biggestleak_parent,"Plant") == 0){
		strcpy(biggestleak_parent, planttree->root->id);
	}
}

// Frees the entire plant tree structure
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

StorageNode* createStorageNode(char id[], float leakage_rate){
	StorageNode* newone = malloc(sizeof(StorageNode));
	if (newone == NULL){
		exit(1);
	}
	strcpy(newone->id,id);
	newone->leakage_rate = leakage_rate;
    newone->flow = 0;
	newone->head = NULL;
	newone->next = NULL;
    newone->leaked_volume = 0;
	return newone;
}

/* The next functions are just the same avl functions but with other types*/
Avl_Storage* createAvlStorage(StorageNode* storage){
    Avl_Storage* newone = malloc(sizeof(Avl_Storage));
    if (newone == NULL){
        exit(1);
    }
    newone->current = storage;
    newone->ls = NULL;
    newone->rs = NULL;
    newone->balance = 0;
    return newone;
}

Avl_Storage* rotateLeftStorage(Avl_Storage* a) {
    Avl_Storage* pivot = a->rs;
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

Avl_Storage* rotateRightStorage(Avl_Storage* a) {
    Avl_Storage* pivot = a->ls;
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

Avl_Storage* doubleRotateLeftStorage(Avl_Storage* root) {
    root->rs = rotateRightStorage(root->rs);
    return rotateLeftStorage(root);
}

Avl_Storage* doubleRotateRightStorage(Avl_Storage* root) {
    root->ls = rotateLeftStorage(root->ls);
    return rotateRightStorage(root);
}


Avl_Storage* balanceAvlStorage(Avl_Storage* a) {
    if (a->balance >= 2) {
        if (a->rs && a->rs->balance >= 0) {
            return rotateLeftStorage(a);
        } else {
            return doubleRotateLeftStorage(a);
        }
    } else if (a->balance <= -2) {
        if (a->ls && a->ls->balance <= 0) {
            return rotateRightStorage(a);
        } else {
            return doubleRotateRightStorage(a);
        }
    }
    return a;
}



Avl_Storage* insertAvlStorage(Avl_Storage* root, StorageNode* storage, int* heightchanged){
    if (root == NULL){
        *heightchanged = 1;
        return createAvlStorage(storage);
    }
    else{
        if (beforeinorderid(storage->id,root->current->id) == 1){
            root->ls = insertAvlStorage(root->ls,storage,heightchanged);
            *heightchanged = -1;
        }
        else if (beforeinorderid(storage->id,root->current->id) == -1){
            root->rs = insertAvlStorage(root->rs,storage,heightchanged);
            *heightchanged = 1;
        }
        else{
            *heightchanged = 0;
            return root;
        }
    }
    if (*heightchanged != 0){
        root->balance += *heightchanged;
        root = balanceAvlStorage(root);
        if (root->balance == 0){
            *heightchanged = 0;
        }
        else{
            *heightchanged = 1;
        }
    }
    return root;
}

Avl_Storage* searchAvlStorageById(Avl_Storage* root, char id[]){
    if (root == NULL){
        return NULL;
    }
     else{
        int found = beforeinorderid(id,root->current->id);
        if (found == 0){
            return root;
        }
        else{
            if (found == 1){
                return searchAvlStorageById(root->ls,id);
            }
            else if (found == -1){
                return searchAvlStorageById(root->rs,id);
            }
            else{
                return NULL;
               
            }
        }
    }
}

void showAvlStorageInorder(Avl_Storage* root){
    if (root != NULL){
        showAvlStorageInorder(root->ls);
        printf("Storage id: %s , leakage rate: %f\n",root->current->id,root->current->leakage_rate);
        showAvlStorageInorder(root->rs);
    }
}

void freeAvlStorage(Avl_Storage* root){
    if (root != NULL){
        freeAvlStorage(root->ls);
        freeAvlStorage(root->rs);
        free(root);
    }
}

Avl_Distribution* createAvlDistribution(DistributionNode* distribution){
    Avl_Distribution* newone = malloc(sizeof(Avl_Distribution));
    if (newone == NULL){
        exit(1);
    }
    newone->current = distribution;
    newone->ls = NULL;
    newone->rs = NULL;
    newone->balance = 0;
    return newone;
}


Avl_Distribution* rotateLeftDistribution(Avl_Distribution* a) {
    Avl_Distribution* pivot = a->rs;
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

Avl_Distribution* rotateRightDistribution(Avl_Distribution* a) {
    Avl_Distribution* pivot = a->ls;
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

Avl_Distribution* doubleRotateLeftDistribution(Avl_Distribution* root) {
    root->rs = rotateRightDistribution(root->rs);
    return rotateLeftDistribution(root);
}

Avl_Distribution* doubleRotateRightDistribution(Avl_Distribution* root) {
    root->ls = rotateLeftDistribution(root->ls);
    return rotateRightDistribution(root);
}

Avl_Distribution* balanceAvlDistribution(Avl_Distribution* a) {
    if (a->balance >= 2) {
        if (a->rs->balance >= 0) {
            return rotateLeftDistribution(a);
        } else {
            return doubleRotateLeftDistribution(a);
        }
    } else if (a->balance <= -2) {
        if (a->ls->balance <= 0) {
            return rotateRightDistribution(a);
        } else {
            return doubleRotateRightDistribution(a);
        }
    }
    return a;
}

Avl_Distribution* insertAvlDistribution(Avl_Distribution* root, DistributionNode* distribution, int* heightchanged){
    if (root == NULL){
		*heightchanged = 1;
		return createAvlDistribution(distribution);
	}
	else{
		if (beforeinorderid(distribution->id,root->current->id) == 1){
			root->ls = insertAvlDistribution(root->ls,distribution,heightchanged);
			*heightchanged = -1;
		}
		else if (beforeinorderid(distribution->id,root->current->id) == -1){
			root->rs = insertAvlDistribution(root->rs,distribution,heightchanged);
			*heightchanged = 1;
		}
		else{
			*heightchanged = 0;
			return root;
		}
	}
	if (*heightchanged != 0){
		root->balance += *heightchanged;
		root = balanceAvlDistribution(root);
		if (root->balance == 0){
			*heightchanged = 0;
		}
		else{
			*heightchanged = 1;
		}
	}
	return root;
}


DistributionNode* createDistributionNode(char* phrase){
    char** colons = malloc(sizeof(char*)*5);
    DistributionNode* newone = malloc(sizeof(DistributionNode));
    if (newone == NULL || colons == NULL){
        exit(1);
    }
    colons = getcolons(phrase);
    if (colons[1] != NULL && colons[2] != NULL && colons[3] != NULL && colons[4] != NULL) {
        strcpy(newone->id,colons[2]);
        newone->leakage_rate = atof(colons[4])/100;
        strcpy(newone->parentid,colons[1]);
    } else {
        free(newone);
        freecolons(colons);
        return NULL;
    }
    newone->next = NULL;
    newone->head = NULL;
    newone->flow = 0;
    newone->leaked_volume = 0;
    freecolons(colons);
    return newone;
}

Avl_Distribution* searchAvlDistributionById(Avl_Distribution* root, char id[]){
    if (root == NULL){
        return NULL;
    }
    else{
        int found = beforeinorderid(id,root->current->id);
        if (found == 0){
            return root;
        }
        else{
            if (found == 1){
                return searchAvlDistributionById(root->ls,id);
            }
            else if (found == -1){
                return searchAvlDistributionById(root->rs,id);
            }
            else{
                return NULL;
                printf("Error in searchAvlDistributionById function\n");
            }
        }
    }
}

void freeAvlDistribution(Avl_Distribution* root){
    if (root != NULL){
        freeAvlDistribution(root->ls);
        freeAvlDistribution(root->rs);
        free(root);
    }
}