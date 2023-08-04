#include "Card.h"
#include <stdlib.h>

/* TESTED */


//returns new card with unique id

Card getNewCard(enum Face face, enum Value value){
  static int id = 0;
  Card card;
  card.face = face;
  card.value = value;
  //give every card a unique id
  card.id = id++;
  card.exists = 1;
  return card;
}

int getCardInt(Card card){
  return (card.face*13 + card.value + 1);
}

Card getNonexistantCard(){
  Card card;
  card.face = NONE;
  card.value = NONEX;
  card.id = -1;
  card.exists = 0;
  return card;
}
