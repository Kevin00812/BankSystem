#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <vector>
#include <string>
#include <Windows.h>

class MainMenu;
using std::string;
using std::vector;

class Account {
private: 

	void createAccountMenu();

	void openAccount(string accountName);

	void createAccount(std::string accountType, std::string accountName, std::string accountPassword);

	std::string assignID(std::string accountType, std::string accountName);

	std::string findAccountByNumber(LPCWSTR directoryPath = L"C:\\Users\\kevin\\Desktop\\CPP Projects\\Banking system\\BankSystem\\Main\\Accounts", int accountNumber = 0);


	void openDeposit(string account);

	void openTransfer(string account);

	void openWithdraw(string account);

	void openChangePassword(string account);

	void adjustAccountBalance(string filePath, int amount, bool isDeposit);

	//void checkingsMenu(string accountName);

	//void savingsMenu(string accountName);

	std::vector<std::pair<int, std::string>> getAccountDetailsSortedByNumber(LPCWSTR directoryPath = L"C:\\Users\\kevin\\Desktop\\CPP Projects\\Banking system\\BankSystem\\Main\\Accounts");

	int findIndexByName(const std::vector<std::pair<int, std::string>>& accountDetails, const std::string& accountName);

	void changePassword(string filePath, string newPassword);

public:
	LPCWSTR defaultDirPath = L"C:\\Users\\kevin\\Desktop\\CPP Projects\\Banking system\\BankSystem\\Main\\Accounts";

	std::string getAccountDetail(string filePath, string detail);

	int getAccountAmount(LPCWSTR directoryPath =  L"C:\\Users\\kevin\\Desktop\\CPP Projects\\Banking system\\BankSystem\\Main\\Accounts");
	
	std::vector<string> getAccountNames(LPCWSTR directoryPath = L"C:\\Users\\kevin\\Desktop\\CPP Projects\\Banking system\\BankSystem\\Main\\Accounts");
	
	std::vector<int> accountIDs;

	void startAccount();
};

#endif