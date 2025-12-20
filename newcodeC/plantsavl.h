#ifndef PLANTSAVL_H
#define PLANTSAVL_H

#include <stdio.h>
#include "managestructures.h"

Plant* createPlant(char string[]);
Avl_Plant* createAvlPlant(Plant* plant);
Avl_Plant* insertAvlPlant(Avl_Plant* root, Plant* plant, int* heightchanged);

Avl_Plant* getAllPlantsFromFile(FILE* file);
void getPlantColAndProcVolume(FILE* file, Avl_Plant* root);
void freeAvlPlant(Avl_Plant* root);

/* Fonctions utilisées par leakage.c */
PlantTree* createPlantTree(Plant* plant);
PlantTree* insertPlantTree(PlantTree* root, StorageNode* storage);

StorageNode* insertInStorageNode(StorageNode* root, DistributionNode* distribution);
DistributionNode* insertInDistributionNode(DistributionNode* root, DistributionNode* distribution);

void browseplanttree(PlantTree* planttree, float* ptotalleakage);

#endif

