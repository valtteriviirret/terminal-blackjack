// this class contains functions and macros needed for Blackjack game
#ifndef BLACKJACK_H
#define BLACKJACK_H

#include "Card.h"
#include "Deck.h"
#include "Account.h"
#include <unistd.h>
#include <iostream>

#define playstring std::cout << "PRESS (h) FOR HIT, (s) FOR STAND, (d) FOR DOUBLE";
#define waitTime 100000

class BlackJack
{
	public:
		BlackJack();
		int CountScore(int &hand, int add, int &aces);
		void PrintCard(Card card);
		void WinHandle(int &player, int &dealer, int &split1, int &split2, int &split3);
		bool Hit(int &split, int hit, int &aces, bool firstround, bool &splitpossible);
		int SplitHand(Card card, std::string handnum, bool &firstround);
		int Doubling(Card card, int &aces, int split, int &counter, int &splitmax);
		void DefineBet(int bet);
		void DefineSplit(bool value);
		bool isSplitted();
	private:
		bool CheckBust(int &hand);
		void WinCheck(int &hand, int &dealer, std::string handname, bool isSplit, bool dodouble);
		void DetermineWinner(int &player, int &dealer, std::string handname, bool dodouble);		
		int bet;
		Account account = Account(true);
		Deck deck;
		bool isSplit;
		bool split1d, split2d, split3d, playerd;
};

#endif
