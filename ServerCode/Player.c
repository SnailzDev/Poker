#include <stdlib.h>
#include <assert.h>
#include "Player.h"

/* TESTED SORT OF */
/* NEED VALGRIND */

Player *getNewPlayer(int startingPoints){
  Player *player = malloc(sizeof(Player));
  if (!player){
    //error
  }

  player->hand = getNewHand();
  player->points = startingPoints;
  player->fold = 0;
  return player;
}

void deletePlayer(Player *player){
  assert(player);
  deleteHand(player->hand);
  free(player);
  player = NULL;
}

int getPoints(Player *player){
  return player->points;
}

void addPoints(Player *player, int points){
  player->points += points;
}

void removePoints(Player *player, int points){
  player->points -= points;
}
