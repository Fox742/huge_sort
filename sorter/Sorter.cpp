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
#include "common.h"

void Sorter::SortSaveVector(std::vector<double>&chunk, int numberOfCurrentChunk)
{
    sort( chunk.begin(),chunk.end() );
    ofstream ot;
    std::string chunkFileName = Common::intToChunkFilename(numberOfCurrentChunk);
    if (Common::fileExists(chunkFileName))
    {
        Common::Delete(chunkFileName);
    }
    ot.open(chunkFileName,std::ios::app|ios::binary);
    if (!ot.is_open())
    {
        throw std::string ("Could not open file for chunk: " + chunkFileName);
    }
    ot.write((char*)&chunk[0],sizeof(double)*(chunk.size()+1));
    ot.flush();
    ot.close();
}

void Sorter::printSorting(unsigned int LinesNumber, unsigned int totalDoublesAmount,std::string fileInPath,std::string fileOutPath)
{
    Common::ClearScreen(  );
    std::cout << "Creating sorted file - merging chunks ... (Sorting " << fileInPath<<" -> " << fileOutPath << ")" <<std::endl;
    std::cout << "Numbers printed to file: "
              <<LinesNumber<<"/"<<totalDoublesAmount<<" ("<<Common::getPercentage(LinesNumber,totalDoublesAmount)<<")"<<std::endl;
}

void Sorter::printChunks(unsigned int LinesNumber,unsigned int chunksCreated, unsigned int totalDoublesAmount,std::string fileInPath,std::string fileOutPath)
{
    Common::ClearScreen();
    std::cout << "Creating chunks ... (Sorting "<<fileInPath<<" -> " << fileOutPath << ")" <<std::endl;
    std::cout << "Passed: "<<LinesNumber<<" from: "<<totalDoublesAmount
              << " ("<< Common::getPercentage(LinesNumber,totalDoublesAmount) <<")"<<std::endl;

    std::cout << "Chunks created: "<<chunksCreated<<std::endl;
}

unsigned int Sorter::makeSortedChunks(std::string fileIn, std::string fileOut, unsigned int doublesAmount)
{
    unsigned int chunksAmount=0;
    fstream infile;
    infile.open(fileIn);
    if (!infile.is_open())
    {
        throw std::string("Could not open the input file"); // Вообще, для таких исключений неплохо было бы сделать отдельный класс исключений
    }
    std::vector<double>buffer;
    std::string oneLine;
    unsigned int lineNumber = 0;
    unsigned int lastLinePrinted = 0;
    this->printChunks(lineNumber,chunksAmount,doublesAmount,fileIn,fileOut);
    while (getline(infile,oneLine))
    {
         if (lineNumber - lastLinePrinted > 500000)
        {
            this->printChunks(lineNumber,chunksAmount,doublesAmount,fileIn,fileOut);
            lastLinePrinted = lineNumber;
        }
        if ( buffer.size() > chunk_size )
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
    this->printChunks(lineNumber,chunksAmount,doublesAmount,fileIn,fileOut);
    return chunksAmount;
}


void Sorter::concatChunks(std::string fileIn, std::string fileOut, unsigned int doublesAmount, unsigned int chunkAmount)
{
    FilePools pools(chunkAmount);
    std::stringstream sstr;

    unsigned int bufferAmount=0, totalAmount = 0,lastPrintedStatistics=0;

    std::ofstream out;


    while (!pools.Finish())
    {
        if (totalAmount - lastPrintedStatistics >= 100000)
        {
            this->printSorting(totalAmount,doublesAmount,fileIn,fileOut);
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
    this->printSorting(totalAmount,doublesAmount,fileIn,fileOut);
}

void Sorter::Sort(std::string fileIn, std::string fileOut)
{
    if (Common::fileExists(fileOut))
    {
        Common::Delete(fileOut);
    }

    PreciseTimer pt;
    pt.start();
    unsigned int doublesAmount = Common::countStrings(fileIn);
    unsigned int chunksAmount = makeSortedChunks( fileIn, fileOut, doublesAmount);
    concatChunks(fileIn, fileOut, doublesAmount,chunksAmount);
    std::cout << "Performance time: " <<pt.stop()<<std::endl;

}
