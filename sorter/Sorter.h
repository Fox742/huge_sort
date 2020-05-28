#ifndef SORTER_H
#define SORTER_H

#include <iostream>
#include <string.h>
#include <vector>
using namespace std;


#if defined(_WIN32)
const int chunk_size=10000000;
#else
const int chunk_size=7000000;
#endif

class Sorter
{
private:
    unsigned int concatBuferSize = 100000;
    unsigned int makeSortedChunks(std::string fileIn, std::string fileOut, unsigned int doublesAmount);
    void concatChunks(std::string fileIn, std::string fileOut, unsigned int doublesAmount, unsigned int chunkAmount);
    void printChunks(unsigned int LinesNumber,unsigned int chunksCreated, unsigned int totalDoublesAmount,std::string fileInPath,std::string fileOutPath);
    void SortSaveVector(std::vector<double>&chunk, int numberOfCurrentChunk);
    void printSorting(unsigned int LinesNumber, unsigned int totalDoublesAmount,std::string fileInPath,std::string fileOutPath);
public:
    Sorter(){};
    void Sort(std::string fileIn, std::string fileOut);
};

#endif // SORTER_H
