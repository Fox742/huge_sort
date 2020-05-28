#include "FilePool.h"
#include "common.h"
#include <iostream>

void FilePool::retrieveDoubles()
{
    begin = end;
    end = begin + MemoryLimit;
    unsigned int numberAmount = end-begin;
std::cout << "[[[["<<std::endl;
std::cout << "doubles size="<<doubles.size()<<std::endl;
std::cout << "numberAmount="<<numberAmount<<std::endl;
    doubles.resize(numberAmount+1);
//doubles.clear();
std::cout << "**********"<<std::endl;
//doubles = std::vector<double>(numberAmount/*+1*/);
std::cout << "-----------"<<std::endl;
    subfile.readsome(reinterpret_cast<char*>(&doubles[0]),(numberAmount/*+1*/)*sizeof (double));
std::cout << "]]]]]"<<std::endl;
    pointer = 0;
}


FilePool::FilePool(int fileNumber, int memoryLimit):MemoryLimit(memoryLimit)
{
    std::string fileName = Common::intToChunkFilename(fileNumber);
    subfile.open(fileName, std::ios::binary | std::ios::ate);
    total = ((float)subfile.tellg())/ (8.0);
    subfile.close();
    subfile.open(fileName,std::ios::binary);
}

FilePool::~FilePool()
{
    subfile.close();
}

bool FilePool::getNext(double & result)
{
    if (Finish())
        return true;
    if (begin+pointer>=end)
    {
        this->retrieveDoubles();
    }
    result = doubles[pointer];
    pointer++;
    return false;
}


bool FilePool::Finish()
{
    return begin+pointer>=total;
}
