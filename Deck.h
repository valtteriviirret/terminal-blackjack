#ifndef DECK_H
#define DECK_H

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Card.h"

class Deck
{
	public:
		Deck();
		Card getCard();
	private:
		void MakeDeck();
		void BlackJackValues();
		bool checkOriginal(Card card);
		
		Card CardDeck[52];
		int playedCards[16]; // max amount of cards played in single hand
		int playedCardsCounter;
};

#endif
