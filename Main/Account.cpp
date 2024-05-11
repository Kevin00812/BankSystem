#include "Account.h"
#include <iostream>
#include <string>
#include <vector>
#include "UtilityFunctions.h"
#include "MainMenu.h"
#include <fstream>
#include <Windows.h>
#include <filesystem>
#include <codecvt>
#include <locale>


#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

UtilityFunctions utilityFunction;

namespace fs = std::filesystem;

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ws;
using std::string;
MainMenu mainmenu;

void Account::createAccountMenu() {
	string type;
	string name;
	string password;
	string passwordRewrite;

	utilityFunction.setColor(3);
	cout << "Create new account" << endl;
	utilityFunction.setColor(7);
	
type:
	cout << "Select account type (1. Checkings 2. Savings): ";
	cin >> ws;
	std::getline(cin, type);
	string newType = utilityFunction.toLowerCase(type);

	cout << "Debug: [" << newType << "]" << endl;

	if (newType != "1" && newType != "2" && newType != "checkings" && newType != "savings") {
		utilityFunction.setColor(4);
		cout << "\"" << newType << "\"" << " is an invalid account type, please enter a valid type (Press Enter to continue)";
		utilityFunction.setColor(7);
		cin.get();
		goto type; // Temporary
	}

	cout << endl << "Enter account name: ";
	std::getline(cin, name);

password:
	cout << endl << "Create new password: ";
	std::getline(cin, password);

	cout << endl << "Rewrite password: ";
	std::getline(cin, passwordRewrite);

	if (password != passwordRewrite)
	{
		utilityFunction.setColor(4); // Red
		cout << "Passwords did not match, make sure to write excactly the same password! (Press Enter to continue)";
		utilityFunction.setColor(7); // Red
		cin.get();
		goto password; // Goto is temporary, make new system!
	}
	else {
		if (newType == "1" || newType == "checkings" || newType == "c")
		{
			newType = "checkings";
		}
		else if(newType == "2" || newType == "savings" || newType == "s") {
			newType = "savings";
		}

		createAccount(newType, name, password);
		system("cls");
		startAccount();
	}
}

void Account::createAccount(string type, string name, string password) {

	string accountID = assignID(type, name);

	int accountNum = getAccountAmount() + 1;

	double accountBalance = 0.00;

	LPCWSTR dirPath = L"C:\\Users\\kevin\\Desktop\\CPP Projects\\Banking system\\BankSystem\\Main\\Accounts";

	CreateDirectory(dirPath, NULL);

	string filePath = "C:\\Users\\kevin\\Desktop\\CPP Projects\\Banking system\\BankSystem\\Main\\Accounts\\" + name + ".txt";
	
	std::ofstream file(filePath);

	file << "Account Type: " << type << endl
		 << "Account Name: " << name << endl
		 << "Account Password: " << password << endl
		 << "Account ID: " << accountID << endl
		 << "Account Number: " << accountNum << endl
		 << "Account Balance: " << accountBalance << endl;
}

void Account::startAccount() {
	int accountAmount = getAccountAmount();
	std::vector<std::pair<int, std::string>> accountDetails;

	if (accountAmount != 0) {
		// Retrieve and sort account names based on their account numbers
		accountDetails = getAccountDetailsSortedByNumber(L"C:\\Users\\kevin\\Desktop\\CPP Projects\\Banking system\\BankSystem\\Main\\Accounts");
		std::sort(accountDetails.begin(), accountDetails.end());  // Ensure the vector is sorted by account numbers

		utilityFunction.setColor(3);
		cout << "Select an account to open (numbers only) - To create new account write \"new\": " << endl;
		utilityFunction.setColor(7);

		// Display sorted account names with their respective numbers
		for (size_t i = 0; i < accountDetails.size(); ++i) {
			cout << (i + 1) << ". " << accountDetails[i].second << endl;  // Increment display number starting from 1
		}

		cout << "Enter account to open: ";
		cin >> ws;
		string line;
		cin >> line;

		if (line == "new") {
			system("cls");
			createAccountMenu();
		}
		else {
			try {
				int numLine = std::stoi(line);
				if (numLine > 0 && numLine <= accountDetails.size()) {
					string accountFile = accountDetails[numLine - 1].second;  // Access the correct account by index
					system("cls");
					openAccount(accountFile);
				}
				else {
					cout << "Invalid account number selected. Please try again." << endl;
				}
			}
			catch (const std::exception& e) {
				cout << "Invalid input. Please enter a number. Error: " << e.what() << endl;
			}
		}
	}
	else {
		string line;
		utilityFunction.setColor(3);
		cout << "Looks like you don't already have an account, do you wish to create one?" << endl;
		utilityFunction.setColor(7);
		cout << "Select option (y/n): ";
		cin >> std::ws;
		std::getline(cin, line);
		string newLine = utilityFunction.toLowerCase(line);


		if (newLine == "y" || newLine == "yes")
		{
			system("cls");
			createAccountMenu();
		}
		else {
			mainmenu.openMainMenu(true);
		}
	}
}

string Account::assignID(string type, string name) {
	string accountID = "EMPTY";
	char shortType = type[0];

	int fileCount = getAccountAmount() + 1;

	accountID = name + shortType + std::to_string(fileCount);

	return accountID;
}

std::vector<string> Account::getAccountNames(LPCWSTR directoryPath) {
	string newDir = utilityFunction.LPCWSTRToString(directoryPath);
	std::vector<std::string> accountNames;

	// If Directory doesn't exist
	if (!fs::exists(newDir) || !fs::is_directory(newDir)) {
		throw std::runtime_error("Provided path does not exist or is not a directory.");
	}

	// Iterate through the directory
	for (const auto& entry : fs::directory_iterator(newDir)) {
		if (fs::is_regular_file(entry)) {  // Check if the entry is a regular file
			accountNames.push_back(entry.path().filename().string());  // Add the file name to the vector
		}
	}

	return accountNames;
}
 
int Account::getAccountAmount(LPCWSTR directoryPath) {
	string newDir = utilityFunction.LPCWSTRToString(directoryPath);
	int fileCount = 0;

	for (const auto& entry : fs::directory_iterator(newDir)) {
		if (fs::is_regular_file(entry.status())) {
			++fileCount;
		}
	}
	return fileCount;
}

string Account::findAccountByNumber(LPCWSTR directoryPath, int accountNumber) {
	string newDir = utilityFunction.LPCWSTRToString(directoryPath);

	for (const auto& entry : fs::directory_iterator(newDir)) {
		if (entry.is_regular_file()) {
			std::ifstream file(entry.path());
			std::string line;

			// Read the file line by line
			while (getline(file, line)) {
				// Check if the line contains "Account Number:"
				size_t pos = line.find("Account Number: ");
				if (pos != std::string::npos) {
					// Extract the number part from the line
					std::string numberStr = line.substr(pos + 16); // 16 is the length of "Account Number: "
					int foundNumber = std::stoi(numberStr);

					// Compare with the desired account number
					if (foundNumber == accountNumber) {
						file.close();
						return entry.path().filename().string(); // Return the filename of the account
					}
				}
			}
			file.close();
		}
	}

	return "Account not found";
}

string Account::getAccountDetail(string filePath, string detail) {
	// Detail can be equal to all of the variables inside the account information files. For example "Account Balance"

	std::ifstream accountFile(filePath);
	std::string line;
	std::string prefix = detail + ": "; // Construct the search prefix

	if (accountFile.is_open()) {
		while (getline(accountFile, line)) {
			// Find the line starting with the detail name
			if (line.find(prefix) == 0) {
				// Extract the value after the prefix
				return line.substr(prefix.size());
			}
		}
		accountFile.close();
	}
	else {
		return "Unable to open file";
	}

	return "Detail not found"; // Return an error message if the detail is not found
}

void Account::openAccount(string accountName) {

	utilityFunction.setColor(3);
	cout << "Account: " << accountName << endl;
	utilityFunction.setColor(7);

	std::ifstream f("C:\\Users\\kevin\\Desktop\\CPP Projects\\Banking system\\BankSystem\\Main\\Accounts\\" + accountName);
	string filePath = "C:\\Users\\kevin\\Desktop\\CPP Projects\\Banking system\\BankSystem\\Main\\Accounts\\" + accountName;
	
	string getBalance = getAccountDetail(filePath, "Account Balance");
	double balance = stod(getBalance);

	if (f.is_open()) {
		cout << "Balance: " ;

		if (balance < 0)
		{
			utilityFunction.setColor(4);
		}
		else {
			utilityFunction.setColor(2);
		}

		cout << "$" << balance << endl;
		utilityFunction.setColor(7);

		string line;
		cout << "1. Deposit " << endl
			<< "2. Transfer" << endl
			<< "3. Withdraw" << endl
			<< "4. Change Account Password" << endl
			<< "5. Coming soon.." << endl
			<< "6. Quit" << endl;

		cout << "Select an option: ";
		cin >> std::ws;
		getline(cin, line);
		line = utilityFunction.toLowerCase(line);

		cout << endl;

		if (line == "1" || line == "deposit")
		{
			openDeposit(accountName);
		}
		else if (line == "2" || line == "transfer") {
			openTransfer(accountName);
		} 
		else if (line == "3" || line == "withdraw") {
			openWithdraw(accountName);
		}
		else if (line == "4" || line == "change account password") {
			openChangePassword(accountName);
		}
		else if (line == "5" || line == "change account name") {
			//openChangeName(accountName);
		}
		else if (line == "6" || line == "quit") {
			mainmenu.openMainMenu(true);
		}
		else {
			cout << "Invalid input";
		}
	}
}

std::vector<std::pair<int, std::string>> Account::getAccountDetailsSortedByNumber(LPCWSTR directoryPath) {
	std::vector<std::pair<int, std::string>> accountDetails;
	std::string newDir = utilityFunction.LPCWSTRToString(directoryPath);

	for (const auto& entry : fs::directory_iterator(newDir)) {
		if (fs::is_regular_file(entry)) {
			std::ifstream file(entry.path());
			std::string line;
			int accountNumber = 0;
			std::string accountName = entry.path().filename().string();
			while (getline(file, line)) {
				size_t pos = line.find("Account Number: ");
				if (pos != std::string::npos) {
					accountNumber = std::stoi(line.substr(pos + 16));
					accountDetails.push_back({ accountNumber, accountName });
					break;
				}
			}
			file.close();
		}
	}

	// Sort the vector based on the account numbers
	std::sort(accountDetails.begin(), accountDetails.end(),
		[](const std::pair<int, std::string>& a, const std::pair<int, std::string>& b) {
			return a.first < b.first;
		});

	return accountDetails;
}




