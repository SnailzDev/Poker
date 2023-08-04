#ifndef __EVALUATION_H__
#define __EVAULATION_H__

////////////// ENUMERATORS AND STRUCTS /////////////////
typedef enum eBool{FALSE, TRUE = !FALSE} eBool;
typedef enum eSuit{E_SPADES, E_CLUBS, E_HEARTS, E_DIAMONDS} eSuit;
typedef enum eHandValue{FOLDED, H, P, TWOP, THREEK, S, F, FH, FOURK, SF, RF} eHandValue;
////// In Order From Least to Greatest Value //////
// H = High Card
// P = Pair
// TWOP = Two Pairs
// THREEK = Three Of A Kind
// S = Straight
// F = Flush
// FH = Full House (P + THREEK)
// FOURK = Four Of A Kind
// SF = Straight Flush
// RF = Royal Flush

////////////// DATA STRUCTURES /////////////////
typedef struct eCard
{
    int rank; // jack(11),queen(12),king(13),ace(14)
    eSuit suit;
} eCard;

typedef struct ePlayer
{
    eCard* hand[2];
    // int points; //money to bet

    eHandValue handValue; //stores hand evaluation for current hand
    eCard* evaluationCards[5]; //stores cards already used to filter out and evaluate others for kicker
    int evaluationCardsSize;

    eBool foldedHand; //bool to keep track of player folding each round
} ePlayer;

typedef struct eBoard
{
    ePlayer* player[4]; //pointer to all players on board (max 4)
    eCard* communityCards[5];//flop 0-2, turn 3, river 4
} eBoard;


//////////////*************GAMEPLAY FUNCTIONS*****************/////////////////
//////////////////DEBUG FUNCTIONS//////////////////
//returns string from enumeration value
char* getStringFromSuit(eSuit suit);
char* getStringFromHandValue(eHandValue handValue);

// prints card value
void printCard(eCard* card);

//void printCardArray(eCard* arr[], int size)

eCard* makeCard(eSuit suit, int rank);

eCard* decodeCardID(int cardID);

void deallocateCard(eCard* card);

void deallocateBoard(eBoard* board);
///////////////GAMEPLAY FUNCTIONS///////////////
void sortCardArray(eCard* cardArr[], int size);

//returns the highest rank of all the player's current cards and updates player evaluation cards
int getHighCard(eCard* allCards[], eCard* evalCards[], int* evalCardSize, eHandValue* handValue);

//returns the amount of pairs found (0,1,2) and sets evaluation cards if found
int countPairs(eCard* allCards[], eCard* evalCards[], int* evalCardSize, eHandValue* handValue);

//returns ThreeK rank if ThreeK found, -1 if no threeK
int threeKRank(eCard* allCards[], eCard* evalCards[], int* evalCardSize, eHandValue* handValue);

//returns straight rank if straight found, -1 if no straight
int straightRank(eCard* allCards[], eCard* evalCards[], int* evalCardSize, eHandValue* handValue);

//returns suit if flush found, -1 if no flush
int flushSuit(eCard* allCards[], eCard* evalCards[], int* evalCardSize, eHandValue* handValue);

int fourKRank(eCard* allCards[], eCard* evalCards[], int* evalCardSize, eHandValue* handValue);

eBool isRoyalFlush(eCard* allCards[], eCard* evalCards[], eSuit flushSuit, int* evalCardSize, eHandValue* handValue);

//sets all player handValue and evaluationCards at start of round
int evaluateHand(eBoard* board, ePlayer* curPlayer);

//gets the current showdown winner from the boards current player handValues (evaluates kicker if needed)
int getWinningPlayerNumber(int player1inGame, int player2inGame, int player3inGame, int player4inGame,
                            int player1card1, int player1card2,
                            int player2card1, int player2card2,
                            int player3card1, int player3card2,
                            int player4card1, int player4card2,
                            int communityCard1, int communityCard2, int communityCard3, int communityCard4, int communityCard5);
#endif
