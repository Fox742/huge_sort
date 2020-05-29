#include "ChunkPool.h"
#include "common.h"

ChunkPool::ChunkPool(int FilesNumber):/*filesNumber(FilesNumber),*/currentValues(std::vector<double>(FilesNumber,0.0))
{
    for (int i=0;i<FilesNumber;i++)
    {
	
        _pools.push_back( shared_ptr<Chunk>(new Chunk(i,subvectors_limits/FilesNumber)) );
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

double ChunkPool::getNext()
{
    unsigned int currentPool;
    // Проходим по массиву чанков до первого чанка, в котором ещё остались значения
    for (currentPool=0;currentPool<_pools.size();currentPool++)
    {
        if (!_pools[currentPool]->Finish())
        {
            break;
        }
    }

    // Находим минимальное из текущих значений всех чанков
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

    // Сохраняем минимальное значение (чтобы его вернуть клиентскому коду)
    //   И чанку, у которого мы взяли значение - вызываем getNext, чтобы взять его следующее значение и положить в буфер currentValues
    double nextVal = 0;
    _pools[minIndex]->getNext(nextVal);
    currentValues[minIndex]=nextVal;

    return minValue;

}

bool ChunkPool::Finish()
{
    for (unsigned int i=0;i<_pools.size();i++)
        if (!_pools[i]->Finish())
            return false;
    return true;
}
