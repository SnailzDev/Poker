#include <stdlib.h>
#include <stdio.h>

#include "Hand.h"

/* TESTED */
/* NEED VALGRIND */

//Get empty hand
Hand *getNewHand(){ // PROB WORK -- VALGRIND
  Hand *hand = malloc(sizeof(Hand));
  if (!hand){
    //error
  }

  //Card cardArray[100];
  //hand->cardArray = malloc(sizeof(Card)*100);
  int i;
  for (i = 0; i < 52; i++){
    hand->cardArray[i] = getNonexistantCard();
  }
  return hand;
}

//Deletes a hand of cards
void deleteHand(Hand *hand){ // PROB WORK -- VALGRIND
  free(hand);
}

//Get a card with a specified i
Card getCard(Hand *hand, int i){ // WORKS
  return hand->cardArray[i];
}

//Adds a card to a hand at the first spot that is unoccupied
void addCard(Hand *hand, Card card){ // WORKS
  int i;
  for(i = 0; i<52; i++){
     if(!(hand->cardArray[i].exists)){
        hand->cardArray[i] = card;
        break;
     }
  }
}

//Creates a new hand that is filled with a proper deck of 52 cards
Hand *getNewDeck(){ // WORKS
   Hand *newhand = getNewHand();
   enum Face face = 0;
   int i;
   int x;
   for(i = 0; i<4; i++){
     if(i == 0){
       face = SPADES;
     } else
     if(i == 1){
       face = CLUBS;
     } else
     if(i == 2){
       face = HEARTS;
     } else
     if(i == 3){
       face = DIAMONDS;
     }
     x = 0 + 13*i;
     newhand->cardArray[x] = getNewCard(face, TWO);
     newhand->cardArray[x + 1] = getNewCard(face, THREE);
     newhand->cardArray[x + 2] = getNewCard(face, FOUR);
     newhand->cardArray[x + 3] = getNewCard(face, FIVE);
     newhand->cardArray[x + 4] = getNewCard(face, SIX);
     newhand->cardArray[x + 5] = getNewCard(face, SEVEN);
     newhand->cardArray[x + 6] = getNewCard(face, EIGHT);
     newhand->cardArray[x + 7] = getNewCard(face, NINE);
     newhand->cardArray[x + 8] = getNewCard(face, TEN);
     newhand->cardArray[x + 9] = getNewCard(face, JACK);
     newhand->cardArray[x + 10] = getNewCard(face, QUEEN);
     newhand->cardArray[x + 11] = getNewCard(face, KING);
     newhand->cardArray[x + 12] = getNewCard(face, ACE);
   }
   return newhand;
}

//Shuffles a full deck of cards
void shuffleHand(Hand *hand){ // WORKS -- NEED VALGRIND
  //Creates a new hand to put the shuffled hand into
  Hand *shuffle = getNewHand();
  //Goes through all 52 cards in a deck
  //
  //SEED THE RANDINT WITH THE CURRENT TIME
  int i;
  for(i = 0; i<52; i++){
    int loop = 1;
    //Will keep generating a new target for the shuffled hand until an empty spot for the card is found
    while(loop == 1){
      int target_i = rand() % 52;
      if(shuffle->cardArray[target_i].exists == 0){
        shuffle->cardArray[target_i] = hand->cardArray[i];
        loop = 0;
      }
    }
  }
  for (i = 0; i<52; i++){
    popCard(hand);
  }
  for (i = 0; i<52; i++){
    addCard(hand, getCard(shuffle, i));
  }
  deleteHand(shuffle);
}

//pop card (Hand. Takes card off top of deck and returns it)
Card popCard(Hand *hand){ // WORKS
  //topCard = newest card in deck
  int topCardIndex = 51;
  int i;
  for(i = 51; i>-1; i--){
    if(hand->cardArray[i].exists == 1){
      topCardIndex = i;
      break;
    }
  }

  Card card = hand->cardArray[topCardIndex];
  hand->cardArray[topCardIndex] = getNonexistantCard();
  return card;
}
