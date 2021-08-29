#include "Account.h"
#include <fstream>

// calling private function from constructor
Account::Account()
{
	createAccount();
}

Account::Account(bool DontCreateNew)
{
	// must be true
	if(!DontCreateNew)
		createAccount();
}

// making the actual account
void Account::createAccount()
{
	if(fileExists())
		std::cout << "WELCOME BACK!\n";
	
	else
	{
		std::ofstream ofs;
		ofs.open(account);

		std::cout << "FIRST TIME PLAYER!\nYOU HAVE BEEN AWARDED 100 FROM THE HOUSE!\n";
		// 100 from the house
		ofs << 100;
		ofs.close();
	}
}

// function for writing into account
void Account::Write(int amount)
{
	std::ofstream ofs(account);
	ofs << amount;
	ofs.close();
}

// function for reading info from account
void Account::Read()
{
	std::cout << "ACCOUNT BALANCE:\n";

	std::ifstream ifs;
	ifs.open(account);
	
	char c = ifs.get();

	while(ifs.good())
	{
		std::cout << c;
		c = ifs.get();
	}
	std::cout << "\n";	
	ifs.close();
}

// getter for balance
double Account::Balance()
{
	double d;
	std::ifstream ifs;
	ifs.open(account);
	
	ifs >> d;
	ifs.close();
	return d;
}

bool Account::fileExists()
{
	std::ifstream file(account);
	return file.good();
}
