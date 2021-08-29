// this class handles "account"
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include<iostream>
#include<fstream>
#include<string>

class Account
{
	public:
		Account();
		Account(bool DontCreateNew);
		void Write(int amount);
		void Read();
		double Balance();

	private:
		void createAccount();
		bool fileExists();
		std::string account = ".account.txt";
};

#endif
