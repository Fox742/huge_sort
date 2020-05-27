#ifndef FILEPOOLS_H
#define FILEPOOLS_H

#include "FilePool.h"
#include <memory>

class FilePools
{
private:
    //int filesNumber = -1;
    std::vector<std::shared_ptr<FilePool>>_pools;
    std::vector<double>currentValues;

public:
    FilePools(int FilesNumber);
    double getNext();
    bool Finish();

};

#endif // FILEPOOLS_H
