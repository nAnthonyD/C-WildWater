#ifndef STDIO
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<time.h>
#define STDIO
#endif
//defines every structure that will be used in the code
//basic structure that represents a plant
typedef struct _plant{
	float max_cap;
	float col_vol;
	float pro_vol;
	char id[40];
}Plant;

//represents a distribution inside a linked list
typedef struct _distribution_node{
    struct _distribution_node* head;
	struct _distribution_node* next;
	float leakage_rate;
	float flow;
	float leaked_volume;
	char id[40];
    char parentid[40];
}DistributionNode;

//represents a storage inside a linked list
typedef struct _storage_node{
	DistributionNode* head;
	struct _storage_node* next;
	float leakage_rate;
	float flow;
	float leaked_volume;
	char id[40];
}StorageNode;

//A plant's classical tree that has storage nodes as children
typedef struct _plant_tree{
	Plant* root;
	StorageNode* head;
}PlantTree;

//The diferent types of avls
typedef struct _distribution_avl{
	int balance;
	DistributionNode* current;
	struct _distribution_avl* ls;
	struct _distribution_avl* rs;
}Avl_Distribution;

typedef struct _storage_avl{
	int balance;
	StorageNode* current;
	struct _storage_avl* ls;
	struct _storage_avl* rs;
}Avl_Storage;

typedef struct _avl_plant{
	int balance;
	Plant* current;
	struct _avl_plant* ls;
	struct _avl_plant* rs;
}Avl_Plant;
