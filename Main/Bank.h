#ifndef BANK_H 
#define BANK_H

#include <string>

using std::string;

class Bank {
public:

	void addTransaction(std::string accountName, std::string type, double amount, std::string destinationAccountName = "");

	void openBank();

	class Transactions {
	public:
		void saveTransactionToFile(string accountID, string type, string amount, string date);
		string getTransactionNumber();
	};
};

#endif
