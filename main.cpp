#include <cctype>
#include <climits>
#include <sstream>
#include "Deck.h"
#include "Account.h"
#include "BlackJack.h"

bool CheckNum(std::string str);
int Betting(int bet);

int main()
{
	std::cout << "\nWELCOME TO TERMINAL BLACKJACK!\n\n";
	bool gameOn = true;
	
	// creating account
	Account account;

	// instance for BlackJack
	BlackJack bj;

	int bet = 0;
	
	// loop for hand played
	while(gameOn)
	{
		account.Read();
		// last round's bet is saved
		if(bet != 0)
		{
			std::cout << "CURRENT BET: " << bet << "\n";
			std::cout << "PRESS ENTER TO PLAY WITH SAME BET OR TYPE DIFFERENT BET\n";
			
			// setting the bet, returns 0 if bet is != integer, pressing enter for same bet
			bet = Betting(bet);
			if(bet == 0)
			{
				std::cout << "\nPLEASE ENTER THE BET AS AN INTEGER!\n";
				continue;
			}
		}

		else
		{
			// this is the first round played, so bet must be chosen
			std::cout << "ENTER YOUR BET AND PRESS ENTER!\n";
			std::string str;
			std::stringstream ss;
			std::cin >> str;
			// checking that bet is correct
			if(CheckNum(str))
			{
				// setting the bet
				ss << str;
				ss >> bet;
			}

			else
			{
				std::cout << "\nPLEASE ENTER THE BEST AS AN INTEGER!\n";
				continue;
			}
		}
		
		if(account.Balance() == 0)
		{
			std::cout << "OUT OF BALANCE!\npsst \"rm .account.txt\" to get more balance\n";
			break;
		}
		
		else if(bet > account.Balance())
		{
			std::cout << "YOU CANNOT BET MORE THAN YOUR BALANCE!\n\n";
			continue;
		}

		// setting the bet to BlackJack class
		bj.DefineBet(bet);

		Deck deck;
		// main player hand, or last hand with split
		int playerHand = 0;
		int dealerHand = 0;

		bool gettingCard = true;
		bool firstround = true;
		bool splitpossible = false;
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
		bj.PrintCard(DC1);
		dealerHand += DC1.value;
	
		std::cout << "\nYOU:\n";
		// player card 1
		usleep(waitTime);
		Card PC1 = deck.getCard();
		bj.PrintCard(PC1);

		// player card 2
		usleep(waitTime);
		Card PC2 = deck.getCard();
		bj.PrintCard(PC2);

		// calculate aces in hand
		if((PC1.value == 1) ^ (PC2.value == 1))
			acesInGame += 1;
		
		if(PC1.value == 1 && PC2.value == 1)
			acesInGame += 2;

		// counting first two cards
		playerHand = bj.CountScore(playerHand, (PC1.value + PC2.value), acesInGame);
		
		if(playerHand == 21)
		{
			std::cout << "BLACKJACK!\nYOU WIN!\n\n";
			account.Write(account.Balance() + bet * 1.5);
			continue;
		}
		
		std::cout << "YOU HAVE " << playerHand << "\n";

		// macro from Blackjack.h
		playstring
		
		// adding split option if two first cards have the same value
		if(PC1.value == PC2.value)
		{
			std::cout << ", (j) FOR SPLIT";
			splitpossible = true;
		}

		std::cout << "\n";

		// while getting more cards
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
				bj.PrintCard(PC3);
				
				// double down and go to next splitcard
				if(splitCardCounter == 1 && splitmax == 2)
				{
					split1 = bj.Doubling(PC3, acesInGame, split1, splitCardCounter, splitmax);
					choice = "j";
				}

				else if(splitCardCounter == 2 && splitmax != 2)
				{
					split2 = bj.Doubling(PC3, acesInGame, split2, splitCardCounter, splitmax);
					choice = "j";
				}
				else if(splitCardCounter == 3 && splitmax != 3)
				{
					split3 = bj.Doubling(PC3, acesInGame, split3, splitCardCounter, splitmax);
					choice = "j";
				}
				else
				{
					playerHand = bj.Doubling(PC3, acesInGame, playerHand, splitCardCounter, splitmax);
					// see the winner only if card didn't bust
					if(playerHand != -1)
						bj.WinHandle(playerHand, dealerHand, split1, split2, split3);
				
					// if lost remove balance
					else
						account.Write(account.Balance() - (bet * 2));

					// break the loop
					gettingCard = false;
				}
			}
			
			if(choice == "s")
			{
				if(bj.isSplitted())
					splitCardCounter += 1;

				else
					// normal gameplay i.e. non-splitted
					bj.WinHandle(playerHand, dealerHand, split1, split2, split3);
				
				if(!bj.isSplitted())
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
					bj.PrintCard(SC1);
					if(!bj.Hit(split1, SC1.value, acesInGame, firstround, splitpossible))
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
				// same for all possible hands	
				else if(splitCardCounter == 2 && splitmax != 2)
				{
					SC2 = deck.getCard();
					bj.PrintCard(SC2);
					if(!bj.Hit(split2, SC2.value, acesInGame, firstround, splitpossible))
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
					bj.PrintCard(SC3);
					if(!bj.Hit(split3, SC3.value, acesInGame, firstround, splitpossible))
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
					bj.PrintCard(card);
					if(!bj.Hit(playerHand, card.value, acesInGame, firstround, splitpossible))
					{
						bj.WinHandle(playerHand, dealerHand, split1, split2, split3);
						gettingCard = false;
					}
					else
					{
						firstround = false;
						continue;
					}
				}
			}
			
			// choose to split
			if(choice == "j" && splitpossible == true)
			{
				bj.DefineSplit(true);

				// break the loop with max of 3 splits
				if(splitCardCounter > splitmax)
				{
					bj.WinHandle(playerHand, dealerHand, split1, split2, split3);
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
					bj.PrintCard(PC2);
					std::cout << "YOU HAVE " << playerHand << "\n";
					playstring
					std::cout <<"\n";
					continue;
				}

				// split1 gets the value of first card
				else if(splitCardCounter == 1)
				{
					split1 = bj.SplitHand(PC1, "ONE", firstround);
					continue;
				}
				
				// split after split
				else if(splitCardCounter == 2)
				{
					split2 = bj.SplitHand(SC1, "TWO", firstround);
					continue;
				}

				// split after split after split
				else if(splitCardCounter == 3)
				{
					split3 = bj.SplitHand(SC2, "THREE", firstround);
					continue;
				}
			}
		}
		
		// ignore until '\n' to prevent instant new game
		std::cin.ignore(INT_MAX, '\n');
	}
	
	return 0;
}

// check if the bet is correct
bool CheckNum(std::string str)
{
	for(int i = 0; i < int(str.size()); i++)
		if(std::isdigit(str[i]) == false)
			return false;
	return true;
}

int Betting(int bet)
{
	std::string str;
	std::stringstream ss;
	// reading input
	std::getline(std::cin, str);
	// check for correct bet
	if(CheckNum(str))
	{
		ss << str;
		ss >> bet;
		return bet;
	}
	else
	{
		bet = 0;
		return bet;
	}
}
