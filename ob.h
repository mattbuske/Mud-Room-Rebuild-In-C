/**
 * file ob.h
 * author mbuske
 * course CSC 454-470
 * date   September 13, 2013
 */

#ifndef OB_H
#define OB_H

typedef struct creature {
    int cType;
    int cNumber;
    int chState;
    int pState;
    struct creature *pc;
    struct room *location;
    int res;
} Creature;

typedef struct room {
    int rNumber;
    int cCount;
    int rState;
    struct creature * creatures[10];
    struct room * n[4];
} Room;

char* decodeState(int state);
char* decodeDirection(int d);
int isFull(Room *room);
void angry(Creature *c, Room *r, int forced);
void happy(Creature *c, Room *r, int forced);
int canCreatureHandle(Creature *c);
void creatureReact(Creature *c);
int findCreature(int cN, Room *r);
void addCreature(Creature *c, Room *room);
int creatureLeaveForce(Creature *c, Room *r);
void allCreaturesReact(Creature *c, int nS);
void notifyCreatures(int nS, Room *r);
void changeRoomState(Room *r, int newState);
void tellCreature(Room *r, Creature *c);
void creatureLeave(Creature *c, int d);
void printC(Creature *c);
void printR(Room *room);

#endif