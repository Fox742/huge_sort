#include <iostream>
#include "Sorter.h"
using namespace std;

int main(int argc, char* argv[])
{
    std::string fileIn = "huge_file.txt";
    std::string fileOut = "huge_file_sorted.txt";
    if (argc>1)
    {
        fileIn = std::string(argv[1]);
    }
    if (argc>2)
    {
        fileOut = std::string(argv[2]);
    }

    try
    {
        Sorter _sorter;
        _sorter.Sort(fileIn,fileOut);
    }
    catch (std::string errorString)
    {
        std::cout << "Fatal error occured: "<<errorString<<std::endl;
    }
    return 0;
}
