#ifndef STDIO
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define STDIO
//#define FICHIER "c-wildwater_v0(1).dat"
#endif

/* To Do: 
Avl plants in id "alphabetic" order
Functions to browse the file and set the plant's collected volume and processed volume
Create Plant's Avl about collected volume, processed volume and capacity*/

typedef struct _plant{
	char id[40];
	int max_cap;
	int col_vol;
	int pro_vol;
}Plant;

typedef struct _avl_plant{
	Plant* current;
	Plant* ls;
	Plant* rs;
}Avl_Plant;

typedef struct _linked_plant{
	Plant* current;
	struct _linked_plant* next;
}Linked_Plant;

int beforeinorderid(char str1[], char str2[]){
	int pos1,pos2 = 0;
	int difference = 0;
	while(str1[pos1] != '#'){
		pos1++;
	}
	while(str2[pos2] != '#'){
		pos2++;
	}
	while(difference == 0 && str1[pos1] != '\0' && str2[pos2] != '\0'){
		if (str1[pos1] < str2[pos2]){
			return 1;
		}
		else if (str1[pos1] > str2[pos2]){
			return -1;
		}
		else{
			pos1++;
			pos2++;
		}
	}
	return 0;
	/*while(str1[pos] != '\0' && str2[pos] != '\0'){
		if (str1[pos] < str2[pos]){
			return 1;
		}
		else if (str1[pos] > str2[pos]){
			return 0;
		}
		pos++;
	}
	if (str1[pos] == '\0' && str2[pos] != '\0'){
		return 1;
	}
	else{
		return 0;
	}*/
}

void stringtotable(char source[],char destiny[],int size){
	int pos = 0;
	while(source[pos] != '\n' && source[pos] != '\0' && pos < size){
		destiny[pos] = source[pos];
	}	
}

Plant* createPlant(char string[]){
	Plant* newone = malloc(sizeof(Plant));
	if (newone == NULL){
		exit(1);
	}
	int pos = 0;
	char cap[40];
	for (int i = 0; i <= 5;i++){
		if (i == 1){
		int j = 0;
			while(string[pos] != ';' && string[pos] != '\n' && string[pos] != '\0'){
					newone->id[j] = string[pos];
					//printf("%c",string[pos]);
					j++;
					pos++;
			}
			printf("\n%s\n",newone->id);
		}
		else if (i == 3){
			int j = 0;
			while(string[pos] != ';' && string[pos] != '\n' && string[pos] != '\0'){
						cap[j] = string[pos];
						j++;
						pos++;
				}
			newone->max_cap = atoi(cap);
			//printf("%d", newone->max_cap);
		}
		if (string[pos] == '\n' || string[pos] == '\0'){
			i = 6;
		}
		while(string[pos] != ';'){
			pos++;
		}
		pos++;
	}
}

char* getlinetype(char string[]){
	int pos = 0;
	char aswer[200];
	int valsvide[5];
	
	for (int i = 0; i <= 5;i++){
		if (string[pos] == '-'){
			valsvide[i] = 1;
		}
		else{
			valsvide[i] = 0;
		}
		if (string[pos] == '\n' || string[pos] == '\0' ){
			i = 6;
		}
		while(string[pos] != ';' && string[pos] != '\n' && string[pos] != '\0'){
			pos++;
		}
		pos++;
	}
	if (valsvide[0] == 1 && valsvide[1] == 0 && valsvide[2] == 0
	&& valsvide[3] == 0 && valsvide[4] == 0){
		return "source";
	}
	else if (valsvide[0] == 1 && valsvide[1] == 0 && valsvide[2] == 1
	&& valsvide[3] == 0 && valsvide[4] == 1){
		createPlant(string);
		return "usine";
	}
	else if (valsvide[0] == 1 && valsvide[1] == 0 && valsvide[2] == 0
	&& valsvide[3] == 1 && valsvide[4] == 0){
		return "stockage";
	}
	else {
		return "distribution";
	}
}

