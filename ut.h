/**
 * file ut.h
 * author mbuske
 * course CSC 454-470
 * date   September 13, 2013
 */

#ifndef UT_H
#define UT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ob.h"

typedef struct ut {
    int number;
    int cType;
    int cLocation;
    int rState;
    int n[4];
} Ut;

void getInput(char* input, int blockSize);
int getANumber(char* prompt);
void getStates(char* inputs[], int number, int base, Ut *states[]);
void qS(Creature *a[], int l, int r);
void shuffleArray(int a[]);

#endif