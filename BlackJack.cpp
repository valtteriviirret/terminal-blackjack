#include "BlackJack.h" 

BlackJack::BlackJack()
{
	bet = 0;
	split1d = false;
	split2d = false;
	split3d = false;
	playerd = false;
	isSplit = false;
}

void BlackJack::DefineSplit(bool value){ isSplit = value; }

bool BlackJack::isSplitted(){ return isSplit; }

int BlackJack::CountScore(int &hand, int add, int &aces)
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

bool BlackJack::CheckBust(int &hand)
{
	if(hand > 21)	
	{
		std::cout << "YOU HAVE " << hand  << "\n" << "\nBUST!\n\n";
		return false;
	}
	else
		return true;
}

void BlackJack::PrintCard(Card card)
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

void BlackJack::WinHandle(int &player, int &dealer, int &split1, int &split2, int &split3)
{
	int count = 2;
	std::string handname = "SECOND ";
	std::string hn = "TWO";
	
	// regular gameplay, without hand number
	if(!isSplit)
		if(player == -1)
		{
			if(playerd)
				account.Write(account.Balance() - (bet * 2));
			else
				account.Write(account.Balance() - bet);
		}
		else if(playerd)
			DetermineWinner(player, dealer, "", true);
		else
			DetermineWinner(player, dealer, "", false);
	else
	{
		// if splitted more than once show score with count and give handname accordingly
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
		{
			std::cout << "FIRST HAND BUSTED!\n\n";
			if(split1d)
				account.Write(account.Balance() - (bet * 2));
			else
				account.Write(account.Balance() - bet);
		}
		else if(split1d)
			DetermineWinner(split1, dealer, "ONE", true);
		else
			DetermineWinner(split1, dealer, "ONE", false);

		// if splitted twice
		if(count > 2)
		{
			if(split2 == -1)
			{
				std::cout << "SECOND HAND BUSTED!\n\n";
				if(split2d)
					account.Write(account.Balance() - (bet * 2));
				else
					account.Write(account.Balance() - bet);
			}

			else if(split2d)
				DetermineWinner(split2, dealer, "TWO", true);
			else
				DetermineWinner(split2, dealer, "TWO", false);
		}

		// if splitted three times
		if(count > 3)
		{
			if(split3 == -1)
			{
				std::cout << "THIRD HAND BUSTED!\n\n";
				if(split3d)
					account.Write(account.Balance() - (bet * 2));
				else
					account.Write(account.Balance() - bet);
			}

			else if(split3d)
				DetermineWinner(split3, dealer, "THREE", true);
			else
				DetermineWinner(split3, dealer, "THREE", false);
		}
	
		// playerHand is always the last hand
		if(player == -1)
		{
			std::cout << handname << "HAND BUSTED!\n\n";
			if(playerd)
				account.Write(account.Balance() - (bet * 2));
			else
				account.Write(account.Balance() - bet);
		}

		else if(playerd)
			DetermineWinner(player, dealer, hn, true);
		else
			DetermineWinner(player, dealer, hn, false);
	}
}

void BlackJack::DetermineWinner(int &player, int &dealer, std::string handname, bool dodouble)
{
	// dealer must hit until 17
	while(dealer < 17)
	{
		usleep(waitTime);
		Card card = deck.getCard();
		// add card's value to dealer, ace is 11 as long as the total is less than 21
		if(card.value == 1 && (dealer + 11) <= 21)
			dealer += 10;
		dealer += card.value;
		std::cout << "DEALER HAS: " << dealer << "\n";
	}
	
	// first bool if split, second if double is played
	if(handname != "")
		if(dodouble)
			WinCheck(player, dealer, handname, true, true);
		else
			WinCheck(player, dealer, handname, true, false);
	else
		if(dodouble)
			WinCheck(player, dealer, "empty", false, true);
		else
			WinCheck(player, dealer, "empty", false, false);

}

// compare playerhand and dealerhand
void BlackJack::WinCheck(int &hand, int &dealer, std::string handname, bool isSplit, bool dodouble)
{
	if(isSplit)
		std::cout << "HAND " << handname << ":\n";
	
	if(dealer > 21)
	{
		std::cout << "\nDEALER BUSTS!\nYOU WIN!\n\n";
		// if player chose to double give double winnings
		if(dodouble)
			account.Write(account.Balance() + (bet * 2));
		else
			account.Write(account.Balance() + bet);
	}
	else if(hand > dealer)
	{
		std::cout << "\nYOU WIN!\n\n";
		if(dodouble)
			account.Write(account.Balance() + (bet * 2));
		else
			account.Write(account.Balance() + bet);
	}
	else if(hand == dealer)
		std::cout << "\nPUSH!\n\n";
	else
	{
		std::cout << "\nYOU LOST!\n\n";
		if(dodouble)
			account.Write(account.Balance() - (bet * 2));
		else
			account.Write(account.Balance() - bet);
	}

}

// return false if 21 or over, return true if less than 21
bool BlackJack::Hit(int &split, int hit, int &aces, bool firstround, bool &splitpossible)
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

// new splitcard, return its beginning value
int BlackJack::SplitHand(Card card, std::string handnum, bool &firstround)
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

// double down, check if new card busts
int BlackJack::Doubling(Card card, int &aces, int split, int &counter, int &splitmax)
{
	if(card.value == 1)
		aces += 1;
	split = CountScore(split, card.value, aces);
	if(CheckBust(split))
	{
		// with these booleans we mark which hands are doubled
		if(counter == 1 && splitmax == 2)
			split1d = true;
		else if(counter == 2 && splitmax != 2)
			split2d = true;
		else if(counter == 3 && splitmax != 3)
			split3d = true;
		else
			playerd = true;

		counter += 1;
		std::cout << "YOU HAVE " << split << "\n";
		return split;
	}
	counter += 1;
	split = -1;
	return split;
}

void BlackJack::DefineBet(int Bet){ bet = Bet; }


