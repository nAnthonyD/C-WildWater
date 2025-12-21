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

StorageNode* createStorageNode(char id[], float leakage_rate){
	StorageNode* newone = malloc(sizeof(StorageNode));
	if (newone == NULL){
		exit(1);
	}
	strcpy(newone->id,id);
	newone->leakage_rate = leakage_rate;
    newone->flow = 0;
	newone->head = NULL;
	newone->next = NULL;
    newone->leaked_volume = 0;
	return newone;
}


Avl_Storage* createAvlStorage(StorageNode* storage){
    Avl_Storage* newone = malloc(sizeof(Avl_Storage));
    if (newone == NULL){
        exit(1);
    }
    newone->current = storage;
    newone->ls = NULL;
    newone->rs = NULL;
    newone->balance = 0;
    return newone;
}

Avl_Storage* rotateLeftStorage(Avl_Storage* a) {
    Avl_Storage* pivot = a->rs;
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

Avl_Storage* rotateRightStorage(Avl_Storage* a) {
    Avl_Storage* pivot = a->ls;
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

Avl_Storage* doubleRotateLeftStorage(Avl_Storage* root) {
    root->rs = rotateRightStorage(root->rs);
    return rotateLeftStorage(root);
}

Avl_Storage* doubleRotateRightStorage(Avl_Storage* root) {
    root->ls = rotateLeftStorage(root->ls);
    return rotateRightStorage(root);
}


Avl_Storage* balanceAvlStorage(Avl_Storage* a) {
    if (a->balance >= 2) {
        if (a->rs && a->rs->balance >= 0) {
            return rotateLeftStorage(a);
        } else {
            return doubleRotateLeftStorage(a);
        }
    } else if (a->balance <= -2) {
        if (a->ls && a->ls->balance <= 0) {
            return rotateRightStorage(a);
        } else {
            return doubleRotateRightStorage(a);
        }
    }
    return a;
}



Avl_Storage* insertAvlStorage(Avl_Storage* root, StorageNode* storage, int* heightchanged){
    if (root == NULL){
        *heightchanged = 1;
        return createAvlStorage(storage);
    }
    else{
        if (beforeinorderid(storage->id,root->current->id) == 1){
            root->ls = insertAvlStorage(root->ls,storage,heightchanged);
            *heightchanged = -1;
        }
        else if (beforeinorderid(storage->id,root->current->id) == -1){
            root->rs = insertAvlStorage(root->rs,storage,heightchanged);
            *heightchanged = 1;
        }
        else{
            *heightchanged = 0;
            return root;
        }
    }
    if (*heightchanged != 0){
        root->balance += *heightchanged;
        root = balanceAvlStorage(root);
        if (root->balance == 0){
            *heightchanged = 0;
        }
        else{
            *heightchanged = 1;
        }
    }
    return root;
}

Avl_Storage* searchAvlStorageById(Avl_Storage* root, char id[]){
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
                return searchAvlStorageById(root->ls,id);
            }
            else if (found == -1){
                return searchAvlStorageById(root->rs,id);
            }
            else{
                return NULL;
               
            }
        }
    }
}

void showAvlStorageInorder(Avl_Storage* root){
    if (root != NULL){
        showAvlStorageInorder(root->ls);
        printf("Storage id: %s , leakage rate: %f\n",root->current->id,root->current->leakage_rate);
        showAvlStorageInorder(root->rs);
    }
}

void freeAvlStorage(Avl_Storage* root){
    if (root != NULL){
        freeAvlStorage(root->ls);
        freeAvlStorage(root->rs);
        free(root);
    }
}

