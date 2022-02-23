/**
 * file main.c
 * author mbuske
 * course CSC 454-470
 * date   September 13, 2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ut.h"
#include "ob.h"
#include "world.h"

//compile gcc main.c ob.c world.c ut.c -std=c99 -g -o main

void printMenu() {
    printf("Help Menu: Commands:\n\n");
    printf("exit - Exits the program\n");
    printf("help - Brings up the help menu\n");
    printf("look - Describes the room the player is in\n");
    printf("clean - Cleans the current room\n");
    printf("dirty - Dirties the current room\n");
    printf("north - Moves the player to the room to the room to the north if it exists\n");
    printf("south - Moves the player to the room to the room to the south if it exists\n");
    printf("east - Moves the player to the room to the room to the east if it exists\n");
    printf("west - Moves the player to the room to the room to the west if it exists\n\n");
    printf("creatureName - This is not a command, creatureName is to be replaced by the desired creature's name\n\n");
    printf("creatureName:look - Forces the creature denoted by creatureName to look around the room\n");
    printf("creatureName:north - Forces the creature denoted by creatureName to enter the room to the north\n");
    printf("creatureName:south - Forces the creature denoted by creatureName to enter the room to the south\n");
    printf("creatureName:east - Forces the creature denoted by creatureName to enter the room to the east\n");
    printf("creatureName:west - Forces the creature denoted by creatureName to enter the room to the west\n");
    printf("creatureName:clean - Forces the creature denoted by creatureName to clean the current room\n");
    printf("creatureName:dirty - Forces the creature denoted by creatureName to dirty the current room\n");
}

//returns 1 if true, 0 if false
int checkWin(int respect) {
    if (80 <= respect) {
        return 1;
    }
    return 0;
}

//returns 1 if true, 0 if false
int checkLose(int respect) {
    if (0 >= respect) {
        return 1;
    }
    return 0;
}

//return 1 if won or lost, 0 if neither.
int checkRespect(int respect) {
    printf("After this action the player's respect is %i\n\n", respect);
    if (checkWin(respect) == 1) {
        printf("Congratulations! You Won!\n");
        return 1;
    } else if (checkLose(respect) == 1) {
        printf("Game Over. Perhaps you need to practice more...\n");
        return 1;
    }
    return 0;
}

//returns 0 = not recognized, 1 = recognized, 2 = exit.
int decide(int len, char *one, char *two, Creature *pc) {
    if (2 == len) {
        int cN = atoi(one);
        if (strcmp(one, "0") == 0 || 0 != cN) {
            int pos = findCreature(cN, pc->location);
            if (-1 != pos) {
                //get the creature in question
                Creature *a = pc->location->creatures[pos];
                if (strcmp(two, "north") == 0) {
                    creatureLeave(a, 0);
                } else if (strcmp(two, "south") == 0) {
                    creatureLeave(a, 1);
                } else if (strcmp(two, "east") == 0) {
                    creatureLeave(a, 2);
                } else if (strcmp(two, "west") == 0) {
                    creatureLeave(a, 3);
                } else if (strcmp(two, "look") == 0) {
                    printR(pc->location);
                } else if (strcmp(two, "clean") == 0) {
                    if (0 != a->location->rState) {
                        printf("\n%i cleaned room %i\n", a->cNumber, pc->location->rNumber);
                        int type = a->cType;
                        if (1 == type) {
                            happy(a, a->location, 1);
                        } else if (2 == type) {
                            angry(a, a->location, 1);
                        }
                        changeRoomState(pc->location, 0);
                        printf("\n");
                    }
                } else if (strcmp(two, "dirty") == 0) {
                    if (2 != a->location->rState) {
                        printf("\n%i dirtied room %i\n", a->cNumber, pc->location->rNumber);
                        int type = a->cType;
                        if (2 == type) {
                            happy(a, a->location, 1);
                        } else if (1 == type) {
                            angry(a, a->location, 1);
                        }
                        changeRoomState(pc->location, 2);
                        printf("\n");
                    }
                } else if (strcmp(two, "exit") == 0) {
                    printf("\nExiting...\nGoodbye.");
                    return 2;
                } else if (strcmp(two, "help") == 0) {
                    printMenu();
                } else {
                    //command not recognized
                    return 0;
                }
            } else {
                //creature doesnt exist in this room
                printf("Creature Does Not Exist in this room!");
            }
        } else {
            //command not recognized.
            return 0;
        }
    } else if (1 == len) {
        if (strcmp(one, "north") == 0) {
            creatureLeave(pc, 0);
        } else if (strcmp(one, "south") == 0) {
            creatureLeave(pc, 1);
        } else if (strcmp(one, "east") == 0) {
            creatureLeave(pc, 2);
        } else if (strcmp(one, "west") == 0) {
            creatureLeave(pc, 3);
        } else if (strcmp(one, "look") == 0) {
            printR(pc->location);
        } else if (strcmp(one, "clean") == 0) {
            if (0 != pc->location->rState) {
                printf("\nPC cleaned the room %i.\n", pc->location->rNumber);
                changeRoomState(pc->location, 0);
            }
        } else if (strcmp(one, "dirty") == 0) {
            if (2 != pc->location->rState) {
                printf("\nPC dirtied the room %i.\n", pc->location->rNumber);
                changeRoomState(pc->location, 2);
            }
        } else if (strcmp(one, "exit") == 0) {
            printf("\nExiting...\nGoodbye.");
            return 2;
        } else if (strcmp(one, "help") == 0) {
            printMenu();
        } else {
            //command not recognized
            return 0;
        }
    } else {
        printf("Null Command - Please enter it again.\n");
        return 1;
    }
    return 1;
}

int playGame(Creature *pc) {
    const char *exit = "exit";
    //rec is bool, 0 = false, 1 = true
    int len, rec = 1;
    char *input = (char*) malloc(16);
    while (strcmp(input, exit) != 0) {
        //start prompt
        rec = 1;
        //use ut.h getInput();
        getInput(input, 16);
        if (NULL != input) {
            char *one, *two;
            //split the string to get the command
            one = strtok(input, ":");
            two = strtok(NULL, ":");
            if (two != NULL) {
                len = 2;
            } else {
                len = 1;
            }
            //test for length.
            int result = decide(len, one, two, pc);
            if (1 == result) {
                //check the game one win or loose and check respect
                if (checkRespect(pc->res) == 1) {
                    free(input);
                    return 0;
                }
            } else if (0 == result) {
                printf("The command %s was not recognized, please input it again.\n", input);
            } else if (2 == result) {
                free(input);
                //exit game
                return 0;
            }
        } else {
            //command entered is NULL
            printf("Null Command - Please enter it again.\n");
        }
    }
    free(input);
    //end game logic
    return 0;
}

void wipe(Room * rooms[], int count) {
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < rooms[i]->cCount; j++) {
            free(rooms[i]->creatures[j]);
        }
        free(rooms[i]);
    }
}

int main(int argc, char** argv) {
    Room * rooms[100];
    Creature *pc = buildPC();
    int count = buildWorld(rooms, pc);
    printMenu();
    printf("\n\n");
    playGame(pc);
    wipe(rooms, count);
    return (EXIT_SUCCESS);
}