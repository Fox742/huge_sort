#include "Sorter.h"
#include "common.h"
#include "PreciseTimer.h"
#include <iostream>
#include <fstream>
#include "FilePool.h"
#include "stdlib.h"
#include <algorithm>
#include "FilePools.h"
#include <string.h>
#include <sstream>

void Sorter::SortSaveVector(std::vector<double>&chunk, int numberOfCurrentChunk)
{
    sort( chunk.begin(),chunk.end() );
    ofstream ot;
    ot.open(Common::intToChunkFilename(numberOfCurrentChunk),std::ios::app|ios::binary);
    ot.write((char*)&chunk[0],sizeof(double)*(chunk.size()));
    ot.flush();
    ot.close();
}

void Sorter::printSorting(unsigned int LinesNumber, unsigned int totalDoublesAmount)
{
    Common::ClearScreen(  );
    std::cout << "Creating sorted file (merging chunks) ..."<<std::endl;
    std::cout << "Numbers printed to file: "
              <<LinesNumber<<"/"<<totalDoublesAmount<<" ("<<Common::getPercentage(LinesNumber,totalDoublesAmount)<<")"<<std::endl;
}

void Sorter::printChunks(unsigned int LinesNumber,unsigned int chunksCreated, unsigned int totalDoublesAmount)
{
    Common::ClearScreen();
    std::cout << "Creating chunks..."<<std::endl;
    std::cout << "Passed: "<<LinesNumber<<" from: "<<totalDoublesAmount
              << " ("<< Common::getPercentage(LinesNumber,totalDoublesAmount) <<")"<<std::endl;

    std::cout << "Chunks created: "<<chunksCreated<<std::endl;
}

unsigned int Sorter::makeSortedChunks(std::string fileIn, unsigned int doublesAmount)
{
    unsigned int chunksAmount=0;
    fstream infile;
    infile.open(fileIn);
    std::vector<double>buffer;
    std::string oneLine;
    unsigned int lineNumber = 0;
    unsigned int lastLinePrinted = 0;
    this->printChunks(lineNumber,chunksAmount,doublesAmount);
    while (getline(infile,oneLine))
    {
         if (lineNumber - lastLinePrinted > 500000)
        {
            this->printChunks(lineNumber,chunksAmount,doublesAmount);
            lastLinePrinted = lineNumber;
        }
        if ( buffer.size() > subvectors_limits )
        {
            this->SortSaveVector(buffer,chunksAmount);
            chunksAmount++;
           buffer.clear();
        }
        buffer.push_back(atof(oneLine.c_str()));
        lineNumber++;
    }
    infile.close();
    this->SortSaveVector(buffer,chunksAmount);
    chunksAmount++;
    buffer.clear();
    return chunksAmount;
}


void Sorter::concatChunks(std::string fileOut, unsigned int doublesAmount, unsigned int chunkAmount)
{
    FilePools pools(chunkAmount);
    std::stringstream sstr;

    unsigned int bufferAmount=0, totalAmount = 0,lastPrintedStatistics=0;

    std::ofstream out;


    while (!pools.Finish())
    {
        if (totalAmount - lastPrintedStatistics >= 100000)
        {
            this->printSorting(totalAmount,doublesAmount);
            lastPrintedStatistics = totalAmount;
        }

        if (bufferAmount >= this->concatBuferSize)
        {
            out.open(fileOut, std::ios::app);
            out << sstr.str();
            out.flush();
            out.close();
            sstr.str(std::string());
            bufferAmount=0;
        }
        sstr << pools.getNext()<<std::endl;
        bufferAmount++;
        totalAmount++;
    }

    out.open(fileOut, std::ios::app);
    out << sstr.str();
    out.flush();
    out.close();
}

void Sorter::Sort(std::string fileIn, std::string fileOut)
{
    PreciseTimer pt;
    pt.startMeasure();
    unsigned int doublesAmount = Common::countStrings(fileIn);
    unsigned int chunksAmount = makeSortedChunks( fileIn, doublesAmount );
    concatChunks(fileOut, doublesAmount,chunksAmount);

    std::cout << "Performance time: " <<pt.stopMeasure()<<std::endl;

}
