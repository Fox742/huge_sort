#include <iostream>
#include <istream>
#include <fstream>
#include <time.h>
#include "common.h"

using namespace std;

/*
void ClearScreen()
{
#if defined(_WIN32)
    std::system("cls");
#else
    std::system ("clear");
#endif
}

unsigned int countStrings(std::string path)
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

*/

bool checkSort(std::string path, unsigned int linesAmount)
{
    bool result = true;
    std::string bufer;

    unsigned int lastCheckedLinesPrinted = 0;
    unsigned int currentLine =0;
    double previousChecked = 0;

    ifstream fin(path);
    while (getline(fin,bufer))
    {
        if (currentLine-lastCheckedLinesPrinted > 1000)
        {
            Common::ClearScreen();
            std::cout << "Checking order in file:"<<std::endl;
            std::cout << "Lines passed: " << currentLine << "\t"<< "Total: "<< linesAmount <<std::endl;
            lastCheckedLinesPrinted = currentLine;
        }

        double currentNumber = atof(bufer.c_str());
        if (currentLine)
        {
            if (currentNumber < previousChecked)
            {
                return false;
            }
        }
        previousChecked = currentNumber;
        currentLine++;
    }

    fin.close();


    return result;
}

int main(int argc, char* argv[])
{

	clock_t start = clock();
    std::string filename = "huge_file_sorted.dat";
    //std::string filename = "0.dat";
    unsigned int stringAmount = Common::countStrings(filename);


    /*
    std::cout << "Number of strings: " << stringAmount <<std::endl;
    */
    bool isSorted = checkSort(filename,stringAmount);
    //Common::ClearScreen();
    std::cout << "argv[0]=" << argv[0]<<std::endl;
    if (isSorted)
    {
        std::cout << "Sorted!"<<std::endl;
    }
    else
    {
        std::cout << "Is not sorted!"<<std::endl;
    }

    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    cout<< "Minutes were spent: " << seconds/60<<std::endl;
    cout<< "Press any key to quit"<<std::endl;
    getchar();
}
