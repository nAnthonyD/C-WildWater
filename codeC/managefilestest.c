#include "managefiles.h"

int main(){
	FILE* fichier = NULL;
	fichier = fopen("c-wildwater_v0(1).dat","r");
	if (fichier == NULL){
		printf("err");
	}
	char* phrase = malloc(sizeof(char)*200);
	for (int i = 0;i<= 100;i++){
			fgets(phrase,199,fichier);
			//printf(" %s ",phrase);
			printf("%s\n",getlinetype(phrase));
	}
	return 0;
}
