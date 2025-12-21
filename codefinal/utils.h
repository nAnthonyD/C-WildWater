#ifndef UTILS_H
#define UTILS_H

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

int max(int a, int b);
int min(int a, int b);
char** getcolons(char string[]);
void freecolons(char** colons);
int beforeinorderid(char str1[], char str2[]);
char* getlinetype(char string[]);
Avl_Plant* searchAvlPlantById(Avl_Plant* root, char id[]);

#endif // UTILS_H
