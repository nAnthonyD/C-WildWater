#include "managefiles.h"

// Normalizes line endings for Windows compatibility
void normalizeToWindows(char phrase[]){
	int len = strlen(phrase);
			if (len && (phrase[len-1] == '\n' || phrase[len-1] == '\r')) {
				phrase[len-1] = '\0';
				len--;
			}
}

/* function that reads every line of file the to create the Plant's Avl
*/
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
			char type[20];
			strcpy(type,getlinetype(phrase));
			if (strcmp(type,"plant") == 0){
				Plant* plant = createPlant(phrase);
				if (strcmp (plant->id,"Plant #GU200349W") == 0){
				}
				root = insertAvlPlant(root,plant, heightchanged);
				count[1]++;
			}
			if (strcmp(type,"source") == 0){
				count[0]++;
			}
			if (strcmp(type,"storage") == 0){
				count[2]++;
			}
			if (strcmp(type,"distribution") == 0){
				count[3]++;
			}
			
		}
		free(phrase);
		free(heightchanged);
		
	}
	return root;
}

/* function that once more reads the file to get the storage*/
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
	}
	
}
//Simple function to free a Avl
void freeAvlPlant(Avl_Plant* root){
	if (root != NULL){
		freeAvlPlant(root->ls);
		freeAvlPlant(root->rs);
		free(root->current);
		free(root);
	}
}
/* Once more reads the full file to create the "Water distribution" tree
*/
float leakage(char id[],FILE* file, Avl_Plant* root,char* biggestleakageid,char* biggestleak_parent,float* biggestleakage){
	Avl_Plant* plant = searchAvlPlantById(root,id);
	int nmb = 0, nmbr = 0, tst = 0;
	if (file == NULL){
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
			PlantTree* planttree = createPlantTree(plant->current);
			while (fgets(phrase,199,file) != NULL)
			{
				normalizeToWindows(phrase);
				
				colons = getcolons(phrase);
				char linetype[20];
				strcpy(linetype,getlinetype(phrase));
				if (strcmp(linetype,"storage") == 0){
					if (colons[1] != NULL && beforeinorderid(colons[1],id) == 0 && colons[2] != NULL && colons[4] != NULL){
						StorageNode* storagecurrent = createStorageNode(colons[2],atof(colons[4])/100);
						planttree = insertPlantTree(planttree,storagecurrent);
						storageavl = insertAvlStorage(storageavl,storagecurrent,heightchanged);
						nmb++;
					}
				}
				else if (strcmp(linetype,"distribution") == 0){
					if (beforeinorderid(colons[0],id) == 0){
						DistributionNode* distcurrent = createDistributionNode(phrase);
						if (distcurrent != NULL) {
							Avl_Storage* avlstoragecurrent = searchAvlStorageById(storageavl,distcurrent->parentid);
							if (avlstoragecurrent != NULL){
								StorageNode* storagecurrent = avlstoragecurrent->current;
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
			browseplanttree(planttree,ptotalleakage,biggestleakageid,biggestleak_parent,biggestleakage);
			freeAvlDistribution(distavl);
			freeAvlStorage(storageavl);
			freePlantTree(planttree);
		}
		else{
			return -1.0f;
		}
		free(phrase);
		free(heightchanged);
		return *ptotalleakage;
	}
}