#include "Account.h"
#include <iostream>
#include <string>
#include <vector>
#include "MainMenu.h"
#include "Bank.h"
#include <fstream>
#include <Windows.h>
#include <filesystem>
#include <codecvt>
#include <locale>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

Bank bank;

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ws;
using std::string;

void Account::openDeposit(string accountName) {
	double depositAmount;
    string filePath = "C:\\Users\\kevin\\Desktop\\CPP Projects\\Banking system\\BankSystem\\Main\\Accounts\\" + accountName;

	cout << "Enter amount to deposit ($): ";
	cin >> depositAmount;

	cout << endl << "Processing..." << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    adjustAccountBalance(filePath, depositAmount, true);
	cout << "$" << depositAmount << " has been added to your account. (Press Enter to continue)" << endl;

    cin.get();
    system("cls");
    openAccount(accountName);
}

void Account::openTransfer(string accountName) {

    // Get sorted account details
    std::vector<std::pair<int, std::string>> accountDetails = getAccountDetailsSortedByNumber(L"C:\\Users\\kevin\\Desktop\\CPP Projects\\Banking system\\BankSystem\\Main\\Accounts");
    int accountAmount = accountDetails.size();

    if (accountAmount <= 1) {
        cout << "Not enough accounts for transfer. (Press Enter to continue)" << endl;
        cin.get();
        return;  // Exit if not enough accounts are available for transfer
    }

    utilityFunction.setColor(3);
    cout << "Select an account to transfer to (number only):" << endl;
    utilityFunction.setColor(7);

    // Display the sorted account names for selection
    for (size_t i = 0; i < accountAmount; i++) {
        cout << (i + 1) << ". " << accountDetails[i].second << endl;
    }

    string line;
    cout << "Enter the number of the account to transfer to: ";
    cin >> ws;
    getline(cin, line);
    int numLine = std::stoi(line) - 1;  // Adjust for zero-based index

    if (numLine < 0 || numLine >= accountAmount || numLine == findIndexByName(accountDetails, accountName)) {
        cout << "Invalid account number or cannot transfer to the same account. (Press Enter to continue)" << endl;
        cin.get();
        return;
    }

    string destinationFilePath = "C:\\Users\\kevin\\Desktop\\CPP Projects\\Banking system\\BankSystem\\Main\\Accounts\\" + accountDetails[numLine].second;

    double transferAmount;
    cout << "Enter amount to transfer ($): ";
    cin >> transferAmount;

    // Process the transfer
    cout << endl << "Processing..." << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    string currentFilePath = "C:\\Users\\kevin\\Desktop\\CPP Projects\\Banking system\\BankSystem\\Main\\Accounts\\" + accountName;
    adjustAccountBalance(currentFilePath, transferAmount, false); // Subtract funds from current account
    adjustAccountBalance(destinationFilePath, transferAmount, true); // Add funds to other account

    system("cls");
    openAccount(accountName);
}

void Account::openWithdraw(string accountName) {
    double withdrawAmount;
    string filePath = "C:\\Users\\kevin\\Desktop\\CPP Projects\\Banking system\\BankSystem\\Main\\Accounts\\" + accountName;

    cout << "Enter amount to withdraw ($): ";
    cin >> withdrawAmount;

    cout << endl << "Processing..." << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    adjustAccountBalance(filePath, withdrawAmount, false);

    system("cls");
    openAccount(accountName);
}

void Account::openChangePassword(string accountName) {
    string passwordAttempt;
    string newPassword;
    string filePath = "C:\\Users\\kevin\\Desktop\\CPP Projects\\Banking system\\BankSystem\\Main\\Accounts\\" + accountName;

    utilityFunction.setColor(3);
    cout << accountName << " - Change account password" << endl;
    utilityFunction.setColor(7);

    attempt:
    cout << "Enter current password: ";
    std::getline(cin, passwordAttempt);

    string password = getAccountDetail(filePath, "Account Password");

    if (passwordAttempt == password)
    {
        utilityFunction.setColor(2);
        cout << endl << "Please enter new password: ";
        utilityFunction.setColor(7);
        std::getline(cin, newPassword);

        changePassword(filePath, newPassword);
        system("cls");
        openAccount(accountName);
    }
    else if (passwordAttempt == "exit" || passwordAttempt == "Exit") {
        system("cls");
        openAccount(accountName);
    }
    else {
        cout << "Wrong password, please try again, or exit by typing \"exit\"" << endl;
        goto attempt;
    }

}

void Account::adjustAccountBalance(string filePath, int amount, bool isDeposit) {

    std::ifstream inFile(filePath);
    std::vector<std::string> lines;
    std::string line;
    bool balanceUpdated = false;

    if (!inFile.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    // Read the file line by line and look for the balance line
    while (getline(inFile, line)) {
        if (line.find("Account Balance: ") == 0) {
            // Extract the current balance
            std::string currentBalanceStr = line.substr(17); // "Account Balance: " is 17 characters
            int currentBalance = std::stoi(currentBalanceStr);

            // Modify the balance
            currentBalance = isDeposit ? (currentBalance + amount) : (currentBalance - amount);

            // Create the new balance line
            line = "Account Balance: " + std::to_string(currentBalance);
            balanceUpdated = true;
        }
        lines.push_back(line);
    }
    inFile.close();

    // Write the updated content back to the file
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        return;
    }
    for (const std::string& outLine : lines) {
        outFile << outLine << std::endl;
    }
    outFile.close();

    // Uncomment if debug is needed
    /*if (balanceUpdated) {
        std::cout << "Balance successfully updated." << std::endl;
    }
    else {
        std::cout << "Balance update failed: No balance found in file." << std::endl;
    }*/ 
}

int Account::findIndexByName(const std::vector<std::pair<int, std::string>>& accountDetails, const std::string& accountName) {
    for (size_t i = 0; i < accountDetails.size(); i++) {
        if (accountDetails[i].second == accountName + ".txt") {
            return i;
        }
    }
    return -1;
}

void Account::changePassword(string filePath, string newPassword) {
    std::ifstream fileIn(filePath);
    if (!fileIn.is_open()) {
        std::cerr << "Failed to open account file." << std::endl;
        return;
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
        if (lines[i].find("Account Password: ") == 0) {
            lines[i] = "Account Password: " + newPassword;
            found = true;
            break;
        }
    }

    if (!found) {
        std::cerr << "Password entry not found in account file." << std::endl;
        return;
    }

    // Write the updated lines back to the file
    std::ofstream fileOut(filePath);
    if (!fileOut.is_open()) {
        std::cerr << "Failed to open account file for writing." << std::endl;
        return;
    }

    for (const auto& updatedLine : lines) {
        fileOut << updatedLine << std::endl;
    }
    fileOut.close();
}


