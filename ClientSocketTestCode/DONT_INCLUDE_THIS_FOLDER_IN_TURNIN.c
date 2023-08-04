char getSuit(int cardNumber){
  int suitNum = (cardNumber-1)/13;
  if (cardNumber == 66) return 'N';
  switch (suitNum){
    case 0:
      return 'S';
    case 1:
      return 'C';
    case 2:
      return 'H';
    case 3:
      return 'D';
    default:
      return 'E';
  }
}

//0-9 --> 0-9 char
//10 --> x
//J, K, Q, K, A --> same
char getValue(int cardNumber){
  int valueNum = ((cardNumber-1)%13)+2;
  if (cardNumber == 66) return 'N';
  switch (valueNum){
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
      return '0' + valueNum;
    case 11:
      return 'J';
    case 12:
      return 'Q';
    case 13:
      return 'K';
    case 14:
      return 'A';
    default:
      return 'E';
  }
}
