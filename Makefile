##################################################
#
# Makefile for the simple GTK example
# author: Weiwei Chen (updates by Rainer Doemer)
# initial version: 01/22/13 EECS22L Winter 2013
# patched version: 01/14/15 EECS22L Winter 2015
#
##################################################

GTKINC	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --cflags gtk+-2.0`
GTKLIBS	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --libs gtk+-2.0`

INC	= $(GTKINC)
LIBS	= $(GTKLIBS)

SERVDIR = ServerCode

OPTS	= -g

PROJECT	= test-gui $(SERVDIR)/PokerServer

server: $(SERVDIR)/PokerServer

ollie: $(SERVDIR)/GameTest

clean3:
	rm $(SERVDIR)/GameTest.o
clean2:
	rm $(SERVDIR)/Card.o
	rm $(SERVDIR)/Hand.o
	rm $(SERVDIR)/Player.o
	rm $(SERVDIR)/Table.o
	rm $(SERVDIR)/Evaluation.o

all: $(PROJECT)

GUI.o: GUI.c
	gcc -c GUI.c $(INC) $(OPTS) -o GUI.o

test-gui: GUI.o
	gcc GUI.o $(LIBS) $(OPTS) -o test-gui

$(SERVDIR)/Card.o: $(SERVDIR)/Card.c
	gcc $(SERVDIR)/Card.c -c $(LIBS) $(OPTS) -o $(SERVDIR)/Card.o

$(SERVDIR)/Hand.o: $(SERVDIR)/Card.c $(SERVDIR)/Hand.c
	gcc $(SERVDIR)/Hand.c -c $(LIBS) $(OPTS) -o $(SERVDIR)/Hand.o

$(SERVDIR)/Table.o: $(SERVDIR)/Card.c $(SERVDIR)/Hand.c $(SERVDIR)/Player.c $(SERVDIR)/Table.c
	gcc $(SERVDIR)/Table.c -c $(LIBS) $(OPTS) -o $(SERVDIR)/Table.o

$(SERVDIR)/Player.o: $(SERVDIR)/Card.c $(SERVDIR)/Hand.c $(SERVDIR)/Player.c
	gcc $(SERVDIR)/Player.c -c $(LIBS) $(OPTS) -o $(SERVDIR)/Player.o

$(SERVDIR)/Evaluation.o:
	gcc $(SERVDIR)/Evaluation.c -c $(LIBS) $(OPTS) -o $(SERVDIR)/Evaluation.o

$(SERVDIR)/PokerServer.o: $(SERVDIR)/PokerServer.c $(SERVDIR)/Card.c
	gcc $(SERVDIR)/PokerServer.c -c $(LIBS) $(OPTS) -o $(SERVDIR)/PokerServer.o

$(SERVDIR)/PokerServer: $(SERVDIR)/PokerServer.o $(SERVDIR)/Card.o $(SERVDIR)/Hand.o $(SERVDIR)/Table.o $(SERVDIR)/Player.o $(SERVDIR)/Evaluation.o
	gcc $(SERVDIR)/PokerServer.o $(SERVDIR)/Card.o $(SERVDIR)/Player.o $(SERVDIR)/Hand.o $(SERVDIR)/Table.o $(SERVDIR)/Evaluation.o $(LIBS) $(OPTS) -o $(SERVDIR)/PokerServer

$(SERVDIR)/GameTest.o: $(SERVDIR)/GameTest.c
	gcc $(SERVDIR)/GameTest.c -c $(LIBS) $(OPTS) -o $(SERVDIR)/GameTest.o

$(SERVDIR)/GameTest: $(SERVDIR)/GameTest.o $(SERVDIR)/Card.o $(SERVDIR)/Hand.o $(SERVDIR)/Player.o $(SERVDIR)/Table.o
	gcc $(SERVDIR)/GameTest.o $(SERVDIR)/Card.o $(SERVDIR)/Hand.o $(SERVDIR)/Player.o $(SERVDIR)/Table.o $(LIBS) $(OPTS) -o GameTest

clean:
	rm -rf *.o $(PROJECT)
