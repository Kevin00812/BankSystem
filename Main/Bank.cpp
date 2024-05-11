#include "Bank.h"
#include "MainMenu.h"
#include "UtilityFunctions.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>

namespace fs = std::filesystem;

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ws;
using std::string;
MainMenu _mainmenu;
Account _account;

using namespace std::chrono;

void Bank::openBank() {
	std::cout << "Bank opened";
}

void Bank::addTransaction(std::string accountName, std::string type, double amount, std::string destinationAccountName) {

	Transactions transaction;
	LPCWSTR dirPath = _account.defaultDirPath;
	string filePath = utilityFunction.LPCWSTRToString(dirPath) + "\\" + accountName;
	string accountID = _account.getAccountDetail(filePath, "Account ID");

	string amountInString = std::to_string(amount);
	string date = utilityFunction.getCurrentDateTime();

	transaction.saveTransactionToFile(accountID, type, amountInString, date);
}

void  Bank::Transactions::saveTransactionToFile(string accountID, string type, string amount, string date) {
	string transactionNum = getTransactionNumber();
	
	LPCWSTR dirPath = L"C:\\Users\\kevin\\Desktop\\CPP Projects\\Banking system\\BankSystem\\Main\\Transactions";

	CreateDirectory(dirPath, NULL);

	string filepath = "C:\\Users\\kevin\\Desktop\\CPP Projects\\Banking system\\BankSystem\\Main\\Transactions\\Transactions.txt";
	std::ofstream file(filepath);

	file << "Number: " << endl
		 << "AccountID: " << accountID << endl
		 << "Type: " << type << endl
		 << "Amount: " << amount << endl
		 << "Date&Time: " << date << endl << endl;

	file.close();
}

string Bank::Transactions::getTransactionNumber() {
    string filePath = "C:\\Users\\kevin\\Desktop\\CPP Projects\\Banking system\\BankSystem\\Main\\Transactions\\General.txt";

    string newNum = _account.getAccountDetail(filePath, "num");
    cout << "Oldnum" << newNum;
    int intNum = std::stoi(newNum, NULL);
    intNum++;
    newNum = intNum;
    cout << "Newnum: " << newNum;

    std::ifstream fileIn(filePath);
    if (!fileIn.is_open()) {
        std::cerr << "Failed to open account file." << std::endl;
        return "ERROR";
    }

    std::vector<std::string> lines;
    std::string line;

    // Read all lines from the file
    while (getline(fileIn, line)) {
        lines.push_back(line);
    }
    fileIn.close();

    // Update the password in the vector
    bool found = false;
    for (size_t i = 0; i < lines.size(); i++) {
        if (lines[i].find("Num: ") == 0) {
            lines[i] = "Num: " + newNum;
            found = true;
            break;
        }
    }

    if (!found) {
        std::cerr << "Num value not found in account file" << std::endl;
        return "ERROR";
    }

    // Write the updated lines back to the file
    std::ofstream fileOut(filePath);
    if (!fileOut.is_open()) {
        std::cerr << "Failed to open account file for writing." << std::endl;
        return "ERROR";
    }

    for (const auto& updatedLine : lines) {
        fileOut << updatedLine << std::endl;
    }
    fileOut.close();

    cout << "Number: " << newNum;
    return "ERROR";

}




