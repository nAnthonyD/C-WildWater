
#ifndef MANAGESTRUCTURES_H
#define MANAGESTRUCTURES_H



#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>



typedef struct _plant{
	char id[64];
	float max_cap;
	float col_vol;
	float pro_vol;
}Plant;

typedef struct _distribution_node{
	char id[40];
    char parentid[40];
	float leakage_rate;
	float flow;
	float leaked_volume;
    struct _distribution_node* head;
	struct _distribution_node* next;
}DistributionNode;

typedef struct _storage_node{
	char id[40];
	float leakage_rate;
	float flow;
	float leaked_volume;
	DistributionNode* head;
	struct _storage_node* next;
}StorageNode;

typedef struct _plant_tree{
	Plant* root;
	StorageNode* head;
}PlantTree;

typedef struct node_avl{
    char id[40]; 
    float val1; //Plant: Max Capacity, Storage: leakage rate, Distribution: leakage rate
    float val2; //Plant: Collected Volume, Storage: unused, Distribution: unused
    float val3; //Plant: Processed Volume, Storage: unused, Distribution: unused
}Node_Avl;

typedef struct _avl{
    void* current;
    void* ls;
    void* rs;
    int balance;
}Avl;

typedef struct _distribution_avl{
	DistributionNode* current;
	struct _distribution_avl* ls;
	struct _distribution_avl* rs;
	int balance;
}Avl_Distribution;

typedef struct _storage_avl{
	StorageNode* current;
	struct _storage_avl* ls;
	struct _storage_avl* rs;
	int balance;
}Avl_Storage;

typedef struct _avl_plant{
	Plant* current;
	struct _avl_plant* ls;
	struct _avl_plant* rs;
	int balance;
}Avl_Plant;


#endif /* MANAGESTRUCTURES_H */
