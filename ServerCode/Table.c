#include "Table.h"


#include <stdlib.h>
#include <stdio.h>


//starts dealer at player 1
Table getNewTable(Player *player1, Player *player2, Player *player3, Player *player4, int dealer){ // WORKS -- NEED VALGRIND
  Table table;
  table.player1 = player1;
  table.player2 = player2;
  table.player3 = player3;
  table.player4 = player4;
  player1->hand = getNewHand();
  player2->hand = getNewHand();
  player3->hand = getNewHand();
  player4->hand = getNewHand();
  table.showCards = 0;
  table.dealer = dealer;
  table.potPoints = 0;
  table.turn = 1;
  table.playerTurn = 1; //gotta work on this
  table.communityCards = getNewHand();
  table.deck = getNewDeck();
  shuffleHand(table.deck);

  table.highestBet = -1;
  table.whoHasHighestBet = 0;

  table.player1Bet = 0;
  table.player2Bet = 0;
  table.player3Bet = 0;
  table.player4Bet = 0;

  table.foldCount = 0;
  return table;
}

Table resetTable(Table table){
  popCard(table.player1->hand);
  popCard(table.player1->hand);
  popCard(table.player2->hand);
  popCard(table.player2->hand);
  popCard(table.player3->hand);
  popCard(table.player3->hand);
  popCard(table.player4->hand);
  popCard(table.player4->hand);
  table.showCards = 0;
  table.dealer = getNextPlayer(table.dealer);
  table.potPoints = 0;
  table.turn = 1;
  table.playerTurn = 0;
  deleteHand(table.communityCards);
  table.communityCards = getNewHand();
  deleteHand(table.deck);
  table.deck = getNewDeck();
  shuffleHand(table.deck);

  table.highestBet = -1;
  table.whoHasHighestBet = 0;

  table.foldCount = 0;

  table.player1Bet = 0;
  table.player2Bet = 0;
  table.player3Bet = 0;
  table.player4Bet = 0;
  return table;
}

//Deletes all players too
void deleteTable(Table table){ // WORKS -- NEED VALGRIND
  deletePlayer(table.player1);
  deletePlayer(table.player2);
  deletePlayer(table.player3);
  deletePlayer(table.player4);
  deleteHand(table.communityCards);
  deleteHand(table.deck);
}

int getPlayerBet(Table table, int playerNumber){
  if (playerNumber == 1){
    return table.player1Bet;
  }
  if (playerNumber == 2){
    return table.player2Bet;
  }
  if (playerNumber == 3){
    return table.player3Bet;
  }
  if (playerNumber == 4){
    return table.player4Bet;
  }
}

//add table to all cards !!!
void drawCardToCommunityCards(Table table){ // WORKS
  addCard(table.communityCards, popCard(table.deck));
}


void drawCardToPlayer(Table table, int playerNumber){ // WORKS
  if (playerNumber == 1){
    addCard(table.player1->hand, popCard(table.deck));
  }
  if (playerNumber == 2){
    addCard(table.player2->hand, popCard(table.deck));
  }
  if (playerNumber == 3){
    addCard(table.player3->hand, popCard(table.deck));
  }
  if (playerNumber == 4){
    addCard(table.player4->hand, popCard(table.deck));
  }
}

//return 2. betting over. nxt round
//return 3. game over lol

Table nextTurn(Table table){  //Works
  table.potPoints += table.player1Bet + table.player2Bet + table.player3Bet + table.player4Bet;
  table.player1->inBy += table.player1Bet;
  table.player2->inBy += table.player2Bet;
  table.player3->inBy += table.player3Bet;
  table.player4->inBy += table.player4Bet;
  table.player1Bet = 0;
  table.player2Bet = 0;
  table.player3Bet = 0;
  table.player4Bet = 0;


  //Update turn number in the imported table
  table.turn++;
  table.playerTurn = getNextPlayer(getNextPlayer(getNextPlayer(table.dealer))); //3rd to right from dealer
  table.highestBet = -1;
  table.whoHasHighestBet = 0;

  while (getPlayer(table, table.playerTurn)->fold == 1) {
    table.playerTurn = getNextPlayer(table.playerTurn);
  } //Inc player turn to next non folded player

  //draw cards
  if (table.turn == 2){
    drawCardToCommunityCards(table);
    drawCardToCommunityCards(table);
    drawCardToCommunityCards(table);
  }
  if (table.turn == 3){
    drawCardToCommunityCards(table);
  }
  if (table.turn == 4){
    drawCardToCommunityCards(table);
  }
  //printf("DEBUG: TURN NUMBER = %d\n", table.turn);
  //If the turn number or foldCount in the imported table meets the following criteria, gameover.
  if (table.turn == 5 || table.foldCount == 3){
    return gameOver(table);
  }
  return table;
}

Table gameOver(Table table){ //Needs to be tested
  //printf("DEBUG: GAME over\n");
  if (table.foldCount == 3){
    //Game win by all but 1 folded
    if (!table.player1->fold){
      //printf("DEBUG: game win 1 by fold");
      payOut(table, 1, table.player1->inBy);
    }
    if (!table.player2->fold){
      //printf("DEBUG: game win 2 by fold");
      payOut(table, 2, table.player2->inBy);
    }
    if (!table.player3->fold){
      //printf("DEBUG: game win 3 by fold");
      payOut(table, 3, table.player3->inBy);
    }
    if (!table.player4->fold){
      //printf("DEBUG: game win 4 by fold");
      payOut(table, 4, table.player4->inBy);
    }
  } else {
    //printf("DEBUG: non fold game over\n");
    //Regular game
    while (table.player1->inBy != 0 || table.player2->inBy != 0 || table.player3->inBy != 0 || table.player4->inBy != 0){
      int playerLowestIn;
      int lowestInBy = -1;
      int i;
      for (i = 1; i < 5; i++){
        int inBy = getPlayer(table, i)->inBy;
        if (inBy == 0 || getPlayer(table, i)->fold){
          //player paid out or folded. Not eligable for win money
          continue;
        }
        if (inBy < lowestInBy || lowestInBy == -1){
          playerLowestIn = i;
          lowestInBy = inBy;
        }
      }
      //Lowest player
      //Should work i think. I think i did folded so they are out if its true regardless of
      //if they actually folded but not completely sure.
      //------HAS SYNTAX ERRORS --------
/*
      int winningPlayerNumber = getWinningPlayerNumber(!table.player1->fold, !table.player2->fold,
      !table.player3->fold, !table.player4->fold,
      getCardInt(getCard(table.player1->hand, 0)), getCardInt(getCard(table.player1->hand, 1)),
      getCardInt(getCard(table.player2->hand, 0)), getCardInt(getCard(table.player2->hand, 1)),
      getCardInt(getCard(table.player3->hand, 0)), getCardInt(getCard(table.player3->hand, 1)),
      getCardInt(getCard(table.player4->hand, 0)), getCardInt(getCard(table.player4->hand, 1)),
      getCardInt(getCard(table.communityCards)), 0), getCardInt(getCard(table.communityCards)), 1),
      getCardInt(getCard(table.communityCards)), 2), getCardInt(getCard(table.communityCards)), 3),
      getCardInt(getCard(table.communityCards)), 4));
*/

      int winningPlayerNumber = getWinningPlayerNumber(!table.player1->fold, !table.player2->fold,
        !table.player3->fold, !table.player4->fold, getCardInt(getCard(table.player1->hand, 0)),
      getCardInt(getCard(table.player1->hand, 1)), getCardInt(getCard(table.player2->hand, 0)),
      getCardInt(getCard(table.player2->hand, 1)), getCardInt(getCard(table.player3->hand, 0)),
      getCardInt(getCard(table.player3->hand, 1)), getCardInt(getCard(table.player4->hand, 0)),
      getCardInt(getCard(table.player4->hand, 1)), getCardInt(getCard(table.communityCards, 0)),
      getCardInt(getCard(table.communityCards, 1)), getCardInt(getCard(table.communityCards, 2)),
      getCardInt(getCard(table.communityCards, 3)), getCardInt(getCard(table.communityCards, 4)));

      //printf("DEBUG: winningPlayerNumber = %d\n", winningPlayerNumber);
      //just for testing setting as 1
      //int winningPlayerNumber = 1;
      switch(winningPlayerNumber){
        case 1:
          winningPlayerNumber = 1;
          break;
        case 2:
          winningPlayerNumber = 2;
          break;
        case 4:
          winningPlayerNumber = 3;
          break;
        case 8:
          winningPlayerNumber = 4;
          break;
      }
      //printf("DEBUG: i = %d\n", i);
      payOut(table, winningPlayerNumber, getPlayer(table, playerLowestIn)->inBy);
      getPlayer(table, playerLowestIn)->fold = 1;

    }
    //And game paid out
  }
  table.showCards = 1;
  return table;
}
//give player Number everyone elses inBy up to amt

void payOut(Table table, int playerNumber, int amt){ //WORKS
  Player *player = getPlayer(table, playerNumber);

  player->points += table.player1->inBy >= amt ? amt : table.player1->inBy;
  table.player1->inBy = table.player1->inBy >= amt ? table.player1->inBy - amt : 0;

  player->points += table.player2->inBy >= amt ? amt : table.player2->inBy;
  table.player2->inBy = table.player2->inBy >= amt ? table.player2->inBy - amt : 0;

  player->points += table.player3->inBy >= amt ? amt : table.player3->inBy;
  table.player3->inBy = table.player3->inBy >= amt ? table.player3->inBy - amt : 0;

  player->points += table.player4->inBy >= amt ? amt : table.player4->inBy;
  table.player4->inBy = table.player4->inBy >= amt ? table.player4->inBy - amt : 0;
}


//Places bet with type and value. call = check for zero. For fold, value doesnt matter.
//Return 0 = invalid Bet
//Return 1 = valid bet
//Return 2 = betting over
//Return 3 = game over
Table bet(Table table, int playerNumber, enum BetType betType, int value){ //Needs to be tested

  if (table.showCards == 1){
    //game is over so ignore bet
    return table;
  }
  if (playerNumber != table.playerTurn){
    return table; //invalid bet. not right player
  }
  if (getPlayer(table, playerNumber)->points < value){
    return table; //invalid bet. not enough points
  }
  if (betType == FOLD){
    getPlayer(table, playerNumber)->fold = 1; //set player folded
    table.foldCount++;
  } else {

    //Player aint folded. we in the game boiiiiis!
    if ((value + getPlayerBet(table, playerNumber)) > table.highestBet){
      table.highestBet = value + getPlayerBet(table, playerNumber);
      table.whoHasHighestBet = playerNumber; //update highest bet
    }
    if ((value + getPlayerBet(table, playerNumber)) == table.highestBet){
      //Call attempt
    }
    if ((value + getPlayerBet(table, playerNumber)) < table.highestBet){
      if (value == getPlayer(table, playerNumber)->points){
        //Going all in fine
      } else if (getPlayer(table, playerNumber)->points == 0 && table.turn > 0){
        //already in so fine
      } else {
        return table;
      }
    }

    //VALID BET PLACED

    //EXECUTE BET
    //This code will execute bet
    if (playerNumber == 1){
      if (table.player1->points < value){
        return table;
      }
      table.player1->points -= value;
      table.player1Bet += value;
      //printf("DEBUG: player1 BETTING %d\n", value);
    }
    if (playerNumber == 2){
      if (table.player2->points < value){
        return table;
      }
      table.player2->points -= value;
      table.player2Bet += value;
      //printf("DEBUG: player2 BETTING %d\n", value);
    }
    if (playerNumber == 3){
      if (table.player3->points < value){
        return table;
      }
      table.player3->points -= value;
      table.player3Bet += value;
      //printf("DEBUG: player3 BETTING %d\n", value);
    }
    if (playerNumber == 4){
      if (table.player4->points < value){
        return table;
      }
      table.player4->points -= value;
      table.player4Bet += value;
      //printf("DEBUG: player4 BETTING %d\n", value);
    }
  }

  //Infinite Loop Fixed
  do {
    table.playerTurn = getNextPlayer(table.playerTurn);
  } while (getPlayer(table, table.playerTurn)->fold == 1); //Inc player turn to next non folded player


  //printf("DEBUG:: table fold count = %d\n", table.foldCount);
  if (table.playerTurn == table.whoHasHighestBet){ //Lowkey done
    return nextTurn(table);

  }
  //not done

  return table;
}


//Pays out the pot to the winning player
//not being used i think
void payOutPot(Table table, int playerNumber, int amtToPay){ // WORKS
  if(playerNumber == 1){
    addPoints(table.player1, amtToPay);
  }
  if(playerNumber == 2){
    addPoints(table.player2, amtToPay);
  }
  if(playerNumber == 3){
    addPoints(table.player3, amtToPay);
  }
  if(playerNumber == 4){
    addPoints(table.player4, amtToPay);
  }
  table.potPoints = 0;
}

//If no player, return NULL
Player *getPlayer(Table table, int playerNumber){ //i dont think we need to test lol
  if(playerNumber == 1){
    return table.player1;
  }
  if(playerNumber == 2){
    return table.player2;
  }
  if(playerNumber == 3){
    return table.player3;
  }
  if(playerNumber == 4){
    return table.player4;
  }
  else{
    return NULL;
  }
}

int getSmallBlind(Table table){ // WORKS
    return (table.dealer%4)+1;
}
int getBigBlind(Table table){ // WORKS
    return (getSmallBlind(table)%4)+1;
}
int getNextPlayer(int i){ // WORKS
  return (i%4)+1;
}
