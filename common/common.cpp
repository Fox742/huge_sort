#include "common.h"
#include "iostream"
#include <fstream>

using namespace std;

void Common::ClearScreen()
{
#if defined(_WIN32)
    std::system("cls");
#else
    std::system ("clear");
#endif
}


unsigned int Common::countStrings(std::string path)
{
    ClearScreen();

    unsigned int result=0;

    ifstream fin(path);
    std::string bufer;
    unsigned int lastCountedLines = 0;
    while (getline(fin,bufer))
    {
        if (result-lastCountedLines > 500000)
        {
            ClearScreen();
            std::cout << "Counting file lines amount:"<<std::endl;
            std::cout << "Lines amount: " << result<<std::endl;
            lastCountedLines = result;
        }

        result++;
    }

    fin.close();
    return result;
}
