#include <iostream>
#include <ostream>
#include <fstream>
#include <limits>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#include <sstream>
#include "common.h"
#include "PreciseTimer.h"

using namespace std;

long GetFileSize(std::string filename)
{
    std::ifstream in_file(filename, std::ios::binary | std::ios::ate);
    long file_size = in_file.tellg();
    return file_size;
}

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

bool fileFull(string failName)
{
    long failSize = GetFileSize(failName);
    if (failSize>1080000000)
        return true;
    return false;
}

int main(int argc, char* argv[])
{
    string fileName = "huge_file.txt";
    if (argc>1)
    {
        fileName = argv[1];
    }


    // Стираем старую версию файла
    if (Common::fileExists(fileName))
    {
        Common::Delete(fileName);
    }

    PreciseTimer timer;
    timer.start();
    while (!fileFull(fileName))
    {
        std::stringstream sstr;
        for (int i=0;i<500000;i++)
        {
            sstr << fRand( std::numeric_limits<double>::min(),std::numeric_limits<double>::max() )<<std::endl;
        }
        std::ofstream out(fileName, std::ios::app);
        if (out.is_open())
        {
            out << sstr.str();
        }
        out.close();

        long fileSize = GetFileSize(fileName);

        Common::ClearScreen();
        cout << "Generating of huge file. Name of the file: ["<<fileName<<"]"<<std::endl;
        cout << "Generated Mbytes: "<<fileSize/1000000<<std::endl;
    }
    cout<< "Generated!"<< " Time was spent: " << timer.stop() <<std::endl;
    cout<< "Press any key to quit"<<std::endl;
    getchar();
    return 0;
}
