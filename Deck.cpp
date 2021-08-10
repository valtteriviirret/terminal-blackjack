#include "Deck.h"

Deck::Deck()
{
	MakeDeck();
	playedCardsCounter = 0;
	for(int i = 0; i < 16; i++)
		playedCards[i] = -1;
}

// creating deck of cards
void Deck::MakeDeck()
{
	for(int i = 0; i < 52; i++)
	{
		CardDeck[i].id = i;
	
		if(i < 13)
		{
			CardDeck[i].suite = "♥";
			CardDeck[i].value = i + 1;
		}
		else if(i < 26)
		{
			CardDeck[i].suite = "♠";
			CardDeck[i].value = i - 12;
		}
		else if(i < 39)
		{
			CardDeck[i].suite = "♦";
			CardDeck[i].value = i - 25;
		}
		else
		{
			CardDeck[i].suite = "♣";
			CardDeck[i].value = i - 38;
		}
		
		// selecting each card by its value and adding stringvalue
		switch(CardDeck[i].value)
		{
			case 1: CardDeck[i].stringvalue = "A"; break;
			case 2: CardDeck[i].stringvalue = "2"; break;
			case 3: CardDeck[i].stringvalue = "3"; break;
			case 4: CardDeck[i].stringvalue = "4"; break;
			case 5: CardDeck[i].stringvalue = "5"; break;
			case 6: CardDeck[i].stringvalue = "6"; break;
			case 7: CardDeck[i].stringvalue = "7"; break;
			case 8: CardDeck[i].stringvalue = "8"; break;
			case 9: CardDeck[i].stringvalue = "9"; break;
			case 10: CardDeck[i].stringvalue = "10"; break;
			case 11: CardDeck[i].stringvalue = "J"; break;
			case 12: CardDeck[i].stringvalue = "Q"; break;
			case 13: CardDeck[i].stringvalue = "K"; break;
			default: break;
		}	
	}
	
	BlackJackValues();
}

void Deck::BlackJackValues()
{
	for(int i = 0; i < 52; i++)
	{
		switch(CardDeck[i].value)
		{
			case 1: CardDeck[i].value = 1; break;
			case 11: CardDeck[i].value = 10; break;
			case 12: CardDeck[i].value = 10; break;
			case 13: CardDeck[i].value = 10; break;
			default: break;
		}
	}
}

// getting one random card from the deck
Card Deck::getCard()
{
	srand((unsigned int)time(NULL));
	Card randomCard;

	while(true)
	{
		randomCard = CardDeck[(rand() % 52)];
		if(checkOriginal(randomCard))
			break;
	}

	return randomCard;
}

// checking that cards in the same hand can never be the same
bool Deck::checkOriginal(Card card)
{
	for(int i = 0; i < 16; i++)
	{
		if(card.id == playedCards[i])
			return false;
	}
	playedCardsCounter += 1;
	playedCards[playedCardsCounter] = card.id;
	return true;
}


