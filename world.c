/**
 * file world.c
 * author mbuske
 * course CSC 454-470
 * date   September 13, 2013
 */

#include <stdlib.h>
#include <stdio.h>
#include "world.h"
#include "ob.h"

Creature* buildPC() {
    Creature *c = (Creature*) malloc(sizeof (*c));
    c->cType = 0;
    c->pState = 3;
    c->chState = 3;
    c->res = 40;
    return c;
}

Creature* buildCreature(int cT, int cN, Creature *pc) {
    Creature *c = (Creature*) malloc(sizeof (*c));
    if (cT == 0) {
        pc->cNumber = cN;
        free(c);
        c = pc;
    } else if (cT == 1) {
        c->cType = 1;
        c->cNumber = cN;
        c->pState = 0;
        c->chState = 2;
    } else if (cT == 2) {
        c->cType = 2;
        c->cNumber = cN;
        c->pState = 2;
        c->chState = 0;
    }
    free(c->pc);
    c->pc = pc;
    return c;
}

//sets all but the room state and creatures
Room* buildRoom(int rN) {
    Room *r = (Room*) malloc(sizeof (*r));
    r->rNumber = rN;
    r->cCount = 0;
    return r;
}

//1 = true, 0 = false.
int roomAlreadyExists(Room *rooms[], int room, int size) {
    for (int i = 0; i < size; i++) {
        if (room == rooms[i]->rNumber) {
            return 1;
        }
    }
    return 0;
}

int findRoom(int rN, int count, Room *rooms[]) {
    for (int i = 0; i < count; i++) {
        if (rN == rooms[i]->rNumber) {
            return i;
        }
    }
    return -1;
}

void makeCreatures(Ut *states[], int creatureCount, Room *rooms[], int roomCount, Creature *pc) {
    for (int i = 0; i < creatureCount; i++) {
        Creature *c = buildCreature(states[i]->cType, i, pc);
        int pos = findRoom(states[i]->cLocation, roomCount, rooms);
        if (-1 == pos) {
            printf("\nroom not found due to bad user input!\n");
        } else {
            addCreature(c, rooms[pos]);
        }
    }
}

void makeRooms(Room *rooms[], Ut *states[], int size) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (roomAlreadyExists(rooms, i, count) != 1) {
            rooms[i] = buildRoom(i);
            count++;
        }
        rooms[i]->rState = states[i]->rState;
        for (int j = 0; j < 4; j++) {
            if (-1 != states[i]->n[j]) {
                if (roomAlreadyExists(rooms, states[i]->n[j], count) != 1) {
                    rooms[count] = buildRoom(states[i]->n[j]);
                    rooms[i]->n[j] = rooms[count];
                    count++;
                } else {
                    int pos = findRoom(states[i]->n[j], count, rooms);
                    if (-1 == pos) {
                        printf("\nError finding room. This is unlikely!\n\n");
                    } else {
                        rooms[i]->n[j] = rooms[pos];
                    }
                }
            } else {
                Room *t = NULL;
                rooms[i]->n[j] = t;
            }
        }
    }
}

int buildWorld(Room *rooms[], Creature *pc) {
    printf("\nWe will now build the world:\n");
    int DEBUG = 0;
    int numberOfRooms, numberOfCreatures, counter = 0;
    /*
     * Getting the number of rooms in the world
     */
    /*DEBUG*/
    if (0 == DEBUG) {
        numberOfRooms = getANumber("Please enter the number of rooms in the world ");
    } else {
        //numberOfRooms = 3;
		numberOfRooms = 10;
    }
    printf("%i rooms to create.\n", numberOfRooms);
    /*
     * Getting the states of the rooms
     * State - 0 = clean, 1 = half-dirty, 2 = dirty.
     * -1 means no neighbor
     * format -> state, n, s, e, w
     */
    char* inputs[numberOfRooms];
    printf("Please enter the states of the %i rooms you created in this format:\n", numberOfRooms);
    printf("state: 0 = clean, 1 = half-dirty, 2 = dirty -> neighbors: north, south, east, west.\n");
    printf("WARNING: Only One PC can be in the game at a time!\n");
    /*DEBUG*/
    if (0 == DEBUG) {
        for (int i = 0; i < numberOfRooms; i++) {
            //deleted at end of setting up rooms through inputs;
            char* input = (char*) malloc(16);
            printf("Room %i: state: north south east west ", i);
            getInput(input, 16);
            inputs[i] = input;
        }
    } else {
        /*char input[] = "0 -1 1 -1 2";
        inputs[0] = input;
        char input2[] = "2 0 -1 -1 -1";
        inputs[1] = input2;
        char input3[] = "2 -1 -1 0 -1";
        inputs[2] = input3;*/
		char input0[] = "1 1 3 2 4";
		inputs[0] = input0;
		char input1[] = "0 5 0 -1 7";
		inputs[1] = input1;
		char input2[] = "2 -1 9 -1 8";
		inputs[2] = input2;
		char input3[] = "2 0 -1 9 8";
		inputs[3] = input3;
		char input4[] = "1 7 8 0 -1";
		inputs[4] = input4;
		char input5[] = "1 -1 1 6 -1";
		inputs[5] = input5;
		char input6[] = "1 -1 -1 -1 5";
		inputs[6] = input6;
		char input7[] = "2 -1 4 1 -1";
		inputs[7] = input7;
		char input8[] = "0 4 -1 3 -1";
		inputs[8] = input8;
		char input9[] = "0 2 -1 -1 3";
		inputs[9] = input9;
    }
    Ut * roomStates[100];
    getStates(inputs, numberOfRooms, 5, roomStates);
    makeRooms(rooms, roomStates, numberOfRooms);
    printf("Rooms created\n");
    for (int i = 0; i < numberOfRooms; i++) {
        free(roomStates[i]);
        if (DEBUG == 0) {
            free(inputs[i]);
        }
    }
    /*DEBUG*/
    if (0 == DEBUG) {
        numberOfCreatures = getANumber("Please enter the number of creatures in the world ");
    } else {
        numberOfCreatures = 27;
    }
    printf("%i creatures to create.\n", numberOfCreatures);
    /*
     * Getting the creature info
     */
    char* inputsT[numberOfCreatures];
    printf("Please enter the creature type and the location for the %i creatures in this format:\n", numberOfCreatures);
    printf("type: 0 for PC, 1 for animal, 2 for NPC -> location: the room number it is in.\n");
    /*DEBUG*/
    if (0 == DEBUG) {
        for (int i = 0; i < numberOfCreatures; i++) {
            char* input = (char*) malloc(16);
            printf("Creature %i: ", i);
            getInput(input, 16);
            inputsT[i] = input;
        }
    } else {
        //0-10
        char inputt4[] = "0 0";
        inputsT[0] = inputt4;
        char inputt5[] = "2 0";
        inputsT[1] = inputt5;
        char inputt6[] = "1 0";
        inputsT[2] = inputt6;
        char inputt7[] = "1 0";
        inputsT[3] = inputt7;
		
        char inputt8[] = "1 1";
        inputsT[4] = inputt8;
        char inputt9[] = "1 1";
        inputsT[5] = inputt9;
		
        char input10[] = "2 2";
        inputsT[6] = input10;
        char input11[] = "2 2";
        inputsT[7] = input11;
        char input12[] = "2 2";
        inputsT[8] = input12;
        char input13[] = "2 2";
        inputsT[9] = input13;
		
        char input14[] = "2 3";
        inputsT[10] = input14;
        char input15[] = "2 3";
        inputsT[11] = input15;
		
        char input16[] = "2 4";
        inputsT[12] = input16;
        char input17[] = "2 4";
        inputsT[13] = input17;
        char input18[] = "1 4";
        inputsT[14] = input18;
        char input19[] = "1 4";
        inputsT[15] = input19;
		
        char input20[] = "2 5";
        inputsT[16] = input20;
        char input21[] = "1 5";
        inputsT[17] = input21;
		
        char input22[] = "1 6";
        inputsT[18] = input22;
        char input23[] = "2 6";
        inputsT[19] = input23;
        
		char input24[] = "2 7";
        inputsT[20] = input24;
        char input25[] = "2 7";
        inputsT[21] = input25;
		
		char input26[] = "1 8";
        inputsT[22] = input26;
        char input27[] = "1 8";
        inputsT[23] = input27;
		char input28[] = "1 8";
        inputsT[24] = input28;
		
		char input29[] = "1 9";
        inputsT[25] = input29;
		char input30[] = "1 9";
        inputsT[26] = input30;
    }
    Ut * creatureInfo[100];
    getStates(inputsT, numberOfCreatures, 2, creatureInfo);
    makeCreatures(creatureInfo, numberOfCreatures, rooms, numberOfRooms, pc);
    for (int i = 0; i < numberOfCreatures; i++) {
        free(creatureInfo[i]);
        if (DEBUG == 0) {
            free(inputsT[i]);
        }
    }
    return numberOfRooms;
}