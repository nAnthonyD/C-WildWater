#ifndef DISTRIBUTIONAVL_H
#define DISTRIBUTIONAVL_H

#include <stdlib.h>
#include <string.h>
#include "managestructures.h"
#include "utils.h"

Avl_Distribution* createAvlDistribution(DistributionNode* distribution);
Avl_Distribution* rotateLeftDistribution(Avl_Distribution* a);
Avl_Distribution* rotateRightDistribution(Avl_Distribution* a);
Avl_Distribution* doubleRotateLeftDistribution(Avl_Distribution* root);
Avl_Distribution* doubleRotateRightDistribution(Avl_Distribution* root);
Avl_Distribution* balanceAvlDistribution(Avl_Distribution* a);

Avl_Distribution* insertAvlDistribution(Avl_Distribution* root, DistributionNode* distribution, int* heightchanged);
DistributionNode* createDistributionNode(char* phrase);
Avl_Distribution* searchAvlDistributionById(Avl_Distribution* root, char id[]);

#endif

