#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "managestructures.h"
#include "utils.h"
#include "plantsavl.h"



Avl_Plant* getAllPlantsFromFile(FILE* file) {
	if (!file) return NULL;

	Avl_Plant* root = NULL;
	int heightchanged = 0;

	rewind(file);

	char line[256];
	int n = 0;

	while (fgets(line, sizeof(line), file)) {
		n++;


		// Important : appeler getlinetype UNE SEULE FOIS
		char* type = getlinetype(line);



		if (type && strcmp(type, "usine") == 0) {
			

			Plant* p = createPlant(line);

		

			root = insertAvlPlant(root, p, &heightchanged);
		}
	}

	return root;
}



void getPlantColAndProcVolume(FILE* file, Avl_Plant* root) {
	if (!file || !root) return;

	rewind(file);
	char line[256];

	while (fgets(line, sizeof(line), file)) {
		if (strcmp(getlinetype(line), "source") == 0) {
			char** col = getcolons(line);

			// col[2] = id usine
			// col[3] = volume capté
			// col[4] = taux de fuite
			Avl_Plant* plant = searchAvlPlantById(root, col[2]);

			if (plant) {
				float captured = atof(col[3]) / 1000.0f;
				float leak = atof(col[4]) / 100.0f;
				float processed = captured * (1.0f - leak);

				plant->current->col_vol += captured;
				plant->current->pro_vol += processed;
			}

			freecolons(col);
		}
	}
}


void freeAvlPlant(Avl_Plant* root) {
	if (!root) return;

	freeAvlPlant(root->ls);
	freeAvlPlant(root->rs);

	free(root->current);
	free(root);
}



Plant* createPlant(char line[]) {
	if (!line) return NULL;

	char buf[512];
	strncpy(buf, line, sizeof(buf) - 1);
	buf[sizeof(buf) - 1] = '\0';

	char* cols[5] = { 0 };
	int i = 0;

	char* tok = strtok(buf, ";");
	while (tok && i < 5) {
		cols[i++] = tok;
		tok = strtok(NULL, ";");
	}

	if (i < 4 || !cols[1] || !cols[3]) return NULL;
	if (cols[1][0] == '-' || cols[3][0] == '-') return NULL;

	Plant* p = malloc(sizeof(*p));
	if (!p) return NULL;

	memset(p, 0, sizeof(*p));

	strncpy(p->id, cols[1], sizeof(p->id) - 1);
	p->id[sizeof(p->id) - 1] = '\0';

	p->max_cap = (float)(atof(cols[3]) / 1000.0f);

	return p;
}


Avl_Plant* createAvlPlant(Plant* plant) {
	Avl_Plant* newone = malloc(sizeof(Avl_Plant));
	if (newone == NULL) {
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
	a->balance = eq_a - max(eq_p, 0) - 1;
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
	a->balance = eq_a - min(eq_p, 0) + 1;
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
		}
		else {
			return doubleRotateLeft(a);
		}
	}
	else if (a->balance <= -2) {
		if (a->ls->balance <= 0) {
			return rotateRight(a);
		}
		else {
			return doubleRotateRight(a);
		}
	}
	return a;
}


Avl_Plant* insertAvlPlant(Avl_Plant* root, Plant* plant, int* heightchanged) {
	if (root == NULL) {
		*heightchanged = 1;
		return createAvlPlant(plant);
	}
	else {
		if (beforeinorderid(plant->id, root->current->id) == 1) {
			root->ls = insertAvlPlant(root->ls, plant, heightchanged);
			*heightchanged = -1;
		}
		else if (beforeinorderid(plant->id, root->current->id) == -1) {
			root->rs = insertAvlPlant(root->rs, plant, heightchanged);
			*heightchanged = 1;
		}
		else {
			*heightchanged = 0;
			return root;
		}
	}
	if (*heightchanged != 0) {
		root->balance += *heightchanged;
		root = balanceAvlPlant(root);
		if (root->balance == 0) {
			*heightchanged = 0;
		}
		else {
			*heightchanged = 1;
		}
	}
	if (root->balance > 1 || root->balance < -1) {
		printf("unbalanced tree detected at plant id: %s, %d\n", root->current->id, root->balance);
	}
	return root;
}


 StorageNode* linkStorageNode(StorageNode* head, StorageNode* newone) {
	if (head == NULL) {
		return newone;
	}
	else {
		StorageNode* current = head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = newone;
		return head;
	}
}

 DistributionNode* linkDistNode(DistributionNode* head, DistributionNode* node) {
	 if (!node) return head;

	 // sécurité : si node arrive avec un next sale
	 node->next = NULL;

	 // insertion en tête
	 node->next = head;
	 return node;
 }


 StorageNode* insertInStorageNode(StorageNode* root, DistributionNode* distribution) {
	 if (!root || !distribution) return root;

	 root->head = linkDistNode(root->head, distribution);
	 return root;
 }

 DistributionNode* insertInDistributionNode(DistributionNode* root, DistributionNode* distribution) {
	 if (!root || !distribution) return root;

	 root->head = linkDistNode(root->head, distribution);
	 return root;
 }

PlantTree* createPlantTree(Plant* plant) {
	PlantTree* newone = malloc(sizeof(PlantTree));
	if (newone == NULL) {
		exit(1);
	}
	newone->root = plant;
	newone->head = NULL;
	return newone;
}

PlantTree* insertPlantTree(PlantTree* root, StorageNode* storage) {
	if (root == NULL) {
		exit(1);
	}
	else {
		root->head = linkStorageNode(root->head, storage);
		return root;
	}
}

StorageNode* insertStorageNode(StorageNode* root, DistributionNode* distribution) {
	if (root == NULL) {
		exit(1);
	}
	else {
		root->head = linkDistNode(root->head, distribution);
		return root;
	}
}

void browsedistributiontree(DistributionNode* distnode, float* ptotalleakage, float flow, int* preapeat) {
	DistributionNode* currentdist = distnode;
	int sons = 0;
	float totalflow = 0;
	while (currentdist != NULL) {
		sons++;
		currentdist = currentdist->next;
	}
	currentdist = distnode;
	while (currentdist != NULL) {
		(*preapeat)++;
		totalflow = flow / sons;
		currentdist->flow = totalflow - (totalflow * currentdist->leakage_rate);
		currentdist->leaked_volume = totalflow * currentdist->leakage_rate;
		(*ptotalleakage) += currentdist->leaked_volume;
		browsedistributiontree(currentdist->head, ptotalleakage, currentdist->flow, preapeat);
		currentdist = currentdist->next;
	}
}

void browsestoragetree(StorageNode* storagenode, float* ptotalleakage) {
	DistributionNode* currentdist = storagenode->head;
	printf("Storage id: %s , leakage rate: %f\n", storagenode->id, storagenode->leakage_rate);
	int sons = 0;
	float totalflow = storagenode->flow;
	storagenode->leaked_volume = totalflow * storagenode->leakage_rate;
	(*ptotalleakage) += storagenode->leaked_volume;
	float realflow = totalflow - storagenode->leaked_volume;
	while (currentdist != NULL) {
		sons++;
		currentdist = currentdist->next;
	}
	int* preapeat = malloc(sizeof(int));
	*preapeat = 0;
	currentdist = storagenode->head;
	//printf(" Leaked volume at storage %s: %f , total leakage so far: %f, realflow: %f, sons: %d\n",storagenode->id,storagenode->leaked_volume,*ptotalleakage, realflow, sons);
	browsedistributiontree(currentdist, ptotalleakage, realflow, preapeat);
	printf("\n%d\n", *preapeat);
	free(preapeat);
}

void browseplanttree(PlantTree* planttree, float* ptotalleakage) {
	StorageNode* currentstorage = planttree->head;
	*ptotalleakage = 0;
	int sons = 0;
	while (currentstorage != NULL) {
		sons++;
		currentstorage = currentstorage->next;
	}
	printf("Plant pro_vol: %f, number of storages: %d\n", planttree->root->pro_vol, sons);
	currentstorage = planttree->head;
	while (currentstorage != NULL) {
		currentstorage->flow = planttree->root->pro_vol / sons;
		printf("Storage %s gets flow: %f\n", currentstorage->id, currentstorage->flow);
		browsestoragetree(currentstorage, ptotalleakage);
		currentstorage = currentstorage->next;
	}
}