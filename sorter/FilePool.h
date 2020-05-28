#ifndef FILEPOOL_H
#define FILEPOOL_H

#include <string.h>

#include <iostream>
#include <fstream>
#include <vector>

const int subvectors_limits=10000000;

using namespace std;

class FilePool
{
private:
    int MemoryLimit;
    std::vector<double>doubles;
    unsigned int begin = 0, end = 0, pointer = 0, total = 0;
    ifstream subfile;
    std::string _fileName;

    void retrieveDoubles();

public:
    FilePool(int fileNumber, int memoryLimit);
    ~FilePool();

    bool getNext(double & result);
    bool Finish();

};

#endif // FILEPOOL_H
