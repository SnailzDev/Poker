#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <assert.h>

#include "Card.h"
#include "Hand.h"
#include "Player.h"
#include "Table.h"

const int MAX_CLIENTS = 4;
const int PORT_NUMBER = 10122;


const int DEFAULT_STARTING_POINTS = 10000;
const int SMALL_BLIND = 50;
const int BIG_BLIND = 100;

void FatalError(const char *ErrorMsg);
int MakeServerSocket(uint16_t PortNo);
int ProcessRequest(int DataSocketFD);
void ServerMainLoop(int ServSocketFD, int Timeout);
int handleRequest(char SendBuf[256], char RecvBuf[256]);
int join(int playerNumber);
int sdown();
int startGame();
int move(int playerNumber, int bet);
int getIntFromRecvBuf(char RecvBuf[256], int i, char endChar);

Table table;
int tableUp;
int player1Joined;
int player2Joined;
int player3Joined;
int player4Joined;
char playerName1[100];	//changed these 4 lines because of errors
char playerName2[100];	//originally they were formatted as:
char playerName3[100];	//char[100] playerName#;
char playerName4[100];

int player1NameSize;
int player2NameSize;
int player3NameSize;
int player4NameSize;

void FatalError(const char *ErrorMsg){
    perror(ErrorMsg);
    exit(20);
}

//IGNORE THIS
int MakeServerSocket(uint16_t PortNo){
    int ServSocketFD;
    struct sockaddr_in ServSocketName;

    /* create the socket */
    ServSocketFD = socket(PF_INET, SOCK_STREAM, 0);
    if (ServSocketFD < 0)
    {   FatalError("service socket creation failed");
    }
    /* bind the socket to this server */
    ServSocketName.sin_family = AF_INET;
    ServSocketName.sin_port = htons(PortNo);
    ServSocketName.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(ServSocketFD, (struct sockaddr*)&ServSocketName,
		sizeof(ServSocketName)) < 0)
    {   FatalError("binding the server to a socket failed");
    }
    /* start listening to this socket */
    if (listen(ServSocketFD, MAX_CLIENTS) < 0)	/* Might have off by 1
error */
    {   FatalError("listening on socket failed");
    }
    return ServSocketFD;
}

//IGNORE
int ProcessRequest(int DataSocketFD){
    int  l, n;

    char SendBuf[256];
    char RecvBuf[256];

    n = read(DataSocketFD, RecvBuf, sizeof(RecvBuf)-1); //reading
    if (n < 0)
    {
      FatalError("reading from data socket failed");
    }
    RecvBuf[n] = 0;

    int shutdown = handleRequest(SendBuf, RecvBuf);


    l = strlen(SendBuf);

    n = write(DataSocketFD, SendBuf, l); //writing
    if (n < 0)
    {
      FatalError("writing to data socket failed");
    }
    return shutdown;
} /* end of ProcessRequest */


//IGNORE
void ServerMainLoop(int ServSocketFD, int Timeout) {
    int DataSocketFD;	/* socket for a new client */
    socklen_t ClientLen;
    struct sockaddr_in ClientAddress;	/* client address we connect with
*/
    fd_set ActiveFDs;	/* socket file descriptors to select from */
    fd_set ReadFDs;	/* socket file descriptors ready to read from */
    struct timeval TimeVal;
    int res, i;

    FD_ZERO(&ActiveFDs);		/* set of active sockets */
    FD_SET(ServSocketFD, &ActiveFDs);	/* server socket is active */
    int Shutdown = 0;
    while(!Shutdown)
    {   ReadFDs = ActiveFDs;
	TimeVal.tv_sec  = Timeout / 1000000;	/* seconds */
	TimeVal.tv_usec = Timeout % 1000000;	/* microseconds */
	/* block until input arrives on active sockets or until timeout */
	res = select(FD_SETSIZE, &ReadFDs, NULL, NULL, &TimeVal);
	if (res < 0)
	{   FatalError("wait for input or timeout (select) failed");
	}
	if (res == 0)	/* timeout occurred */
	{
#ifdef DEBUG
	    printf("Handling timeout...\n");
#endif
	}
	else		/* some FDs have data ready to read */
	{   for(i=0; i<FD_SETSIZE; i++)
	    {   if (FD_ISSET(i, &ReadFDs))
		{   if (i == ServSocketFD)
		    {	/* connection request on server socket */
#ifdef DEBUG
			printf("Accepting new client %d...\n", i);
#endif
			ClientLen = sizeof(ClientAddress);
			DataSocketFD = accept(ServSocketFD,
				(struct sockaddr*)&ClientAddress,
&ClientLen);
			if (DataSocketFD < 0)
			{
        FatalError("data socket creation (accept)failed");
			}
#ifdef DEBUG
			printf("Client %d connected from %s:%hu.\n", i,
				inet_ntoa(ClientAddress.sin_addr),
				ntohs(ClientAddress.sin_port));
#endif
			FD_SET(DataSocketFD, &ActiveFDs);
		    }
		    else
		    {   /* active communication with a client */
#ifdef DEBUG
			printf("Dealing with client %d...\n", i);
#endif
			Shutdown = ProcessRequest(i);
#ifdef DEBUG
			printf("Closing client %d connection.\n", i);
#endif
			close(i);
			FD_CLR(i, &ActiveFDs);
		    }
		}
	    }
	}
    }
} /* end of ServerMainLoop */


int main(int argc, char *argv[])
{
    int ServSocketFD;	/* socket file descriptor for service */
    printf("Starting...\n");
    ServSocketFD = MakeServerSocket(PORT_NUMBER);

    //Initialize SRAND to seed random integers
    srand(time(NULL));

    //GLOBAL DECLARED
    player1Joined = 0;
    player2Joined = 0;
    player3Joined = 0;
    player4Joined = 0;

    tableUp = 0;

    table = getNewTable(getNewPlayer(DEFAULT_STARTING_POINTS), getNewPlayer(DEFAULT_STARTING_POINTS), getNewPlayer(DEFAULT_STARTING_POINTS), getNewPlayer(DEFAULT_STARTING_POINTS), 1);

    strncpy(playerName1, "Player has not yet joined...", sizeof("Player has not yet joined...")-1);
    strncpy(playerName2, "Player has not yet joined...", sizeof("Player has not yet joined...")-1);
    strncpy(playerName3, "Player has not yet joined...", sizeof("Player has not yet joined...")-1);
    strncpy(playerName4, "Player has not yet joined...", sizeof("Player has not yet joined...")-1);
    player1NameSize = 28;
    player2NameSize = 28;
    player3NameSize = 28;
    player4NameSize = 28;

    //END GLOBAL DELCARED

    ServerMainLoop(ServSocketFD, 250000);
    printf("\nShutting down.\n");
    close(ServSocketFD);
    return 0;
}


int handleRequest(char SendBuf[256], char RecvBuf[256]){
  int i;
  for (i = 0; i < 256; i++){
    SendBuf[i] = 0;
  }
  char from[256];
  if (RecvBuf[0] == 'z'){
    if (RecvBuf[1] - '0' >= 1 && RecvBuf[1] - '0' <= 4){
      char name[100];
      //printf("DEBUG: NAME before add %s\n", name);
      if (RecvBuf[1] - '0' == 1){
        strncpy(name, playerName1, player1NameSize);
        //printf("DEBUG: SIZE: %d ||| NAME for z = %s\n", player1NameSize, name);
      }
      if (RecvBuf[1] - '0' == 2){
        strncpy(name, playerName2, player2NameSize);
        //printf("DEBUG: %d NAME for z = %s\n", player2NameSize, name);
      }
      if (RecvBuf[1] - '0' == 3){
        strncpy(name, playerName3, player3NameSize);
        //printf("DEBUG: %d NAME for z = %s\n", player3NameSize, name);
      }
      if (RecvBuf[1] - '0' == 4){
        strncpy(name, playerName4, player4NameSize);
        //printf("DEBUG: %d NAME for z = %s\n", player4NameSize, name);
      }
      sprintf(from, "oz%s|", name);
      for (i = 0; i < 100; i++){
        name[i] = 0;
      }
    } else {
      sprintf(from, "nz");
    }
    strncpy(SendBuf, from, 255);
  }

  if (RecvBuf[0] == 'y'){
    if (RecvBuf[1] - '0' >= 1 && RecvBuf[1] - '0' <= 4){
      char *name;
      //idk if this works but its gonna be cool if it does
      name = strtok(RecvBuf+2, "|");
      //printf("DEBUG: name VAR = %s\n", name);
      //printf("DEBUG: SIZE = %d\n", strlen(name));
      if (RecvBuf[1] - '0' == 1){
        strncpy(playerName1, name, strlen(name));
        player1NameSize = strlen(name);
        sprintf(from, "oy");
      }
      if (RecvBuf[1] - '0' == 2){
        strncpy(playerName2, name, strlen(name));
        player2NameSize = strlen(name);
        sprintf(from, "oy");
      }
      if (RecvBuf[1] - '0' == 3){
        strncpy(playerName3, name, strlen(name));
        player3NameSize = strlen(name);
        sprintf(from, "oy");
      }
      if (RecvBuf[1] - '0' == 4){
        strncpy(playerName4, name, strlen(name));
        player4NameSize = strlen(name);
        sprintf(from, "oy");
      }
    } else {
      sprintf(from, "ny");
    }
    /*
    printf("DEBUG: PLAYER 1 NAME = %s\n", playerName1);
    printf("DEBUG: SIZE = %d\n", player1NameSize);
    printf("DEBUG: PLAYER 2 NAME = %s\n", playerName2);
    printf("DEBUG: SIZE = %d\n", player2NameSize);
    printf("DEBUG: PLAYER 3 NAME = %s\n", playerName3);
    printf("DEBUG: SIZE = %d\n", player3NameSize);
    printf("DEBUG: PLAYER 4 NAME = %s\n", playerName4);
    printf("DEBUG: SIZE = %d\n", player4NameSize);
    */
    strncpy(SendBuf, from, 255);
  }
  //SEND MOVE
  if (RecvBuf[0] == 'm'){
    if (!tableUp){
      sprintf(from, "nm");
      strncpy(SendBuf, from, 255);
    } else {
      int playerNum = RecvBuf[1] - '0';

      int bet;
      if (RecvBuf[2] == 'f'){
        bet = -1;
      } else {
        bet = getIntFromRecvBuf(RecvBuf, 2, '|');
      }
      //printf("DEBUG: playerNumber of bet = %d\n", playerNum);
      //printf("DEBUG: bet of bet = %d\n", bet);
      int returnValBad = move(playerNum, bet);
      //int returnValBad = 1;
      if (!returnValBad){
        sprintf(from, "nm");
      } else {
        sprintf(from, "om");
      }
    }
    strncpy(SendBuf, from, 255);
  }

  //GET GAME
  if (RecvBuf[0] == 'g'){
    if (!tableUp){
      sprintf(from, "gn");
      strncpy(SendBuf, from, 255);
    }
    else {
    int TURN = table.playerTurn;
    int CARD1 = getCardInt(getCard(table.communityCards, 0));
    int CARD2 = getCardInt(getCard(table.communityCards, 1));
    int CARD3 = getCardInt(getCard(table.communityCards, 2));
    int CARD4 = getCardInt(getCard(table.communityCards, 3));
    int CARD5 = getCardInt(getCard(table.communityCards, 4));
    //Just ignore this. had to inverse it for silly reason. like i said, ignore lol;
    int SHOWN = table.showCards ? 0 : 1;
    int PLAYER1_CARD1 = getCardInt(getCard(table.player1->hand, 0));
    int PLAYER1_CARD2 = getCardInt(getCard(table.player1->hand, 1));
    int PLAYER2_CARD1 = getCardInt(getCard(table.player2->hand, 0));
    int PLAYER2_CARD2 = getCardInt(getCard(table.player2->hand, 1));
    int PLAYER3_CARD1 = getCardInt(getCard(table.player3->hand, 0));
    int PLAYER3_CARD2 = getCardInt(getCard(table.player3->hand, 1));
    int PLAYER4_CARD1 = getCardInt(getCard(table.player4->hand, 0));
    int PLAYER4_CARD2 = getCardInt(getCard(table.player4->hand, 1));
    int PLAYER1_BET = table.player1Bet;
    int PLAYER2_BET = table.player2Bet;
    int PLAYER3_BET = table.player3Bet;
    int PLAYER4_BET = table.player4Bet;
    int PLAYER1_BUCKS = getPoints(table.player1);
    int PLAYER2_BUCKS = getPoints(table.player2);
    int PLAYER3_BUCKS = getPoints(table.player3);
    int PLAYER4_BUCKS = getPoints(table.player4);
    int POT = table.potPoints;
    int PLAYER1_FOLDED = table.player1->fold;
    int PLAYER2_FOLDED = table.player2->fold;
    int PLAYER3_FOLDED = table.player3->fold;
    int PLAYER4_FOLDED = table.player4->fold;


    //printf("DEBUG ---- get game\n");
    //printf("CARDS %d %d %d %d %d %d %d %d\n", PLAYER1_CARD1, PLAYER1_CARD2, PLAYER2_CARD1, PLAYER2_CARD2, PLAYER3_CARD1, PLAYER3_CARD2, PLAYER4_CARD1, PLAYER4_CARD2);

    sprintf(from, "gs%d%c%c%c%c%c%d%c%c%c%c%c%c%c%c|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d%d%d%d", TURN, CARD1, CARD2, CARD3, CARD4, CARD5,
    SHOWN, PLAYER1_CARD1, PLAYER1_CARD2, PLAYER2_CARD1, PLAYER2_CARD2, PLAYER3_CARD1, PLAYER3_CARD2,
    PLAYER4_CARD1, PLAYER4_CARD2, PLAYER1_BET, PLAYER2_BET, PLAYER3_BET, PLAYER4_BET, PLAYER1_BUCKS,
    PLAYER2_BUCKS, PLAYER3_BUCKS, PLAYER4_BUCKS, POT, PLAYER1_FOLDED, PLAYER2_FOLDED, PLAYER3_FOLDED, PLAYER4_FOLDED);
    strncpy(SendBuf, from, 255);
    }
  }
  if (RecvBuf[0] == 'x'){
    //resets the table and starts a new game.
    int player1Bucks = table.player1->points;
    int player2Bucks = table.player2->points;
    int player3Bucks = table.player3->points;
    int player4Bucks = table.player4->points;
    int dealer = table.dealer;
    deleteTable(table);
    table = getNewTable(getNewPlayer(player1Bucks), getNewPlayer(player2Bucks), getNewPlayer(player3Bucks), getNewPlayer(player4Bucks), getNextPlayer(dealer));
    startGame();
    sprintf(from, "ox");
    strncpy(SendBuf, from, 255);
  }
  //GET FREE NUMBERS
  if (RecvBuf[0] == 'n'){
    sprintf(from, "f%d%d%d%d", player1Joined, player2Joined, player3Joined, player4Joined);
    strncpy(SendBuf, from, 255);
  }
  //JOIN
  if (RecvBuf[0] == 'j')
  {
    if (join(RecvBuf[1] - '0')){
      sprintf(from, "oj%c", RecvBuf[1]);
      strncpy(SendBuf, from, 255);
      if (player1Joined && player2Joined && player3Joined && player4Joined){
        startGame();
      }
    }
    else {
      sprintf(from, "nj");
      strncpy(SendBuf, from, 255);
    }
  }
  //SHUTDOWN
  if (RecvBuf[0] == 's'){
    sprintf(from, "os");
    sdown();
    strncpy(SendBuf, from, 255);
    SendBuf[255] = 0;
    return 1;
  }

  SendBuf[255] = 0;

  //printf("DEBUG: SEND BUF = %s\nSendBuf Chars = ", SendBuf);
  i = 0;
  while (SendBuf[i] != 0){
    //printf("%d ", SendBuf[i]);
    i++;
  }
  //printf("\n");
  for (i = 0; i < 256; i++){
    RecvBuf[i] = 0;
  }
  return 0;

}

int join(int playerNumber){
  if (playerNumber == 1){
    if (!player1Joined){
      printf("join as %d\n", playerNumber);
      player1Joined = 1;
      return 1;
    }
  }
  if (playerNumber == 2){
    if (!player2Joined){
      printf("join as %d\n", playerNumber);
      player2Joined = 1;
      return 1;
    }
  }
  if (playerNumber == 3){
    if (!player3Joined){
      printf("join as %d\n", playerNumber);
      player3Joined = 1;
      return 1;
    }
  }
  if (playerNumber == 4){
    if (!player4Joined){
      printf("join as %d\n", playerNumber);
      player4Joined = 1;
      return 1;
    }
  }
  return 0;
}

int startGame(){
  tableUp = 1;
  table.playerTurn = getSmallBlind(table);
  table = bet(table, getSmallBlind(table), CALL_RAISE, SMALL_BLIND);
  table = bet(table, getBigBlind(table), CALL_RAISE, BIG_BLIND);
  //Draw 2 cards to each player
  drawCardToPlayer(table, 1);
  drawCardToPlayer(table, 1);
  drawCardToPlayer(table, 2);
  drawCardToPlayer(table, 2);
  drawCardToPlayer(table, 3);
  drawCardToPlayer(table, 3);
  drawCardToPlayer(table, 4);
  drawCardToPlayer(table, 4);
  table.turn = 1;
}

int move(int playerNumber, int betAmt){
  if (playerNumber != table.playerTurn){
    return 0;
  }
  if (betAmt == -1){
    table = bet(table, playerNumber, FOLD, -1);
  }
  table = bet(table, playerNumber, CALL_RAISE, betAmt);
  //return bet(table, playerNumber, CALL_RAISE, betAmt);
  return 1;
}

int sdown(){
  deleteTable(table);
  printf("shutdown command\n");
  return 1;
}

int getIntFromRecvBuf(char RecvBuf[256], int i, char endChar){
  int number = 0;
  while (RecvBuf[i] != endChar){
    number *= 10;
    number += RecvBuf[i] - '0';
    i++;
  }
  //printf("DEBUG: getInt NUMBER = %d\n", number);
  return number;
}
