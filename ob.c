/**
 * file ob.c
 * author mbuske
 * course CSC 454-470
 * date   September 13, 2013
 */

#include <stdio.h>
#include "ob.h"
#include "ut.h"

char* decodeState(int state) {
    if (state == 0) {
        return "clean";
    } else if (state == 1) {
        return "half-dirty";
    } else {
        return "dirty";
    }
}

char* decodeDirection(int d) {
    if (0 == d) {
        return "north";
    } else if (1 == d) {
        return "south";
    } else if (2 == d) {
        return "east";
    } else {
        return "west";
    }
}

//1=true, 0=false;
int isFull(Room *room) {
    if (room->cCount >= 10) {
        return 1;
    }
    return 0;
}

void angry(Creature *c, Room *r, int forced) {
    //animal 
    if (c->cType == 1) {
        if (r == c->pc->location) {
            if (forced == 0) {
                printf("%i the animal growled at you! respect -1!\n", c->cNumber);
                c->pc->res--;
            } else {
                printf("%i the animal bit you! respect -2!\n", c->cNumber);
                c->pc->res--;
                c->pc->res--;
            }
        }
        //npc
    } else if (c->cType == 2) {
        if (r == c->pc->location) {
            if (forced == 0) {
                printf("%i the human yelled at you! respect -1!\n", c->cNumber);
                c->pc->res--;
            } else {
                printf("%i the human punched you! respect -2!\n", c->cNumber);
                c->pc->res--;
                c->pc->res--;
            }
        }
    }
}

void happy(Creature *c, Room *r, int forced) {
    //animal 
    if (c->cType == 1) {
        if (r == c->pc->location) {
            if (forced == 0) {
                printf("%i the animal licked your face! respect +1!\n", c->cNumber);
                c->pc->res++;
            } else {
                printf("%i the animal jumped with joy! respect +2!\n", c->cNumber);
                c->pc->res++;
                c->pc->res++;
            }
        }
        //npc
    } else if (c->cType == 2) {
        if (r == c->pc->location) {
            if (forced == 0) {
                printf("%i the human smiled at you! respect +1!\n", c->cNumber);
                c->pc->res++;
            } else {
                printf("%i the human kissed you! respect +2!\n", c->cNumber);
                c->pc->res++;
                c->pc->res++;
            }
        }
    }
}

//1 for true, 0 for false;
int canCreatureHandle(Creature *c) {
    //animal
    if (c->cType == 0) {
        return 1;
    } else {
        if (c->location->rState == c->chState) {
            return 0;
        } else {
            return 1;
        }
    }
}

void creatureReact(Creature *c) {
    if (c->pc->location != c->location) {
        if (canCreatureHandle(c) == 0) {
            //animal
            if (c->cType == 1) {
                printf("%i cleaned the room %i.\n", c->cNumber, c->location->rNumber);
                changeRoomState(c->location, 0);
                //npc
            } else if (c->cType == 2) {
                printf("%i dirtied the room %i.\n", c->cNumber, c->location->rNumber);
                changeRoomState(c->location, 2);
            }
        }
    }
}

int findCreature(int cN, Room *r) {
    int mid;
    int left = 0;
    int right = r->cCount - 1;
    while (left <= right) {
        mid = (left + right) / 2;
        int number = r->creatures[mid]->cNumber;
        if (cN > number) {
            left = mid + 1;
        } else if (cN < number) {
            right = mid - 1;
        } else {
            return mid;
        }
    }
    return -1;
}

void addCreature(Creature *c, Room *room) {
    if (isFull(room) == 0) {
        room->creatures[room->cCount] = c;
        room->creatures[room->cCount]->location = room;
        room->cCount++;
        qS(room->creatures, 0, room->cCount - 1);
    }
}

//1=true, 0=false;
int creatureLeaveForce(Creature *c, Room *r) {
    int counter = 0;
    int rC = 0;
    int compare[] = {0, 1, 2, 3};
    shuffleArray(compare);
    int pos = findCreature(c->cNumber, r);
    if (pos != -1) {
        angry(c, r, 0);
        while (counter < 4) {
            rC = compare[counter];
            if (r->n[rC] != NULL) {
                if (isFull(r->n[rC]) == 1) {
                    counter++;
                } else {
                    addCreature(c, r->n[rC]);
                    r->creatures[pos] = r->creatures[r->cCount - 1];
                    r->creatures[r->cCount - 1] = NULL;
                    r->cCount--;
                    qS(r->creatures, 0, r->cCount - 1);
                    printf("%i had left the room %i and entered the room %i.\n", c->cNumber, r->rNumber, r->n[rC]->rNumber);
                    return 1;
                }
            } else {
                counter++;
            }
        }
        Creature *t = r->creatures[pos];
        r->creatures[pos] = r->creatures[r->cCount - 1];
        r->creatures[r->cCount - 1] = NULL;
        r->cCount--;
        qS(r->creatures, 0, r->cCount - 1);
        printf("%i could not find an empty room and thus exploded with rage!\n", t->cNumber);
        //free the memory for the exploded creature
        free(t);
        for (int i = 0; i < r->cCount; i++){
            angry(r->creatures[i], r, 0);
        }
        return 0;
    } else {
        printf("If you are seeing this error there is a bug in the code!\n");
        return 0;
    }
}

void allCreaturesReact(Creature *c, int nS) {
    if (canCreatureHandle(c) == 0) {
        if (creatureLeaveForce(c, c->location) == 1) {
            creatureReact(c);
        }
    } else if (nS == c->pState) {
        happy(c, c->location, 0);
    } else {
        angry(c, c->location, 0);
    }
}

void notifyCreatures(int nS, Room *r) {
    int cc = 0;
    int temp = r->cCount;
    int len = r->cCount;
    for (int i = 0; i < len; i++) {
        allCreaturesReact(r->creatures[i - cc], nS);
        if ((temp - 1) == r->cCount) {
            cc++;
            temp = r->cCount;
        }
    }
}

void changeRoomState(Room *r, int newState) {
    if (newState == 0) {
        if (r->rState == 1) {
            r->rState = 0;
        } else if (r->rState == 2) {
            r->rState = 1;
        }
    } else if (newState == 2) {
        if (r->rState == 1) {
            r->rState = 2;
        } else if (r->rState == 0) {
            r->rState = 1;
        }
    }
    notifyCreatures(newState, r);
    printf("\nThe Room %i is now %s.\n", r->rNumber, decodeState(r->rState));
}

void tellCreature(Room *r, Creature *c) {
    int pos = findCreature(c->cNumber, r);
    if (pos != -1) {
        creatureReact(r->creatures[pos]);
    }
}

void creatureLeave(Creature *c, int d) {
    Room *r = c->location;
    int pos = findCreature(c->cNumber, r);
    if (pos != -1) {
        if (r->n[d] != NULL) {
            if (isFull(r->n[d]) == 1) {
                printf("\nThe room %i is full.\n", r->n[d]->rNumber);
                angry(c, r, 0);
            } else {
                if (r->creatures[pos]->cType == 0) {
                    printf("\nPC has left room %i  and entered room %i.\n", r->rNumber, r->n[d]->rNumber);
                } else {
                    printf("\n%i has left room %i  and entered room %i.\n", c->cNumber, r->rNumber, r->n[d]->rNumber);
                }
                addCreature(c, r->n[d]);
                r->creatures[pos] = r->creatures[r->cCount - 1];
                r->creatures[r->cCount - 1] = NULL;
                r->cCount--;
                qS(r->creatures, 0, r->cCount - 1);
                tellCreature(r->n[d], c);
            }
        } else {
            printf("\nThat Room Does Not Exist. Please pick a different Room.\n");
        }
    } else {
        printf("\n%i does not exist in this room.\n", c->cNumber);
    }
}

void printC(Creature *c) {
    printf("Creature #%i: Type: %i, CHState: %i, pState: %i, Room Unknown.\n", c->cNumber, c->cType, c->chState, c->pState);
}

void printR(Room *room) {
    printf("\nRoom %i\n", room->rNumber);
    printf("State: %s\n", decodeState(room->rState));
    printf("Neighbors: \n");
    for (int i = 0; i < 4; i++){
        if (NULL == room->n[i]) {
            printf("No Neighbor");
        } else {
            printf("%i", room->n[i]->rNumber);
        }
        printf(" to the %s\n", decodeDirection(i));
    }
    printf("\nContains: \n");
    for (int i = 0; i < room->cCount; i++){
        int cT = room->creatures[i]->cType;
        if (0 == cT) {
            printf("PC\n");
        } else if (1 == cT) {
            printf("animal %i\n", room->creatures[i]->cNumber);
        } else {
            printf("human %i\n", room->creatures[i]->cNumber);
        }
    }
    printf("\n");
}