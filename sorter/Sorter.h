#ifndef SORTER_H
#define SORTER_H

#include <iostream>
#include <string.h>
#include <vector>
using namespace std;

class Sorter
{
private:
    unsigned int concatBuferSize = 100000;
    unsigned int makeSortedChunks(std::string fileIn, unsigned int doublesAmount);
    void concatChunks(std::string fileOut, unsigned int doublesAmount, unsigned int chunkAmount);
    void printChunks(unsigned int LinesNumber,unsigned int chunksCreated, unsigned int totalDoublesAmount);
    void SortSaveVector(std::vector<double>&chunk, int numberOfCurrentChunk);
    //std::string intToChunkFilename(unsigned int chunkNumber);
    void printSorting(unsigned int LinesNumber, unsigned int totalDoublesAmount);
public:
    Sorter(){};
    void Sort(std::string fileIn, std::string fileOut);
};

#endif // SORTER_H
