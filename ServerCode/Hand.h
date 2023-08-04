#ifndef HAND_H
#define HAND_H

#include "Card.h"

struct Hand{
  Card cardArray[52];
};

typedef struct Hand Hand;

//Get empty hand
Hand *getNewHand();

//Delete hand
void deleteHand(Hand *hand);

//get cards
Card getCard(Hand *hand, int index);

void addCard(Hand *hand, Card card);

Hand *getNewDeck();

void shuffleHand(Hand *hand);

Card popCard(Hand *hand);

#endif
