#include <iostream>
#include <string>

#include "MainMenu.h"

using std::cout;
using std::endl;
using std::cin;

void MainMenu::manageInput(std::string input) {
	input = utilityFunction.toLowerCase(input);


	if (input == "1" || input == "account")
	{
		utilityFunction.clearScreen();
		account.startAccount();
	}
	else if (input == "2" || input == "bank") {
		utilityFunction.clearScreen();
		bank.openBank();
	}
	else if(input == "3" || input == "quit") {
		utilityFunction.quitProgram();
	}
	else {
		cout << "Input is invalid, please enter a valid input. (Press Enter to continue)";
		cin;
		openMainMenu(true);
	}
}


void MainMenu::openMainMenu(bool _clearScreen) {
	if (_clearScreen) {
		utilityFunction.clearScreen();
	}

	std::string line;

	utilityFunction.setColor(3);
	cout << "Kevin Bank - Welcome to the main menu" << endl;
	utilityFunction.setColor(7);

	cout << "1. Account" << endl
		<< "2. Bank (Employees only)" << endl <<
		   "3. Quit" << endl;

	cout << "Select an option: ";
	cin >> std::ws;
	std::getline(cin, line);
	manageInput(line);
}