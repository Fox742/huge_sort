#include "FilePools.h"

FilePools::FilePools(int FilesNumber):/*filesNumber(FilesNumber),*/currentValues(std::vector<double>(FilesNumber,0.0))
{
    for (int i=0;i<FilesNumber;i++)
    {
        _pools.push_back( shared_ptr<FilePool>(new FilePool(i,subvectors_limits/FilesNumber)) );
    }
    for (int i=0;i<FilesNumber;i++)
    {
        double nextValue;
        bool finished = _pools[i]->getNext(nextValue);
        if (!finished)
        {
            currentValues[i]=nextValue;
        }
    }
}

double FilePools::getNext()
{
    unsigned int currentPool;
    for (currentPool=0;currentPool<_pools.size();currentPool++)
    {
        if (!_pools[currentPool]->Finish())
        {
            break;
        }
    }

    int minIndex = currentPool;
    double minValue =currentValues[currentPool];

    for (unsigned int j=currentPool+1;j<_pools.size();j++)
    {
        if (!_pools[j]->Finish())
        {
            if (currentValues[j]<minValue)
            {
                minValue=currentValues[j];
                minIndex=j;
            }
        }
    }

    double nextVal = 0;
    _pools[minIndex]->getNext(nextVal);
    currentValues[minIndex]=nextVal;

    return minValue;

}

bool FilePools::Finish()
{
    for (unsigned int i=0;i<_pools.size();i++)
        if (!_pools[i]->Finish())
            return false;
    return true;
}
