#include <iostream>
#include "Sorter.h"
using namespace std;

int main(int argc, char* argv[])
{
    std::string fileIn = "huge_file.dat";
    std::string fileOut = "huge_file_sorted.dat";
    if (argc)
    {

    }
    Sorter _sorter;
    _sorter.Sort(fileIn,fileOut);
}
