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


Avl_Distribution* createAvlDistribution(DistributionNode* distribution){
    Avl_Distribution* newone = malloc(sizeof(Avl_Distribution));
    if (newone == NULL){
        exit(1);
    }
    newone->current = distribution;
    newone->ls = NULL;
    newone->rs = NULL;
    newone->balance = 0;
    return newone;
}


Avl_Distribution* rotateLeftDistribution(Avl_Distribution* a) {
    Avl_Distribution* pivot = a->rs;
    a->rs = pivot->ls;
    pivot->ls = a;
    int eq_a = a->balance;
    int eq_p = pivot->balance;
    a->balance = eq_a - max(eq_p,0) - 1;
    int val1 = min(eq_a - 2, eq_a + eq_p - 2);
    pivot->balance = min(val1, eq_p - 1);
    a = pivot;
    return a;
}

Avl_Distribution* rotateRightDistribution(Avl_Distribution* a) {
    Avl_Distribution* pivot = a->ls;
    a->ls = pivot->rs;
    pivot->rs = a;
    int eq_a = a->balance;
    int eq_p = pivot->balance;
 	a->balance = eq_a - min(eq_p,0) + 1;
    int val1 = max(eq_a + 2, eq_a + eq_p + 2);
    pivot->balance = max(val1, eq_p + 1);
    a = pivot;
    return a;
}

Avl_Distribution* doubleRotateLeftDistribution(Avl_Distribution* root) {
    root->rs = rotateRightDistribution(root->rs);
    return rotateLeftDistribution(root);
}

Avl_Distribution* doubleRotateRightDistribution(Avl_Distribution* root) {
    root->ls = rotateLeftDistribution(root->ls);
    return rotateRightDistribution(root);
}

Avl_Distribution* balanceAvlDistribution(Avl_Distribution* a) {
    if (a->balance >= 2) {
        if (a->rs->balance >= 0) {
            return rotateLeftDistribution(a);
        } else {
            return doubleRotateLeftDistribution(a);
        }
    } else if (a->balance <= -2) {
        if (a->ls->balance <= 0) {
            return rotateRightDistribution(a);
        } else {
            return doubleRotateRightDistribution(a);
        }
    }
    return a;
}

Avl_Distribution* insertAvlDistribution(Avl_Distribution* root, DistributionNode* distribution, int* heightchanged){
    if (root == NULL){
		*heightchanged = 1;
		return createAvlDistribution(distribution);
	}
	else{
		if (beforeinorderid(distribution->id,root->current->id) == 1){
			root->ls = insertAvlDistribution(root->ls,distribution,heightchanged);
			*heightchanged = -1;
		}
		else if (beforeinorderid(distribution->id,root->current->id) == -1){
			root->rs = insertAvlDistribution(root->rs,distribution,heightchanged);
			*heightchanged = 1;
		}
		else{
			*heightchanged = 0;
			return root;
		}
	}
	if (*heightchanged != 0){
		root->balance += *heightchanged;
		root = balanceAvlDistribution(root);
		if (root->balance == 0){
			*heightchanged = 0;
		}
		else{
			*heightchanged = 1;
		}
	}
	return root;
}


DistributionNode* createDistributionNode(char* phrase){
    char** colons = malloc(sizeof(char*)*5);
    DistributionNode* newone = malloc(sizeof(DistributionNode));
    if (newone == NULL || colons == NULL){
        exit(1);
    }
    colons = getcolons(phrase);
    strcpy(newone->id,colons[2]);
    newone->leakage_rate = atof(colons[4])/100;
    strcpy(newone->parentid,colons[1]);
    newone->next = NULL;
    newone->head = NULL;
}

Avl_Distribution* searchAvlDistributionById(Avl_Distribution* root, char id[]){
    if (root == NULL){
        return NULL;
    }
    else{
        int found = beforeinorderid(id,root->current->id);
        if (found == 0){
            return root;
        }
        else{
            if (found == 1){
                return searchAvlDistributionById(root->ls,id);
            }
            else if (found == -1){
                return searchAvlDistributionById(root->rs,id);
            }
            else{
                return NULL;
                printf("Error in searchAvlDistributionById function\n");
            }
        }
    }
}