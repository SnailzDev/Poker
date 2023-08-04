#include <stdio.h>

#include "Table.h"

int main(){
  /*

  Card card;
  card = getNewCard(SPADES, TWO);
  Card card2;
  card2 = getNewCard(DIAMONDS, KING);
  printf("CARD 1: %d\n", getCardInt(card));
  printf("CARD 2: %d\n", getCardInt(card2));
  */
  /*
  //e
  int i;
  Hand *hand = getNewDeck();
  for (i = 0; i < 52; i++){
    printf("CARD: %d\n", getCardInt(getCard(hand, i)));
  }
  printf("------- SHUFFLE -------\n");
  shuffleHand(hand);
  for (i = 0; i < 52; i++){
    printf("CARD: %d\n", getCardInt(getCard(hand, i)));
  }
  printf("------- POP 2 CARDS -------\n");
  popCard(hand);
  popCard(hand);
  for (i = 0; i < 52; i++){
    printf("CARD: %d\n", getCardInt(getCard(hand, i)));
  }
  printf("------- ADD ACE OF SPACES -------\n");
  addCard(hand, getNewCard(SPADES, ACE));
  for (i = 0; i < 52; i++){
    printf("CARD: %d\n", getCardInt(getCard(hand, i)));
  }
  deleteHand(hand);

  */
  /*
  Player *player = getNewPlayer(1000);
  //player->hand = getNewHand();
  deletePlayer(player);
  */




  /* 
  int i;

  Table table = getNewTable(getNewPlayer(100), getNewPlayer(100), getNewPlayer(100), getNewPlayer(100));
  printf("------- COM CARDS lol -------\n");
  for (i = 0; i < 52; i++){
    printf("CARD: %d\n", getCardInt(getCard(table.communityCards, i)));
  }
  drawCardToCommunityCards(table);

  printf("------------ DRAW CARD 2 COMM CARDS ------------\n");
  printf("------- COM CARDS lol -------\n");
  for (i = 0; i < 52; i++){
    printf("CARD: %d\n", getCardInt(getCard(table.communityCards, i)));
  }
  printf("------- player 1 cards -------\n");
  for (i = 0; i < 52; i++){
    printf("CARD: %d\n", getCardInt(getCard(table.player1->hand, i)));
  }
  printf("------------ DRAW CARD 2 PLAYER1 ------------\n");
  drawCardToPlayer(table, 1);
  printf("------- player 1 cards -------\n");
  for (i = 0; i < 52; i++){
    printf("CARD: %d\n", getCardInt(getCard(table.player1->hand, i)));
  }

  printf("---------the bucks---------\n");
  printf("player1: %d\nplayer2: %d\nplayer3: %d\nplayer4: %d\n", getPoints(table.player1), getPoints(table.player2), getPoints(table.player3), getPoints(table.player4));
  table.potPoints = 100;
  payOutPot(table, 1, 75);
  payOutPot(table, 2, 25);
  printf("------------ PAY OUT POT ------------\n");
  printf("---------the bucks---------\n");
  printf("player1: %d\nplayer2: %d\nplayer3: %d\nplayer4: %d\n", getPoints(table.player1), getPoints(table.player2), getPoints(table.player3), getPoints(table.player4));


  printf("---------the players---------\n");
  printf("dealer: %d\nSmall Blind: %d\nBig Blind: %d\nNextPlayer: %d\n", table.dealer, getSmallBlind(table), getBigBlind(table), getNextPlayer(getBigBlind(table)));
  table.dealer = getNextPlayer(table.dealer);
  printf("------------inc by 1---------------\n");
  printf("dealer: %d\nSmall Blind: %d\nBig Blind: %d\nNextPlayer: %d\n", table.dealer, getSmallBlind(table), getBigBlind(table), getNextPlayer(getBigBlind(table)));
  deleteTable(table);
  */ 

  //Betting Function Tests
  
  Table testtable = getNewTable(getNewPlayer(500), getNewPlayer(500), getNewPlayer(500), getNewPlayer(500));
  
  void DebugTable(){
  printf("Bets: Player 1 %d Player 2 %d Player 3 %d Player 4 %d \n", testtable.player1Bet, testtable.player2Bet, testtable.player3Bet, testtable.player4Bet);
  printf("Pot Points %d \n", testtable.potPoints);
  printf("Turn %d \n", testtable.turn);
  printf("Player Turn %d \n", testtable.playerTurn);
  printf("whoHasHighestBet %d \n", testtable.whoHasHighestBet);
  printf("Fold Count %d \n", testtable.foldCount);
  }
  
  /*
  testtable.player1->inBy = 400;
  testtable.player2->inBy = 400;
  testtable.player3->inBy = 400;
  testtable.player4->inBy = 300;
  //printf("Player 1's points %d \n", testtable.player1->points);   
  */

  /*
  printf("Test of Bet \n");
  testtable.playerTurn = 1;
  printf("Player Turn %d", testtable.playerTurn);
  printf("Bet Results Player 1 %d", bet(testtable, 1, CALL_RAISE, 100));
  DebugTable();
  printf("Bet Results Player 2 %d", bet(testtable, 2, CALL_RAISE, 100));
  DebugTable();
  printf("Bet Results Player 3 %d", bet(testtable, 3, CALL_RAISE, 100));
  DebugTable();
  printf("Bet Results Player 4 %d", bet(testtable, 4, CALL_RAISE, 100));
  DebugTable();
  */

  /*
  //Code for testing Game Over. Segfaults as is.
  testtable.player1->fold = 1;
  testtable.player2->fold = 1;
  testtable.player3->fold = 1;
  testtable.foldCount = 3;
  gameOver(testtable);
  printf("Player 4's points %d \n", testtable.player4->points);
  */
 
}
