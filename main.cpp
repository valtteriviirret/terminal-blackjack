#include <iostream>
#include <unistd.h>
#include <climits>
#include "Deck.h"

#define waitTime 100000
#define playstring std::cout << "PRESS (h) FOR HIT, (s) FOR STAND, (d) FOR DOUBLE";

bool CheckBust(int &hand);
int CountScore(int &hand, int add, int &aces);
void PrintCard(Card card);
void DetermineWinner(int &player, int &dealer, Deck &d, std::string handname);
void WinHandle(int &player, int &dealer, Deck &d, bool isSplit, int &split1, int &split2, int &split3);
bool Hit(int &split, int hit, int &aces, bool firstround, bool &splitpossible);
void WinCheck(int &hand, int &dealer, std::string handname, bool isSplit);
int SplitHand(Card card, std::string handnum, bool &firstround);
int Doubling(Card card, int &aces, int split, int &counter);

int main()
{
	std::cout << "\nWELCOME TO TERMINAL BLACKJACK!\n\n";
	bool gameOn = true;
	
	// loop for hand played
	while(gameOn)
	{
		std::cout << "PRESS (ENTER) TO PLAY!\n";
		std::cin.get();
		
		Deck deck;
		// main player hand, or last hand with split
		int playerHand = 0;
		int dealerHand = 0;

		bool gettingCard = true;
		bool firstround = true;
		bool splitpossible = false;
		bool isSplitted = false;
		int acesInGame = 0;

		// max amount of splits
		int splitmax = 1;

		// counter for current splitcard
		int splitCardCounter = 1;

		// splitcards and values
		Card SC1, SC2, SC3;
		int split1 = 0, split2 = 0, split3 = 0;
	
		std::cout << "------------------------------\n"
			"--------  A NEW HAND  --------\n------------------------------\n";
		std::cout << "DEALER:\n";
	
		// dealer card no1	
		usleep(waitTime);
		Card DC1 = deck.getCard();
		PrintCard(DC1);
		dealerHand += DC1.value;

	
		std::cout << "\nYOU:\n";
		// player card 1
		usleep(waitTime);
		Card PC1 = deck.getCard();
		PrintCard(PC1);

		// player card 2
		usleep(waitTime);
		Card PC2 = deck.getCard();
		PrintCard(PC2);

		// calculate aces in hand
		if((PC1.value == 1) ^ (PC2.value == 1))
			acesInGame += 1;
		
		if(PC1.value == 1 && PC2.value == 1)
			acesInGame += 2;

		// counting first two cards
		playerHand = CountScore(playerHand, (PC1.value + PC2.value), acesInGame);
		
		if(playerHand == 21)
		{
			std::cout << "BLACKJACK!\nYOU WIN!\n\n";
			continue;
		}
		
		std::cout << "YOU HAVE " << playerHand << "\n";

		playstring
		
		// adding split option if two first cards have the same value
		if(PC1.value == PC2.value)
		{
			std::cout << ", (j) FOR SPLIT";
			splitpossible = true;
		}

		std::cout << "\n";

		while(gettingCard)
		{
			std::string choice;
			std::cin >> choice;

			if(choice == "j")
				splitmax += 1;
			
			// draw one card and stand (only available in the first round)
			if(choice == "d" && firstround == true)
			{
				usleep(waitTime);
				// generate and print random card
				Card PC3 = deck.getCard();
				PrintCard(PC3);
				
				// double down and go to next splitcard
				if(splitCardCounter == 1 && splitmax == 2)
				{
					split1 = Doubling(PC3, acesInGame, split1, splitCardCounter);
					choice = "j";
				}

				else if(splitCardCounter == 2 && splitmax != 2)
				{
					split2 = Doubling(PC3, acesInGame, split2, splitCardCounter);
					choice = "j";
				}
				else if(splitCardCounter == 3 && splitmax != 3)
				{
					split3 = Doubling(PC3, acesInGame, split3, splitCardCounter);
					choice = "j";
				}
				else
				{
					playerHand = Doubling(PC3, acesInGame, playerHand, splitCardCounter);
					// see the winner only if card didn't bust
					if(playerHand != -1)
						WinHandle(playerHand, dealerHand, deck, isSplitted, split1, split2, split3);
					// break the loop
					gettingCard = false;
				}
				
			}
			
			if(choice == "s")
			{
				if(isSplitted)
					splitCardCounter += 1;

				else
					// normal gameplay i.e. non-splitted
					WinHandle(playerHand, dealerHand, deck, isSplitted, split1, split2, split3);
				
				if(!isSplitted)
					gettingCard = false;
				else
					choice = "j";
			}

			if(choice == "h")
			{
				usleep(waitTime);
				if(splitCardCounter == 1 && splitmax == 2)
				{
					// get new card
					SC1 = deck.getCard();
					PrintCard(SC1);
					if(!Hit(split1, SC1.value, acesInGame, firstround, splitpossible))
					{
						// if 21 or bust move on to next card
						splitCardCounter += 1;
						choice = "j";
					}
					// else get new card
					else
					{
						firstround = false;
						continue;
					}
				}
				
				else if(splitCardCounter == 2 && splitmax != 2)
				{
					SC2 = deck.getCard();
					PrintCard(SC2);
					if(!Hit(split2, SC2.value, acesInGame, firstround, splitpossible))
					{
						splitCardCounter += 1;
						choice = "j";
					}
					else
					{
						firstround = false;
						continue;
					}
				}
				
				else if(splitCardCounter == 3 && splitmax != 3)
				{
					SC3 = deck.getCard();
					PrintCard(SC3);
					if(!Hit(split3, SC3.value, acesInGame, firstround, splitpossible))
					{
						splitCardCounter += 1;
						choice = "j";
					}
					else
					{
						firstround = false;
						continue;
					}
				}

				// gameplay for default non splitted hand or last card of splitted hand
				else
				{
					Card card = deck.getCard();
					PrintCard(card);
					if(!Hit(playerHand, card.value, acesInGame, firstround, splitpossible))
					{
						WinHandle(playerHand, dealerHand, deck, isSplitted, split1, split2, split3);
						gettingCard = false;
					}
					else
					{
						firstround = false;
						continue;
					}
				}
			}

			if(choice == "j" && splitpossible == true)
			{
				isSplitted = true;

				// break the loop with max of 3 splits
				if(splitCardCounter > splitmax)
				{
					WinHandle(playerHand, dealerHand, deck, isSplitted, split1, split2, split3);
					gettingCard = false;
				}

				// last card in split
				else if(splitCardCounter == splitmax)
				{
					firstround = true;
					// value of second card in the beginning
					playerHand = PC2.value;
					if(PC2.value == 1)
						playerHand += 10;
					if(splitmax == 2)
						std::cout << "\n\nHAND TWO:\n";
					else if(splitmax == 3)
						std::cout << "\n\nHAND THREE:\n";
					else
						std::cout << "\n\nHAND FOUR:\n";
					PrintCard(PC2);
					std::cout << "YOU HAVE " << playerHand << "\n";
					playstring
					std::cout <<"\n";
					continue;
				}

				// split1 gets the value of first card
				else if(splitCardCounter == 1)
				{
					split1 = SplitHand(PC1, "ONE", firstround);
					continue;
				}
				
				// split after split
				else if(splitCardCounter == 2)
				{
					split2 = SplitHand(SC1, "TWO", firstround);
					continue;
				}

				// split after split after split
				else if(splitCardCounter == 3)
				{
					split3 = SplitHand(SC2, "THREE", firstround);
					continue;
				}
			}
		}
		
		// ignore until '\n' to prevent instant new game
		std::cin.ignore(INT_MAX, '\n');
	}
	
	return 0;
}

// double down, check if new card busts
int Doubling(Card card, int &aces, int split, int &counter)
{
	counter += 1;
	if(card.value == 1)
		aces += 1;
	split = CountScore(split, card.value, aces);
	if(CheckBust(split))
	{
		std::cout << "YOU HAVE " << split << "\n";
		return split;
	}
	split = -1;
	return split;
}

// new splitcard, return its beginning value
int SplitHand(Card card, std::string handnum, bool &firstround)
{
	int split;
	firstround = true;
	split = card.value;
	if(card.value == 1)
		split += 10;
	std::cout << "\n\nHAND " << handnum << ":\n";
	PrintCard(card);
	std::cout << "YOU HAVE " << split << "\n";
	playstring
	std::cout << "\n";
	return split;
}

// return false if 21 or over, return true if less than 21
bool Hit(int &split, int hit, int &aces, bool firstround, bool &splitpossible) 
{
	std::string splitstring = "\nPRESS (h) FOR HIT OR (s) FOR STAND";
	// possibility for new split
	if(split == hit && firstround == true)
	{
		splitstring += " OR (j) FOR SPLIT";
		splitpossible = true;
	}
	if(hit == 1)
		aces += 1;
	split = CountScore(split, hit, aces);
	if(CheckBust(split))
	{
		if(split == 21)
		{
			std::cout << "YOU HAVE " << split << "\n";
			return false;
		}
		splitstring += "\n";
		std::cout << "YOU HAVE " << split;
		std::cout << splitstring;
		return true;	
	}
	split = -1;
	return false;
}

void WinHandle(int &player, int &dealer, Deck &d, bool isSplit, int &split1, int &split2, int &split3)
{
	int count = 2;
	std::string handname = "SECOND ";
	std::string hn = "TWO";
	
	// regular gameplay, without hand number
	if(!isSplit)
		DetermineWinner(player, dealer, d, "");
	else
	{
		// if splitted more than once show score with count and give string values to "player"
		if(split2 != 0)
		{
			count = 3;
			handname = "THIRD ";
			hn = "THREE";
		}

		if(split3 != 0)
		{
			count = 4;
			handname = "FOURTH ";
			hn = "FOUR";
		}

		if(split1 == -1)
			std::cout << "FIRST HAND BUSTED!\n\n";
		else
			DetermineWinner(split1, dealer, d, "ONE");

		// if splitted twice
		if(count > 2)
		{
			if(split2 == -1)
				std::cout << "SECOND HAND BUSTED!\n\n";
			else
				DetermineWinner(split2, dealer, d, "TWO");
		}

		// if splitted three times
		if(count > 3)
		{
			if(split3 == -1)
				std::cout << "THIRD HAND BUSTED!\n\n";
			else
				DetermineWinner(split3, dealer, d, "THREE");
		}
	
		// playerHand is always the last hand. This is the reason this function is messy
		if(player == -1)
			std::cout << handname << "HAND BUSTED!\n\n";
		else
			DetermineWinner(player, dealer, d, hn);
	}
}


void DetermineWinner(int &player, int &dealer, Deck &d, std::string handname)
{
	// dealer must hit until 17
	while(dealer < 17)
	{
		usleep(waitTime);
		Card card = d.getCard();
		dealer += card.value;
		std::cout << "DEALER HAS: " << dealer << "\n";
	}
	
	if(handname != "")
		WinCheck(player, dealer, handname, true);
	else
		WinCheck(player, dealer, "empty", false);
}

// 
void WinCheck(int &hand, int &dealer, std::string handname, bool isSplit)
{
	if(isSplit)
		std::cout << "HAND " << handname << ":\n";
	
	if(dealer > 21)
		std::cout << "\nDEALER BUSTS!\nYOU WIN!\n\n";
	else if(hand > dealer)
		std::cout << "\nYOU WIN!\n\n";
	else if(hand == dealer)
		std::cout << "\nPUSH!\n\n";
	else
		std::cout << "\nYOU LOST!\n\n";
}

int CountScore(int &hand, int add, int &aces)
{
	int score;
	if(aces == 0)
		score = hand + add;
	// ace acts as eleven if score is less than 21
	else if(aces == 1)
	{
		aces -= 1;
		score = (hand + add + 10);
		if(score > 21)
			score -= 10;
	}
	// this means 2 aces in the beginning hand
	else
	{
		score = 12;
		aces -= 2;
	}
	return score;
}

bool CheckBust(int &hand)
{
	if(hand > 21)	
	{
		std::cout << "YOU HAVE " << hand  << "\n" << "\nBUST!\n\n";
		return false;
	}
	else
		return true;
}

void PrintCard(Card card)
{
	std::string empty = " ";

	// bigger card for number 10
	if(card.stringvalue == "10")
	{
		std::string bigmodel = "  -----\n  |";
		std::string bigothermodel = "|\n  |    |\n  -----\n";
		bigmodel += card.suite;
		bigmodel += empty;
		bigmodel += card.stringvalue;
		bigmodel += bigothermodel;
		std::cout << bigmodel;
	}

	else
	{
		std::string cardmodel = "  ----\n  |";
		std::string othermodel = "|\n  |   |\n  ----\n";
		cardmodel += card.suite;
		cardmodel += empty;
		cardmodel += card.stringvalue;
		cardmodel += othermodel;
		std::cout << cardmodel;	
	}
}


