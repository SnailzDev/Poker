#ifndef TABLE_H
#define TABLE_H

#include "Card.h"
#include "Hand.h"
#include "Player.h"

#include "Evaluation.h"


struct Table{
  int joined;
  Player *player1;
  Player *player2;
  Player *player3;
  Player *player4;
  int player1Bet;
  int player2Bet;
  int player3Bet;
  int player4Bet;
  int dealer;
  //int currentPlayer;
  int potPoints;
  //like show all the cards at end of game
  int showCards;
  Hand *communityCards;
  Hand *deck;
  //1 = no cards on table1
  //2 = 3 cards on table
  //3 = 4 cards on table
  //4 = 5 cards on table
  //5 = rap up game bro lol
  int turn;
  int playerTurn; //whos turn
  int whoHasHighestBet;
  int highestBet; // current highest bet

  int foldCount;
};

typedef struct Table Table;

enum BetType{CHECK, CALL_RAISE, FOLD};

Table getNewTable(Player *player1, Player *player2, Player *player3, Player *player4, int dealer);

void deleteTable(Table table);

//add table to all cards !!!
void drawCardToCommunityCards();

void drawCardToPlayer(Table table, int playerNumber);

//Places bet with type and value. call = check for zero. For fold, value doesnt matter.
//Return 1 if valid bet placed. Return 0 if not possible
Table bet(Table table, int playerNumber, enum BetType betType, int value);

void payOutPot(Table table, int playerNumber, int amtToPay);

Player *getPlayer(Table table, int playerNumber);

int getWinningPlayerNumber(int player1In, int player2In, int player3In, int player4In,
int player1Card1, int player1Card2, int player2Card1, int player2Card2,
int player3Card1, int player3Card2, int player4Card1, int player4Card2,
int comCard1, int comCard2, int comCard3, int comCard4, int comCard5);

Table resetTable(Table table);

Table nextTurn(Table table);

Table gameOver(Table table);

void payOut(Table table, int playerNumber, int amt);

int getPlayerBet(Table table, int playerNumber);


//Returns player num
int getSmallBlind(Table table);
//Returns player num
int getBigBlind(Table table);
int getNextPlayer(int i);
void incrementDealer(Table table);
#endif
