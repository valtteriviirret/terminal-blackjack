#include <iostream>
#include <unistd.h>
#include <climits>
#include "Deck.h"

#define waitTime 100000

bool CheckBust(int &hand);
int CountScore(int &hand, int add, int &aces);
void PrintCard(Card card);
void DetermineWinner(int &player, int &dealer, Deck &d);
void WinHandle(int &player, int &dealer, Deck &d, bool isSplit, int &splitCounter);
int savedHand;

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
		// main player hand
		int playerHand = 0;
		int dealerHand = 0;

		// global for saving the first hand in split
		savedHand = 0;
		bool gettingCard = true;
		bool firstround = true;

		bool splitpossible = false;
		bool isSplitted = false;
		int splitCardCounter = 0;
		int acesInGame = 0;
		
		std::cout << "------------------------------\n"
			"--------  A NEW HAND  --------\n------------------------------\n";
		std::cout << "  DEALER:\n";
	
		// dealer card no1	
		usleep(waitTime);
		Card DC1 = deck.getCard();
		PrintCard(DC1);
		dealerHand += DC1.value;

	
		std::cout << "\n  YOU:\n";
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
		std::string playstring = "PRESS (h) FOR HIT, (s) FOR STAND, (d) FOR DOUBLE";

		// adding split option if two first cards have the same value
		if(PC1.value == PC2.value)
		{
			playstring += ", (j) FOR SPLIT";
			splitpossible = true;
		}

		playstring += "\n";
		std::cout << playstring;
		while(gettingCard)
		{
			std::string choice;
			std::cin >> choice;
			
			// draw one card and stand (only available in the first round)
			if(choice == "d" && firstround == true)
			{		
				usleep(waitTime);
				Card PC3 = deck.getCard();
				PrintCard(PC3);
				if(PC3.value == 1)
					acesInGame += 1;
				playerHand = CountScore(playerHand, PC3.value, acesInGame);
				if(CheckBust(playerHand))
				{
					std::cout << "YOU HAVE " << playerHand << "\n";
					WinHandle(playerHand, dealerHand, deck, isSplitted, splitCardCounter);
					gettingCard = false;
				}
				gettingCard = false;
			}
			// stand for first or second card in split
			if(choice == "s")
			{
				if(splitCardCounter == 1)
					splitCardCounter += 1;
				else if(splitCardCounter == 2)
				{
					WinHandle(playerHand, dealerHand, deck, isSplitted, splitCardCounter);
					splitCardCounter += 1;
				}
				else
					// normal gameplay i.e. non-splitted
					WinHandle(playerHand, dealerHand, deck, isSplitted, splitCardCounter);
				
				if(!isSplitted)
					gettingCard = false;
				else
					choice = "j";
			}

			// hit and once again check if not splitted
			if(choice == "h")
			{
				usleep(waitTime);
				Card card = deck.getCard();
				PrintCard(card);
				if(card.value == 1)
					acesInGame += 1;
				if(splitCardCounter == 1)
				{
					// store first split card into global
					savedHand = CountScore(savedHand, card.value, acesInGame);
					if(CheckBust(savedHand))
					{
						if(savedHand == 21)
						{
							// no simple solution to this rare game so this will do
							std::cout << "YOU HAVE " << savedHand << "\nPRESS J!";
							splitCardCounter += 1;
							continue;
						}
						std::cout << "YOU HAVE " << savedHand << "\nPRESS (h) FOR HIT OR (s) FOR STAND\n";
						continue;
					}
					// move to second card if busted
					savedHand = -1;
					splitCardCounter += 1;	
				}
				else
				{
					playerHand = CountScore(playerHand, card.value, acesInGame);
					// check for bust or Blackjack
					if(CheckBust(playerHand))
					{
						if(playerHand == 21)
						{
							std::cout << "YOU HAVE " << playerHand << "\n\n";
							WinHandle(playerHand, dealerHand, deck, isSplitted, splitCardCounter);
							break;
						}
						// continue normally
						std::cout << "YOU HAVE " << playerHand << "\nPRESS (h) FOR HIT OR (s) FOR STAND\n";
						firstround = false;
						continue;
					}

					// kill splitting after two cards
					if(splitCardCounter == 2)
					{
						playerHand = -1;
						splitCardCounter += 1;
						WinHandle(playerHand, dealerHand, deck, isSplitted, splitCardCounter);
					}
				}
				if(!isSplitted)		
					gettingCard = false;
				else
					choice = "j";
			}

			if(choice == "j" && splitpossible == true)
			{
				firstround = false;
				isSplitted = true;

				// break the loop with third cycle
				if(splitCardCounter == 3)
					gettingCard = false;

				if(splitCardCounter == 2)
				{
					// make hand the second card and start the loop
					if(playerHand == 1)
						playerHand += 10;
					playerHand = PC2.value;
					std::cout << "\n\nHAND TWO:\n";
					PrintCard(PC2);
					std::cout << "YOU HAVE " << playerHand << "\nPRESS (h) FOR HIT OR (s) FOR STAND\n";
					continue;
				}

				if(splitCardCounter == 0)
					splitCardCounter += 1;

				if(splitCardCounter == 1)
				{
					// make global the first card and start the loop
					if(savedHand == 1)
						savedHand += 10;
					savedHand = PC1.value;
					std::cout << "\n\nHAND ONE:\n";
					PrintCard(PC1);
					std::cout << "YOU HAVE " << savedHand << "\nPRESS (h) FOR HIT OR (s) FOR STAND\n";
					continue;
				}
			}
		}
		
		// ignore '\n' to prevent instant new game
		std::cin.ignore(INT_MAX, '\n');
	}
	
	return 0;
}

void WinHandle(int &player, int &dealer, Deck &d, bool isSplit, int &splitCounter)
{
	// regular gameplay
	if(!isSplit)
		DetermineWinner(player, dealer, d);

	// check winner after two rounds
	else if(splitCounter == 2)
	{
		if(savedHand == -1 && player == -1)
		{
			std::cout << "BOTH HANDS BUSTED\n\nYOU LOSE!\n";
			splitCounter += 1;
		}

		else if(savedHand == -1)
		{
			std::cout << "FIRST HAND BUSTED!\n\n";
			DetermineWinner(player, dealer, d);
		}
		else if(player == -1)
		{
			DetermineWinner(savedHand, dealer, d);
			std::cout << "SECOND HAND BUSTED!\n\n";
		}
		else
		{
			// checking two cards against one dealer
			while(dealer < 17)
			{
				usleep(waitTime);
				Card card = d.getCard();
				dealer += card.value;
				std::cout << "DEALER HAS: " << dealer << "\n\n";
			}
			if(dealer > 21)
				std::cout << "DEALER BUSTS!\n\nBOTH HANDS WIN!\n\n";
			else
			{
				std::cout << "HAND ONE:\n";
				if(savedHand > dealer)
					std::cout << "YOU WIN!\n\n";
				else if(savedHand == dealer)
					std::cout << "PUSH!\n\n";
				else
					std::cout << "YOU LOST!\n\n";
				
				std::cout << "HAND TWO:\n";
				if(player > dealer)
					std::cout << "YOU WIN!\n\n";
				else if(player == dealer)
					std::cout << "PUSH!\n\n";
				else
					std::cout << "YOU LOST!\n\n";
			}
		}
	}
}


void DetermineWinner(int &player, int &dealer, Deck &d)
{
	// dealer must hit until 17
	while(dealer < 17)
	{
		usleep(waitTime);
		Card card = d.getCard();
		dealer += card.value;
		std::cout << "DEALER HAS: " << dealer << "\n";
	}
	if(dealer > 21)
		std::cout << "DEALER BUSTS!\n\nYOU WIN!\n\n";
	else if(player > dealer)
		std::cout << "\nYOU WIN!\n\n";
	else if(player == dealer)
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


