# files in the program
FILES = Deck.cpp Account.cpp BlackJack.cpp main.cpp

# compiler
CC = g++

# name of the executable
OUT = blackjack

# nothing else needed

all : $(FILES)
	$(CC) $(FILES) -o $(OUT)
