#include "managefiles.h"

int main(){
	clock_t start = clock();
	FILE* fichier = NULL;
	fichier = fopen("c-wildwater_v3.dat","rb");
	if (fichier == NULL){
		printf("error opening file");
	}
	char* phrase = malloc(sizeof(char)*200);
	/*for (int i = 0;i<= 100;i++){
			fgets(phrase,199,fichier);
			//printf(" %s ",phrase);
		char type[20] = getlinetype(phrase);
		if (strcmp(type,"usine") == 0){
			printf("usine detectee\n");
			Plant* plant = createPlant(phrase);
			Avl_Plant* avlplant = createAvlPlant(plant);
		}
	}*/
	Avl_Plant* allplants = getAllPlantsFromFile(fichier);
	clock_t mid = clock();
	double time_spent = (double)(mid - start) / CLOCKS_PER_SEC;
	printf("Time spent: %f seconds\n",time_spent);
	getPlantColAndProcVolume(fichier,allplants);
	clock_t end = clock();
	time_spent = (double)(end - start) / CLOCKS_PER_SEC;
	printf("Time spent: %f seconds\n",time_spent);
	printf("Leakage rate: %f\n",leakage("Unit #NY000804Z",fichier,allplants));
	return 0;
}
