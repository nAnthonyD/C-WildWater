#ifndef MANAGEFILES_H
#define MANAGEFILES_H

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

#include "avls.h"

void testAvlPlantInorder(Avl_Plant* root);
void normalizeToWindows(char phrase[]);
Avl_Plant* getAllPlantsFromFile(FILE* file);
void getPlantColAndProcVolume(FILE* file, Avl_Plant* root);
void freeAvlPlant(Avl_Plant* root);
float leakage(char id[],FILE* file, Avl_Plant* root,char* biggestleakageid,char* biggestleak_parent,float* biggestleakage);

#endif // MANAGEFILES_H

