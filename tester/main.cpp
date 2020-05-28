#include <iostream>
#include <istream>
#include <fstream>
#include <time.h>
#include "common.h"
#include "PreciseTimer.h"

using namespace std;

void printCheckStatus(unsigned int currentLine, unsigned int linesAmount, std::string fileName)
{
    Common::ClearScreen();
    std::cout << "Checking order in file ["<< fileName <<"]"<<std::endl;
    std::cout << "Lines passed: " << currentLine << "\t("<< Common::getPercentage(currentLine,linesAmount)<<")\tTotal: "<< linesAmount <<std::endl;
}

bool checkSort(std::string path, unsigned int linesAmount)
{
    bool result = true;
    std::string bufer;

    unsigned int lastCheckedLinesPrinted = 0;
    unsigned int currentLine =0;
    double previousChecked = 0;

    ifstream fin(path);
    if (!fin.is_open())
    {
        throw std::string("Could not open input file!");
    }
    while (getline(fin,bufer))
    {
        if (currentLine-lastCheckedLinesPrinted > 1000)
        {
            printCheckStatus(currentLine,linesAmount,path);
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
    printCheckStatus(currentLine,linesAmount,path);
    return result;
}

int main(int argc, char* argv[])
{

    std::string filename = "huge_file_sorted.txt";

    if (argc>1)
    {
        filename = std::string(argv[1]);
    }

    PreciseTimer timer;
    timer.start();
    try
    {
        unsigned int stringAmount = Common::countStrings(filename);
        if (!stringAmount)
        {
            std::cout << "File empty"<<std::endl;
        }
        else
        {
            bool isSorted = checkSort(filename,stringAmount);
            if (isSorted)
            {
                std::cout << "Sorted!"<<std::endl;
            }
            else
            {
                std::cout << "Is not sorted!"<<std::endl;
            }

            cout<< "Time was spent: " << timer.stop() <<std::endl;
        }
    }
    catch (std::string errorString)
    {
        std::cout << "Fatal Error: "<<errorString<<std::endl;
    }
}
