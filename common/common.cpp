#include "common.h"
#include "iostream"
#include <fstream>

#if defined(_WIN32)
#include "windows.h"
#else
#include <unistd.h>
#endif

using namespace std;

void Common::ClearScreen()
{
#if defined(_WIN32)
    std::system("cls");
#else
    std::system ("clear");
#endif
}

std::string Common::getPercentage( int numerator, int denominator )
{
    int resultNumber = 0;
    if (denominator!=0)
    {
        resultNumber = (int)(  ((float)numerator / (float)denominator)*100   );
    }
    return to_string(resultNumber)+" %";

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


std::string Common::intToChunkFilename(unsigned int chunkNumber)
{
    return to_string(chunkNumber)+".huge_sort.chunk";
}

void Common::HugeSortSleep(unsigned int msToSleep)
{
#if defined(_WIN32)
    Sleep(msToSleep);
#else
    usleep(msToSleep);
#endif
}