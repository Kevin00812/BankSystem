#ifndef  UTILITYFUNCTION_H
#define  UTILITYFUNCTION_H
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <string>
#include <Windows.h>
#include <vector>

class UtilityFunctions {

public:

    void setColor(int textColor = 7, int bgColor = 0);

    void clearScreen();

    void quitProgram();

    std::string toLowerCase(const std::string& input);

    std::string LPCWSTRToString(LPCWSTR lpcwstr);

    std::vector<std::string> removeTxt(std::vector<std::string> fileNames);

    std::string getCurrentDateTime();
};

extern UtilityFunctions utilityFunction;

#endif 
