#ifndef MAINMENU_H
#define MAINMENU_H
#include <string>
#include "UtilityFunctions.h"
#include "Account.h"
#include "Bank.h"

class Account;
class Bank;


class MainMenu {
private: 
	UtilityFunctions utilityFunction;
	Account account;
	Bank bank;
	void manageInput(std::string input);

public:
	void openMainMenu(bool _clearScreen = false);
};

#endif