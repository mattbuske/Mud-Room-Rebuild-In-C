/**
 * file ut.c
 * author mbuske
 * course CSC 454-470
 * date   September 13, 2013
 */

#include "ut.h"

//get the input string
void getInput(char* input, int blockSize) {
    printf("-> ", stdout);
    fflush(stdout);
    if (fgets(input, blockSize, stdin) != NULL) {
        //fix the string
        char *newLine = strchr(input, '\n');
        if (newLine != NULL) {
            *newLine = '\0';
        }
    } else {
        input = NULL;
    }
}

int getANumber(char* prompt) {
    int isCorrect = 0;
    char* input = (char*) malloc(8);
    int number = 0;
    while (1 != isCorrect) {
        printf("%s", prompt);
        getInput(input, 8);
        number = atoi(input);
        if (number > 1 && number <= 100) {
            isCorrect = 1;
            printf("Number %i ACCEPTED.\n", number);
        } else {
            printf("%s is not a valid number, Please pick a number between 2 and 100\n", input);
        }
    }
    free(input);
    return number;
}

void getStates(char* inputs[], int number, int base, Ut *states[]) {
    char* token;
    char* input;
    for (int i = 0; i < number; i++) {
        input = inputs[i];
        states[i] = (Ut*) malloc(sizeof (*states[i]));
        states[i]->number = i;
        if (base == 5) {
            token = strtok(input, " ");
            states[i]->rState = atoi(token);
            for (int j = 0; j < base - 1; j++) {
                token = strtok(NULL, " ");
                states[i]->n[j] = atoi(token);
            }
        } else {
            token = strtok(input, " ");
            states[i]->cType = atoi(token);
            token = strtok(NULL, " ");
            states[i]->cLocation = atoi(token);
        }
    }
}

void qS(Creature *a[], int l, int r) {
    if (l >= r) {
        return;
    }
    int i = l + 1;
    int j = r;
    if (a[l] == NULL) {
        return;
    }
    while (a[j] == NULL) {
        j--;
    }
    while (i <= j) {
        while (i <= j && a[l]->cNumber >= a[i]->cNumber) {
            i++;
        }
        while (i <= j && a[l]->cNumber < a[j]->cNumber) {
            j--;
        }
        if (i < j) {
            Creature *t = a[i];
            a[i] = a[j];
            a[j] = t;
        }
    }
    Creature *t = a[l];
    a[l] = a[j];
    a[j] = t;
    qS(a, l, j - 1);
    qS(a, i, r);
}

void shuffleArray(int a[]) {
    int v;
    for (int i = 0; i < 4; i++){
        v = rand() % 4;
        int t = a[i];
        a[i] = a[v];
        a[v] = t;
    }
}