#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "leakage.h"
#include "managefiles.h"        
#include "plantsavl.h"          
#include "storageavl.h"         
#include "distributionavl.h"    




float leakage(char id[], FILE* file, Avl_Plant* root) {
	StorageNode* storagecurrent = malloc(sizeof(StorageNode));
	DistributionNode* distcurrent = malloc(sizeof(DistributionNode));
	DistributionNode* distparent = malloc(sizeof(DistributionNode));
	PlantTree* planttree = malloc(sizeof(PlantTree));
	Avl_Plant* plant = searchAvlPlantById(root, id);
	int nmb = 0, nmbr = 0, tst = 0;
	if (file == NULL || storagecurrent == NULL || distcurrent == NULL || planttree == NULL || distparent == NULL) {
		exit(1);
	}
	else {
		rewind(file);
		char* phrase = malloc(sizeof(char) * 200);
		Avl_Storage* storageavl = NULL;
		Avl_Distribution* distavl = NULL;
		char** colons;
		int* heightchanged = malloc(sizeof(int));
		*heightchanged = 0;
		float totalleakage = 0;
		float* ptotalleakage = &totalleakage;
		if (phrase == NULL || heightchanged == NULL) {
			exit(1);
		}
		if (plant != NULL) {
			planttree = createPlantTree(plant->current);
			while (fgets(phrase, 199, file) != NULL)
			{
				colons = getcolons(phrase);
				char linetype[20];
				strcpy(linetype, getlinetype(phrase));
				if (strcmp(linetype, "stockage") == 0) {
					if (colons[1] != NULL && beforeinorderid(colons[1], id) == 0 && colons[2] != NULL && colons[4] != NULL) {
						storagecurrent = createStorageNode(colons[2], atof(colons[4]) / 100);
						planttree = insertPlantTree(planttree, storagecurrent);
						storageavl = insertAvlStorage(storageavl, storagecurrent, heightchanged);
						nmb++;
					}
				}
				else if (strcmp(linetype, "distribution") == 0) {
					if (beforeinorderid(colons[0], id) == 0) {
						distcurrent = createDistributionNode(phrase);
						if (distcurrent != NULL && distcurrent->parentid[0] != '\0'){
							Avl_Storage* avlstoragecurrent = searchAvlStorageById(storageavl, distcurrent->parentid);
							if (avlstoragecurrent != NULL) {
								storagecurrent = avlstoragecurrent->current;
								insertInStorageNode(storagecurrent, distcurrent);
								distavl = insertAvlDistribution(distavl, distcurrent, heightchanged);
							}
							else {
								distavl = insertAvlDistribution(distavl, distcurrent, heightchanged);
								Avl_Distribution* avldistcurrent = searchAvlDistributionById(distavl, distcurrent->parentid);
								if (avldistcurrent != NULL) {
									insertInDistributionNode(avldistcurrent->current, distcurrent);
									tst++;
								}
								else {
								}
							}
							nmbr++;
						}
					}
				}
				freecolons(colons);
			}
			printf("\nt\n");

			showAvlStorageInorder(storageavl);
			browseplanttree(planttree, ptotalleakage);
			printf("%d distributions linked to storages\n", nmbr - tst);
			printf("%d distributions linked to distributions\n", tst);
			printf("%d storages linked to plant %s\n", nmb, id);
			printf("%d distributions linked to plant %s\n", nmbr, id);
			printf("Total leakage for plant %s: %f\n", id, *ptotalleakage);
		}
		else {
			printf("Plant id %s not found in the tree\n", id);
			exit(1);
		}
		free(phrase);
		free(heightchanged);
		return *ptotalleakage;
	}
}