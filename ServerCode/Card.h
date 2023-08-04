#ifndef CARD_H
#define CARD_H

enum Face {SPADES, CLUBS, HEARTS, DIAMONDS, NONE};
enum Value {TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE, NONEX};

struct Card {
  enum Face face;
  enum Value value;
  int id;
  int exists;
};

typedef struct Card Card;


//returns new card with unique id
Card getNewCard(enum Face face, enum Value value);

int getCardInt(Card card);

Card getNonexistantCard();

#endif
