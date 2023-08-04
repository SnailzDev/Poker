#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>


const int PORT_NUMBER = 10122;
const char SERVER[] = "laguna.eecs.uci.edu";

void FatalError(const char *ErrorMsg);
void queryServer(struct sockaddr_in ServerAddress, const char *message,
char RecvBuf[256]);



void FatalError(const char *ErrorMsg){
    perror(ErrorMsg);
    exit(20);
}


int main(int argc, char *argv[])
{
    int PortNo;		/* port number */
    struct sockaddr_in
	ServerAddress;	/* server address we connect with */
    struct hostent
	*Server;	/* server host */

    Server = gethostbyname(SERVER);

    PortNo = PORT_NUMBER;
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(PortNo);
    ServerAddress.sin_addr = *(struct in_addr*)Server->h_addr_list[0];

    char RecvBuf[256];	/* message buffer for receiving a response */
    char SendBuf[256];	/* message buffer for receiving a response */

    while (!(RecvBuf[0] == 'o' && RecvBuf[1] == 's')){
      printf("Send Message: ");
      scanf("%s", SendBuf);
      queryServer(ServerAddress, SendBuf, RecvBuf);
      printf("Received Message: [%s]\nReceived Chars: ", RecvBuf);
      int i = 0;
      while (RecvBuf[i] != 0){
        printf("%d ", RecvBuf[i]);
        i++;
      }
      printf("\n");
    }
}

void queryServer(struct sockaddr_in ServerAddress, const char *message,
char RecvBuf[256]){
      int SocketFD;	/* socket file descriptor */
      int l, n;
    char SendBuf[256];	/* message buffer for sending a message */
    int i;
    for (i = 0; i < 256; i++){
      RecvBuf[i] = 0;
    }
  strncpy(SendBuf, message, sizeof(SendBuf)-1);
  l = strlen(SendBuf);
  if (SendBuf[l-1] == '\n')
  {
    SendBuf[--l] = 0;
  }

  if (l)
	{   SocketFD = socket(AF_INET, SOCK_STREAM, 0);
	    if (SocketFD < 0)
	    {   FatalError("socket creation failed");
	    }
	    if (connect(SocketFD, (struct sockaddr*)&ServerAddress,
			sizeof(ServerAddress)) < 0)
	    {   FatalError("connecting to server failed");
	    }
	    n = write(SocketFD, SendBuf, l);
	    if (n < 0)
	    {   FatalError("writing to socket failed");
	    }
      //printf("Write clear\n");
	    n = read(SocketFD, RecvBuf, 255);
	    if (n < 0)
	    {   FatalError("reading from socket failed");
	    }
	    RecvBuf[n] = '\n';
      //printf("read clear\n");
	    close(SocketFD);
	}
  for (i = 0; i < 256; i++){
    SendBuf[i] = 0;
  }
}
