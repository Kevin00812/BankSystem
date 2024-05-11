#define _CRT_SECURE_NO_WARNINGS 
#pragma warning(suppress : 4996)
#include <Windows.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <conio.h>
#include <cctype>
#include <codecvt>
#include <locale>
#include "UtilityFunctions.h"
#include "MainMenu.h"
#include <iomanip>
#include <ctime>
#include <sstream>

class MainMenu;

//Trying to do something

using namespace std;
using namespace this_thread;
using namespace chrono;

void UtilityFunctions::setColor(int textColor, int bgColor) { // Default background color is black
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)((bgColor << 4) | textColor));
}

void UtilityFunctions::clearScreen() {
    cout << "\033[2J\033[1;1H";
}

void UtilityFunctions::quitProgram() {
    MainMenu mainmenu;
    int i = 3;
    bool quit = false;

    clearScreen();
    cout << "Press S to return" << endl;

    while (i >= 0) {
        if (_kbhit())
        {
            char ch = _getch();
            if (ch == 'S' || ch == 's')
            {
                quit = true;
                break;
            }
        }

        switch (i) {
        case 3:
            setColor(13); // Magenta
            break;
        case 2:
            setColor(5); // Purple
            break;
        case 1:
            setColor(12); // Red
            break;
        }

        cout << "Quitting in: " << i << "..." << endl;
        i--;
        sleep_for(seconds(1));
    }

    if (quit)
    {
        mainmenu.openMainMenu(true);
    }
    else {
        exit(0);
    }
}

string UtilityFunctions::toLowerCase(const std::string& input) {

    std::string lowercaseString;
    lowercaseString.reserve(input.size()); // Reserve memory to avoid reallocations

    // Convert each character to lowercase
    for (char c : input) {
        lowercaseString += std::tolower(static_cast<unsigned char>(c));
    }

    return lowercaseString;
}

string UtilityFunctions::LPCWSTRToString(LPCWSTR lpcwstr) {
    if (!lpcwstr) return "";  // Handle null pointer

    // First, find out the required length (in characters) to hold the converted string
    int length = WideCharToMultiByte(CP_UTF8, 0, lpcwstr, -1, nullptr, 0, nullptr, nullptr);
    if (length == 0) {
        // Handle the error that may occur during conversion.
        throw std::runtime_error("Failed to convert wide char to multi-byte string.");
    }

    // Allocate a string of appropriate length
    std::string result(length - 1, 0);  // Length includes the null terminator, which we don't need in std::string

    // Now convert from wide characters to multi-byte string
    WideCharToMultiByte(CP_UTF8, 0, lpcwstr, -1, &result[0], length, nullptr, nullptr);

    return result;
}

std::vector<std::string> UtilityFunctions::removeTxt(std::vector<std::string> fileNames) {
    std::vector<string> newFileNames;

    newFileNames.reserve(fileNames.size());  // Pre-allocate memory for efficiency

    for (std::string name : fileNames) {
        std::string newName = name;
        // Check if the filename ends with ".txt"
        if (newName.size() >= 4 && newName.substr(newName.size() - 4) == ".txt") {
            newName.erase(newName.size() - 4);  // Remove the last 4 characters
        }
        newFileNames.push_back(newName);  // Add the modified or unmodified name to the new vector
    }
    return newFileNames;
}

std::string UtilityFunctions::getCurrentDateTime() {
    using namespace std::chrono;

    // Get current time as time_point
    auto now = system_clock::now();
    // Convert to time_t for conversion to tm (broken-down time)
    auto now_c = system_clock::to_time_t(now);
    // Convert to broken-down time
    std::tm* now_tm = std::localtime(&now_c);

    std::ostringstream oss;
    // Use put_time to format the date and time
    oss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}



