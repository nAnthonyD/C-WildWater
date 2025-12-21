#ifndef AVLS_H
#define AVLS_H

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

// From plantsavl.h
Plant* createPlant(char string[]);
Avl_Plant* createAvlPlant(Plant* plant);
Avl_Plant* rotateLeft(Avl_Plant* a);
Avl_Plant* rotateRight(Avl_Plant* a);
Avl_Plant* doubleRotateLeft(Avl_Plant* root);
Avl_Plant* doubleRotateRight(Avl_Plant* root);
Avl_Plant* balanceAvlPlant(Avl_Plant* a);
Avl_Plant* insertAvlPlant(Avl_Plant* root, Plant* plant,int* heightchanged);
StorageNode* linkStorageNode(StorageNode* head, StorageNode* newone);
DistributionNode* linkDistNode(DistributionNode* head, DistributionNode* newone);
StorageNode* insertInStorageNode(StorageNode* root, DistributionNode* distribution);
DistributionNode* insertInDistributionNode(DistributionNode* root, DistributionNode* distribution);
PlantTree* createPlantTree(Plant* plant);
PlantTree* insertPlantTree(PlantTree* root, StorageNode* storage);
void browsedistributiontree(DistributionNode* distnode, float* ptotalleakage, float flow, char biggestleakageid[], char biggestleak_parent[], float* biggestleakage);
void browsestoragetree(StorageNode* storagenode, float* ptotalleakage, char biggestleakageid[], char biggestleak_parent[], float* biggestleakage);
void browseplanttree(PlantTree* planttree, float* ptotalleakage, char biggestleakageid[], char biggestleak_parent[], float* biggestleakage);
void freePlantTree(PlantTree* planttree);

// From storageavl.h
StorageNode* createStorageNode(char id[], float leakage_rate);
Avl_Storage* createAvlStorage(StorageNode* storage);
Avl_Storage* rotateLeftStorage(Avl_Storage* a);
Avl_Storage* rotateRightStorage(Avl_Storage* a);
Avl_Storage* doubleRotateLeftStorage(Avl_Storage* root);
Avl_Storage* doubleRotateRightStorage(Avl_Storage* root);
Avl_Storage* balanceAvlStorage(Avl_Storage* a);
Avl_Storage* insertAvlStorage(Avl_Storage* root, StorageNode* storage, int* heightchanged);
Avl_Storage* searchAvlStorageById(Avl_Storage* root, char id[]);
void showAvlStorageInorder(Avl_Storage* root);
void freeAvlStorage(Avl_Storage* root);

// From distributionavl.h
Avl_Distribution* createAvlDistribution(DistributionNode* distribution);
Avl_Distribution* rotateLeftDistribution(Avl_Distribution* a);
Avl_Distribution* rotateRightDistribution(Avl_Distribution* a);
Avl_Distribution* doubleRotateLeftDistribution(Avl_Distribution* root);
Avl_Distribution* doubleRotateRightDistribution(Avl_Distribution* root);
Avl_Distribution* balanceAvlDistribution(Avl_Distribution* a);
Avl_Distribution* insertAvlDistribution(Avl_Distribution* root, DistributionNode* distribution, int* heightchanged);
DistributionNode* createDistributionNode(char* phrase);
Avl_Distribution* searchAvlDistributionById(Avl_Distribution* root, char id[]);
void freeAvlDistribution(Avl_Distribution* root);

#endif // AVLS_H