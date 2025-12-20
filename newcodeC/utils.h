#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "managestructures.h"

int max(int a, int b);
int min(int a, int b);

char** getcolons(const char* string);
void freecolons(char** colons);

int beforeinorderid(const char str1[], const char str2[]);
char* getlinetype(const char* string);

Avl_Plant* searchAvlPlantById(Avl_Plant* root, char id[]);

#endif

