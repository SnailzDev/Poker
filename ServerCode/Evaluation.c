#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Evaluation.h"

int DISABLE_TIE = 1; //returns only 1 winner for showdown if bool is enabled

char* getStringFromSuit(eSuit suit)
{
    char* temp;

    switch(suit)
    {
        case 0:
            temp = "C";
            break;
        case 1:
            temp = "D";
            break;
        case 2:
            temp = "H";
            break;
        case 3:
            temp = "S";
            break;
    }

    return temp;
}

char* getStringFromHandValue(eHandValue handValue)
{
    char* temp;

    switch(handValue)
    {
        case 0:
            temp = "Folded";
            break;
        case 1:
            temp = "High Card";
            break;
        case 2:
            temp = "Pair";
            break;
        case 3:
            temp = "Two Pairs";
            break;
        case 4:
            temp = "Three Of A Kind";
            break;
        case 5:
            temp = "Straight";
            break;
        case 6:
            temp = "Flush";
            break;
        case 7:
            temp = "Full House";
            break;
        case 8:
            temp = "Four of A Kind";
            break;
        case 9:
            temp = "Straight Flush";
            break;
        case 10:
            temp = "Royal Flush";
            break;
    }

    return temp;
}

void printCard(eCard* card)
{
    printf("%d%s", card->rank, getStringFromSuit(card->suit));
}
/*
void printCardArray(eCard* arr[], int size)
{
    printf("Printing all cards in current deck (%d cards):\n", size);
    for(int i=0;i < size; i++)
    {
        printCard(arr[i]);
        printf(" ");
    }
    printf("\n");
}
*/
eCard* makeCard(eSuit suit, int rank)
{
    eCard* newCard = (eCard*) malloc(sizeof(eCard));
    newCard->suit = suit;
    newCard->rank = rank;
    return newCard;
}

eCard* decodeCardID(int cardID)
{
    eSuit suit = ((cardID-1) / 13);
    int rank = ((cardID-1) % 13) + 2;
    return makeCard(suit, rank);
}

void deallocateCard(eCard* card)
{
    card->rank = 0;
    card->suit = 0;
    free(card);
}

void deallocatePlayer(ePlayer* player)
{
    deallocateCard(player->hand[0]);
    deallocateCard(player->hand[1]);
    player->handValue = 0;
    int i;
    for(i=0; i<5; i++)
    {
        deallocateCard(player->evaluationCards[i]);
    }
    player->evaluationCardsSize = 0;
    player->foldedHand = FALSE;
    free(player);
}

void deallocateBoard(eBoard* board)
{
    int i;
    for(i=0; i<5; i++)
    {
        if(i<4)
            deallocatePlayer(board->player[i]);
        deallocateCard(board->communityCards[i]);
    }
    free(board);
}

///////////////GAMEPLAY FUNCTIONS///////////////
//general card array sort by rank then suit
void sortCardArray(eCard* cardArr[], int size) //WORKS
{
    eCard* temp;
    eBool swapped;

    //fixed bubble sort (was only doing one pass and added second heuristic) ~Christian
    int i;
    int j;
    for(i=0 ; i < size-1 ; i++)
    {
        swapped = FALSE;
        for(j=0 ; j < size-i-1 ; j++)
        {
            //move higher value card up array with rank as first heuristic and suit as second
            if (cardArr[j]->rank > cardArr[j+1]->rank

                || ((cardArr[j]->rank == cardArr[j+1]->rank)
                    && (cardArr[j]->suit > cardArr[j+1]->suit)))
            {
                temp = cardArr[j+1];
                cardArr[j+1] = cardArr[j];
                cardArr[j] = temp;
                swapped = TRUE;
            }
        }
        if(swapped == FALSE)
            break;
    }
}

//returns the highest rank of all the player's current cards and updates player evaluation cards
int getHighCard(eCard* allCards[], eCard* evalCards[], int* evalCardSize, eHandValue* handValue)
{
    //loops and creates hand of Highcard and 4 Kicker cards return Highcard
    int i;
    for(i = 0; i < 5; i++)
    {
     evalCards[i] = allCards[i+2];
    }
    *evalCardSize = 5;
    *handValue = H;
    return allCards[6]->rank;
}

//checks for pairs returning the amount of pairs found (0,1,2) and sets evaluation cards if found
int countPairs(eCard* allCards[], eCard* evalCards[], int* evalCardSize, eHandValue* handValue)
{
    eCard* pairCards[4];
    int cardCount[13] = {0};
    int count = 0;

    // get count of all ranks
    int i;
    int j;
    for(i=0; i < 7;i++)
    {
        cardCount[allCards[i]->rank-2] += 1;
    }

    //loop through each rank
    for(i=12; i > -1;i--)
    {
        //ONLY if a pair is found for a rank, loop through each card
        if(cardCount[i] == 2)
        {
            for(j=6 ; j >-1 ; j--)
            {
                //if rank found, store card and increment counter
                if(i+2 == allCards[j]->rank)
                {
                    pairCards[count] = allCards[j];
                    count++;
                }
                //if 2 pairs found exit all card loop
                if(count >= 4)
                {
                    sortCardArray(pairCards, count);
                    break;
                }
            }
        }
        //if 2 pairs found rank loop
        if(count >= 4){break;}
    }

    if (count > 0) { *evalCardSize = 5;}

    switch(count/2)
    {
        case 1:
            *handValue = P;
            evalCards[4] = pairCards[1];
            evalCards[3] = pairCards[0];
            if(allCards[5]->rank != evalCards[4]->rank)
            {
                evalCards[2] = allCards[6];
                evalCards[1] = allCards[5];
                if(allCards[4]->rank != evalCards[4]->rank)
                {
                    evalCards[0] = allCards[4];
                }
                else
                {
                    evalCards[0] = allCards[2];
                }
            }
            else
            {
                if(allCards[6]->rank != evalCards[4]->rank)
                {
                    evalCards[2] = allCards[6];
                    evalCards[1] = allCards[3];
                    evalCards[0] = allCards[2];
                }
                else
                {
                    evalCards[2] = allCards[4];
                    evalCards[1] = allCards[3];
                    evalCards[0] = allCards[2];
                }
            }
            break;
        case 2:
            *handValue = TWOP;
            for(i=0; i<4; i++)
            {
                evalCards[i+1] = pairCards[i];
            }
            for(i=0; i<6; i++)
            {
                if (allCards[6-i]->rank != evalCards[4]->rank && allCards[6-i]->rank != evalCards[2]->rank)
                {
                    evalCards[0] = allCards[6-i];
                    break;
                }
            }
            break;
    }
    return count/2;
}

//returns ThreeK rank if ThreeK found, -1 if no threeK
int threeKRank(eCard* allCards[], eCard* evalCards[], int* evalCardSize, eHandValue* handValue)
{
    //loops through all 7 and checks if any cards ranks matches 3 and makes hand of 3 of a kind and 2 highest card
    int i;
    for (i = 6; i > 1; i--) {
        if (allCards[i]->rank == allCards[i - 1]->rank
            && allCards[i - 1]->rank == allCards[i - 2]->rank) {
            int j;
            for (j = 0; j < 3; j++)
            {
                evalCards[j+2] = allCards[i-j];

            }
            int k;
            for(k=0; k<6; k++)
            {
                //highest card not in 3 k
                if (allCards[6-k]->rank != evalCards[4]->rank)
                {
                    evalCards[1] = allCards[6-k];
                    break;
                }
            }
            int l;
            for(l=0; l<5; l++)
            {
                //second highest card not in 3 k
                if (allCards[5-l]->rank != evalCards[4]->rank && allCards[5-l]->rank != evalCards[1]->rank)
                {
                    evalCards[0] = allCards[5-l];
                    break;
                }
            }
            *evalCardSize = 5;
            *handValue = THREEK;
            return allCards[i]->rank;
        }
    }
    return -1;
}

//returns straight rank if straight found, -1 if no straight
int straightRank(eCard* allCards[], eCard* evalCards[], int* evalCardSize, eHandValue* handValue)
{
    int straightCount = 1;
    int straightIndex[5] = {0,-1,-1,-1,-1};

    //counts if there are consecutive numbers and breaks if there is more than 4

    int i;
    int j;
    for(i = 0; i < 6; i++)
    {
        if (allCards[i+1]->rank - allCards[i]->rank == 1)
        {
            straightIndex[straightCount] = i+1;
            straightCount++;

            if (straightCount > 4)
            {
                break;
            }
        }
        //if no consecutive rank found, reset straightCount and index
        else if (allCards[i]->rank != allCards[i + 1]->rank) {
            straightCount = 1;
            straightIndex[0] = i+1;
            for(j=1; j<5 ; j++) {straightIndex[j] = 0;}
        }
    }
    if(straightCount != 5)
        straightCount = 1;
    //makes hand of straight found
    if(straightCount > 1)
    {
        for(i=0; i < 5 ; i++)
        {
            evalCards[i] = allCards[straightIndex[i]];
        }
        *evalCardSize = 5;
        *handValue = S;
        return evalCards[*evalCardSize-1]->rank;
    }

    return -1;
}


//checks for flush
//returns suit if flush found, -1 if no flush
int flushSuit(eCard* allCards[], eCard* evalCards[], int* evalCardSize, eHandValue* handValue)
{
    int count;
    int cardIndex[5];

    //loops through each suit
    int suit;
    int reset;
    int card;
    int j;
    for(suit=0; suit < 4; suit++)
    {
        //reset count for each suit
        count = 0;
        for(reset=0; reset < 5; reset++)
        {
            cardIndex[reset] = 0;
        }
        //goes through each card to see if any matches and counts how many matches
        for(card=0; card < 7; card++)
        {
            if(allCards[card]->suit == suit)
            {
                cardIndex[count] = card;
                count++;
                if(count > 4)
                {
                    //makes hand of 5 same suit hand
                    for(j=0; j<5;j++)
                    {
                        evalCards[j] = allCards[cardIndex[j]];
                    }
                    *handValue = F;
                    *evalCardSize = count;
                    return suit;
                } //flush == true returns suit value
            }
        }
   }
    return -1;// -1 means no flush
}


//returns fourK rank if fourK found, -1 if no fourK
int fourKRank(eCard* allCards[], eCard* evalCards[], int* evalCardSize, eHandValue* handValue)
{
    //loops through all 7 and checks if any cards ranks matches 4 and makes hand of 4 of a kind and 1 highest card
    int i;
    for (i = 6; i > 2; i--)
    {
        if (allCards[i]->rank == allCards[i - 1]->rank
            && allCards[i - 1]->rank == allCards[i - 2]->rank
            && allCards[i - 2]->rank == allCards[i - 3]->rank)
        {
            int j;
            for (j = 0; j < 4; j++)
            {
                evalCards[j+1] = allCards[i-j];
            }
            *evalCardSize = 5;
            *handValue = FOURK;
            int k;
            for(k=0; k<6; k++)
            {
                if (allCards[6-k]->rank != evalCards[4]->rank)
                {
                    evalCards[0] = allCards[6-k];
                    break;
                }
            }
            return allCards[i]->rank;
        }
    }

    return -1;
}

//if flushSuit to calculate royal flush
eBool isRoyalFlush(eCard* allCards[], eCard* evalCards[], eSuit flushSuit, int* evalCardSize, eHandValue* handValue)
{
    // card flags for RF
    eBool ten = FALSE;
    eBool jack = FALSE;
    eBool queen = FALSE;
    eBool king = FALSE;
    eBool ace = FALSE;
    int count = 0;

    eCard* tempEvalCards[5];

    //checks for flush and if cards in flush matches test cases(10,11,12,13,14)
    int i;
    for (i = 0; i < 7; i++)
    {

        if (allCards[i]->suit == flushSuit)
        {
            switch (allCards[i]->rank) {
                case 10:
                    ten = TRUE;
                    tempEvalCards[count] = allCards[i];
                    count++;
                    break;
                case 11: // Jack
                    jack = TRUE;
                    tempEvalCards[count] = allCards[i];
                    count++;
                    break;
                case 12: // Queen
                    queen = TRUE;
                    tempEvalCards[count] = allCards[i];
                    count++;
                    break;
                case 13: // King
                    king = TRUE;
                    tempEvalCards[count] = allCards[i];
                    count++;
                    break;
                case 14: // Ace
                    ace = TRUE;
                    tempEvalCards[count] = allCards[i];
                    count++;
                    break;
            }
        }
    }
    //if true then makes hand of RF
    if(ten && jack && queen && king && ace)
    {
        sortCardArray(evalCards, count);
        *evalCardSize = 5;
        *handValue = RF;
        return TRUE;
    }
    sortCardArray(tempEvalCards, count);
    return FALSE;
}

//sets individual player handValue and evaluationCards after hands and community cards have been assigned
int evaluateHand(eBoard* board, ePlayer* curPlayer)
{
    //REFERENCE VARIABLES FOR POINTER INITIALIZATION
    eHandValue HV = H;
    int ECS = 0;

    //IMPORTANT VALUES
    eHandValue* handValue = &HV;
    eCard* evalCards[5];
    int* evalCardSize = &ECS;

    eCard* allCards[7];

    eBool flagP = FALSE;
    eBool flagTWOP = FALSE;
    eBool flagTHREEK = FALSE;
    eBool flagS = FALSE;
    eBool flagF = FALSE;

    /* me guessing - ollie */
    eBool flagSF = FALSE;
    eBool flagRF = FALSE;

    int tempVal = -1;
    eSuit tempSuit;
    eCard* pairArr[5];
    eCard* threekArr[5];
    eCard* straightArr[5];
    eCard* flushArr[5];


    //add cards to all available cards and sort
    int i;
    for(i=0; i < 2; i++)
    {
        allCards[i] = curPlayer->hand[i];
    }
    for(i=2; i < 7; i++)
    {
        allCards[i] = board->communityCards[i-2];
    }
    sortCardArray(allCards, 7);

    // H = High Card
    getHighCard(allCards, evalCards, evalCardSize, handValue);
    //printCardArray(evalCards, *evalCardSize); WORKING

    // P = Pair, TWOP = Two Pairs
    switch(countPairs(allCards, evalCards, evalCardSize, handValue))
    {
        case 0:
            break;

        case 1:
            flagP = TRUE;
            break;

        case 2:
            flagTWOP = TRUE;
            break;

        default:
            printf("ERROR: Unknown value passed into evalPairSwitchCase.");
    }

    if(flagP || flagTWOP)
    {
        int i;
        for(i=0 ; i < *evalCardSize ; i++)
        {
            pairArr[i] = evalCards[i];
        }
        //sortCardArray(pairArr, *evalCardSize);
    }

    // THREEK = Three Of A Kind
    tempVal = threeKRank(allCards, evalCards, evalCardSize, handValue);
    if(tempVal != -1)
    {
        flagTHREEK = TRUE;
        tempVal = -1;
        int i;
        for(i=0 ; i < *evalCardSize ; i++)
        {
            threekArr[i] = evalCards[i];
        }
    }

    // S = Straight
    tempVal = straightRank(allCards, evalCards, evalCardSize, handValue);
    if(tempVal != -1)
    {
        flagS = TRUE;
        tempVal = -1;
    }

    // F = Flush
    tempVal = flushSuit(allCards, evalCards, evalCardSize, handValue);
    if(tempVal != -1)
    {
        tempSuit = tempVal;
        flagF = TRUE;
        tempVal = -1;
    }

    // FH = Full House (P + THREEK)
    if(flagP && flagTHREEK)
    {
        if(pairArr[4]->rank != threekArr[4]->rank)
        {
            int i;
            for(i=0; i<2 ; i++)
            {
                evalCards[i] = pairArr[4-i];
            }
            for(i=2; i<5 ; i++)
            {
                evalCards[i] =threekArr[i];
            }

            *evalCardSize = 5;
            *handValue = FH;
        }
    }

    // FOURK = Four Of A Kind
    fourKRank(allCards, evalCards, evalCardSize, handValue);

    // SF = Straight Flush
    if(flagF && flagS)
    {
        int i;
        for(i=0; i < 5; i++)
        {
            if(straightArr[i] != flushArr[i])
                break;
            if(i == 4)
            {
                flagSF = TRUE;
            }
        }

        if(flagSF)
        {
            for(i=0; i<5; i++)
            {
                evalCards[i] = straightArr[i];
            }
            *evalCardSize = 5;
            *handValue = SF;
        }
    }

    // RF = Royal Flush
    flagRF = isRoyalFlush(allCards, evalCards, tempSuit, evalCardSize, handValue);

    //assign values to player
    curPlayer->handValue = *handValue;
    for(i=0; i < *evalCardSize ; i++)
    {
        curPlayer->evaluationCards[i] = evalCards[i];
    }
    curPlayer->evaluationCardsSize = *evalCardSize;

    return *handValue;
}

//gets the current showdown winner from the boards current player handValues (evaluates kicker if needed)
// returns binary encoded integer with LSB being player 1 and MSB being player 4 ie. 0101 will be 1 and 3 tied
int getWinningPlayerNumber(int player1inGame, int player2inGame, int player3inGame, int player4inGame,
                            int player1card1, int player1card2,
                            int player2card1, int player2card2,
                            int player3card1, int player3card2,
                            int player4card1, int player4card2,
                            int communityCard1, int communityCard2, int communityCard3, int communityCard4, int communityCard5)

{
    //assignment to eval structs
    eBoard* board = (eBoard*) malloc(sizeof(eBoard));








    board->communityCards[0] = decodeCardID(communityCard1);
    board->communityCards[1] = decodeCardID(communityCard2);
    board->communityCards[2] = decodeCardID(communityCard3);
    board->communityCards[3] = decodeCardID(communityCard4);
    board->communityCards[4] = decodeCardID(communityCard5);

    int i;
    for(i = 0; i<4;i++)
    {





    board->player[i] = (ePlayer*) malloc(sizeof(ePlayer));
    }
    board->player[0]->foldedHand = !(player1inGame);
    board->player[0]->hand[0] = decodeCardID(player1card1);
    board->player[0]->hand[1] = decodeCardID(player1card2);

    board->player[1]->foldedHand = !(player2inGame);
    board->player[1]->hand[0] = decodeCardID(player2card1);
    board->player[1]->hand[1] = decodeCardID(player2card2);

    board->player[2]->foldedHand = !(player3inGame);
    board->player[2]->hand[0] = decodeCardID(player3card1);
    board->player[2]->hand[1] = decodeCardID(player3card2);

    board->player[3]->foldedHand = !(player4inGame);
    board->player[3]->hand[0] = decodeCardID(player4card1);
    board->player[3]->hand[1] = decodeCardID(player4card2);

    //evaluation
    for(i=0; i<4; i++)
    {
        evaluateHand(board, board->player[i]);
    }

    //showdown
    ePlayer* temp = board->player[0];
    int winner = 0b0001;
    int j;
    for (j=1; j<4; j++)
    {
        if(temp->handValue < board->player[j]->handValue)
        {
            temp = board->player[j];
            switch(j)
            {
                case 1:
                    winner = 0b0010;
                    break;
                case 2:
                    winner = 0b0100;
                    break;
                case 3:
                    winner = 0b1000;
                    break;
            }
        }
        else if(temp->handValue == board->player[j]->handValue)
        {
            int k;
            for (k = 4; k>-1; k--)
            {
                if (temp->evaluationCards[k]->rank < board->player[j]->evaluationCards[k]->rank)
                {
                    temp = board->player[j];
                    switch(j)
                    {
                            case 1:
                                winner = 0b0010;
                                break;
                            case 2:
                                winner = 0b0100;
                                break;
                            case 3:
                                winner = 0b1000;
                                break;
                        }
                    break;
                }
                else if (temp->evaluationCards[k]->rank > board->player[j]->evaluationCards[k]->rank)
                {
                    break;
                }
                else
                {
                    if(k == 0)
                    {
                        switch(j){
                            case 1:
                                winner = 0b0010 | winner;
                                break;
                            case 2:
                                winner = 0b0100 | winner;
                                break;
                            case 3:
                                winner = 0b1000 | winner;
                                break;
                        }
                    }
                }
            }
        }
    }
    if(DISABLE_TIE && (winner != 8) && (winner != 4) && (winner != 2) && (winner != 1))
    {
        if(winner & 8 == 8)
            return 8;
        else if(winner & 4 == 4)
            return 4;
        else if(winner & 2 == 2)
            return 2;
        else if(winner & 1 == 1)
            return 1;
    }

    //deallocateBoard(board);

    return winner;
}
