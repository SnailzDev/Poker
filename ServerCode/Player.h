#ifndef PLAYER_H
#define PLAYER_H

#include "Card.h"
#include "Hand.h"

struct Player{
  Hand *hand;
  int points;
  int inBy;
  int fold;
};

typedef struct Player Player;	//edited from typedef struct Player Player

Player *getNewPlayer();

void deletePlayer(Player *player);

int getPoints(Player *player);

void addPoints(Player *player, int points);

void removePoints(Player *player, int points);

#endif
