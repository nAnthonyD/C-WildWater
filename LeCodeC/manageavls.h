#ifndef MANAGEAVLS_H
#define MANAGEAVLS_H

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

#ifndef UTILS
#include "utils.h"
#define UTILS
#endif

DistributionNode* createDistributionNodeFromId(char id[], float leakage_rate);

#endif // MANAGEAVLS_H