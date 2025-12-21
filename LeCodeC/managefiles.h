#ifndef STDIO
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<time.h>

#define STDIO
//#define FICHIER "c-wildwater_v0(1).dat"
#endif

#ifndef STRUCTURES
#include "managestructures.h"	
#define STRUCTURES
#endif

#ifndef UTILS
#include "utils.h"
#define UTILS
#endif

#include "storageavl.h"
#include "plantsavl.h"
#include "distributionavl.h"

void testAvlPlantInorder(Avl_Plant* root){
	Avl_Plant* test;
	if (root != NULL){

		testAvlPlantInorder(root->ls);
		test = searchAvlPlantById(root,root->current->id);
		if (test == NULL){
			printf("not found, id = %s\n",root->current->id);
		}
		//printf("Plant id: %s , max capacity: %f\n",root->current->id,root->current->max_cap);
		testAvlPlantInorder(root->rs);
	}
}

/* This function allows to read the files on windows even if it's much slower.
   It removes the \r or \n at the end of the string if present, so fgets will reach the next line
   even on windows.
*/
void normalizeToWindows(char phrase[]){
	int len = strlen(phrase);
			if (len && (phrase[len-1] == '\n' || phrase[len-1] == '\r')) {
				phrase[len-1] = '\0';
				len--;
			}
}

Avl_Plant* getAllPlantsFromFile(FILE* file){
	Avl_Plant* root = NULL;
	int count[4] = {0,0,0,0};
	int* heightchanged = malloc(sizeof(int));
	*heightchanged = 0;
	if (file == NULL){
		exit(1);
	}
	
	else{
		rewind(file);
		char* phrase = malloc(sizeof(char)*200);
		while (fgets(phrase,199,file) != NULL)
		{	
			
			normalizeToWindows(phrase);
			//printf(" %s ",phrase);
			char type[20];
			strcpy(type,getlinetype(phrase));
			if (strcmp(type,"usine") == 0){
				//printf("usine detectee\n");
				Plant* plant = createPlant(phrase);
				if (strcmp (plant->id,"Plant #GU200349W") == 0){
					printf("Plant created with id: %s and max capacity: %f\n",plant->id,plant->max_cap);
				}
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
	if (file == NULL){
		exit(1);
	}
	else{
		rewind(file);
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
			normalizeToWindows(phrase);
			colons = getcolons(phrase);
			char type[20];
			strcpy(type,getlinetype(phrase));
			if (strcmp(type,"source") == 0){
				if (colons[2] != NULL){
					searchedplant = searchAvlPlantById(root,colons[2]);
					if (searchedplant != NULL && colons[3] != NULL && colons[4] != NULL){
						col_vol = atof(colons[3])/1000;
						leakage_rate = (atof(colons[4])/100);
						proc_vol = col_vol * (1 - leakage_rate);
						searchedplant->current->col_vol += col_vol;
						searchedplant->current->pro_vol += proc_vol;
					}
				}
			}
			for(int i=0; i<5; i++){
				free(colons[i]);
			}			
			free(colons);

		}
		free(phrase);
		Avl_Plant* test = searchAvlPlantById(root,"Module #OS101217V");
		if (test != NULL){
			printf("Max Volume: %f, Collected volume: %f, Processed volume: %f\n",test->current->max_cap,test->current->col_vol,test->current->pro_vol);
		}
		else{
			printf("Plant not found\n");
		}
	}
}

void freeAvlPlant(Avl_Plant* root){
	if (root != NULL){
		freeAvlPlant(root->ls);
		freeAvlPlant(root->rs);
		free(root->current);
		free(root);
	}
}

float leakage(char id[],FILE* file, Avl_Plant* root){
	StorageNode* storagecurrent = malloc(sizeof(StorageNode));
	DistributionNode* distcurrent = malloc(sizeof(DistributionNode));
	DistributionNode* distparent = malloc(sizeof(DistributionNode));
	PlantTree* planttree = malloc (sizeof(PlantTree));
	Avl_Plant* plant = searchAvlPlantById(root,id);
	int nmb = 0, nmbr = 0, tst = 0;
	if (file == NULL || storagecurrent == NULL || distcurrent == NULL || planttree == NULL || distparent == NULL){
		exit(1);
	}
	else{
		rewind(file);
		char* phrase = malloc(sizeof(char)*200);
		Avl_Storage* storageavl = NULL;
		Avl_Distribution* distavl = NULL;
		char** colons;
		int* heightchanged = malloc(sizeof(int));
		*heightchanged = 0;
		float totalleakage = 0;
		float* ptotalleakage = &totalleakage;
		if (phrase == NULL || heightchanged == NULL){
			exit(1);
		}
		if (plant != NULL){
			planttree = createPlantTree(plant->current);
			while (fgets(phrase,199,file) != NULL)
			{
				normalizeToWindows(phrase);
				
				colons = getcolons(phrase);
				char linetype[20];
				strcpy(linetype,getlinetype(phrase));
				if (strcmp(linetype,"stockage") == 0){
					if (colons[1] != NULL && beforeinorderid(colons[1],id) == 0 && colons[2] != NULL && colons[4] != NULL){
						storagecurrent = createStorageNode(colons[2],atof(colons[4])/100);
						planttree = insertPlantTree(planttree,storagecurrent);
						storageavl = insertAvlStorage(storageavl,storagecurrent,heightchanged);
						nmb++;
					}
				}
				else if (strcmp(linetype,"distribution") == 0){
					if (beforeinorderid(colons[0],id) == 0){
						distcurrent = createDistributionNode(phrase);
						if (distcurrent != NULL) {
							Avl_Storage* avlstoragecurrent = searchAvlStorageById(storageavl,distcurrent->parentid);
							if (avlstoragecurrent != NULL){
								storagecurrent = avlstoragecurrent->current;
								insertInStorageNode(storagecurrent,distcurrent);
								distavl = insertAvlDistribution(distavl,distcurrent,heightchanged);
							}
							else{
								distavl = insertAvlDistribution(distavl,distcurrent,heightchanged);		
								Avl_Distribution* avldistcurrent = searchAvlDistributionById(distavl,distcurrent->parentid);
								if (avldistcurrent != NULL){
									insertInDistributionNode(avldistcurrent->current,distcurrent);
									tst++;
								}
								else{
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
			browseplanttree(planttree,ptotalleakage);
			printf("%d distributions linked to storages\n",nmb - tst);
			printf("%d distributions linked to distributions\n",tst);
			printf("%d storages linked to plant %s\n",nmb,id);
			printf("%d distributions linked to plant %s\n",nmbr,id);
			printf("Total leakage for plant %s: %f\n",id,*ptotalleakage);
		}
		else{
			printf("Plant id %s not found in the tree\n",id);
			exit(1);
		}
		free(phrase);
		free(heightchanged);
		return *ptotalleakage;
	}
}

