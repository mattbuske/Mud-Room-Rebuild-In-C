/**
 * file world.h
 * author mbuske
 * course CSC 454-470
 * date   September 13, 2013
 */

#ifndef WORLD_H
#define WORLD_H

#include <stdlib.h>
#include "ob.h"
#include "ut.h"

Creature *buildPC();
Creature *buildCreature(int cT, int cN, Creature *pc);
Room* buildRoom(int rN) ;
int roomAlreadyExists(Room *rooms[], int room, int size);
int findRoom(int rN, int count, Room *rooms[]);
void makeCreatures(Ut *states[], int creatureCount, Room *rooms[], int roomCount, Creature *pc);
void makeRooms(Room *rooms[], Ut *states[], int size);
int buildWorld(Room *rooms[], Creature *pc);

#endif