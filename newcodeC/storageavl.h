#ifndef STORAGEAVL_H
#define STORAGEAVL_H

#include <stdlib.h>
#include <string.h>
#include "managestructures.h"
#include "utils.h"

StorageNode* createStorageNode(char id[], float leakage_rate);

Avl_Storage* createAvlStorage(StorageNode* storage);
Avl_Storage* rotateLeftStorage(Avl_Storage* a);
Avl_Storage* rotateRightStorage(Avl_Storage* a);
Avl_Storage* doubleRotateLeftStorage(Avl_Storage* root);
Avl_Storage* doubleRotateRightStorage(Avl_Storage* root);
Avl_Storage* balanceAvlStorage(Avl_Storage* a);

Avl_Storage* insertAvlStorage(Avl_Storage* root, StorageNode* storage, int* heightchanged);
Avl_Storage* searchAvlStorageById(Avl_Storage* root, char id[]);
void showAvlStorageInorder(Avl_Storage* root);

#endif

