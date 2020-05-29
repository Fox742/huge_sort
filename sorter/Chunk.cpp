#include "Chunk.h"
#include "common.h"
#include <iostream>

void Chunk::retrieveDoubles()
{
    // конецная позиция становится теперь начальной
    begin = end;
    end = begin + MemoryLimit;
    unsigned int numberAmount = end-begin;
    doubles.resize(numberAmount+1);
    subfile.readsome(reinterpret_cast<char*>(&doubles[0]),(numberAmount/*+1*/)*sizeof (double));
    pointer = 0;
}


Chunk::Chunk(int fileNumber, int memoryLimit):MemoryLimit(memoryLimit)
{
    _fileName = Common::intToChunkFilename(fileNumber);

    // Вычислим общее количество чисел в файле чанка
    subfile.open(_fileName, std::ios::binary | std::ios::ate);
    total = ((float)subfile.tellg())/ (8.0);
    subfile.close();
    subfile.open(_fileName,std::ios::binary);
}

Chunk::~Chunk()
{
    subfile.close();
    Common::Delete(this->_fileName);
}

bool Chunk::getNext(double & result)
{
    if (Finish())
        return true;
    // Проверим нужно ли считать данные их файла
    if (begin+pointer>=end)
    {
        this->retrieveDoubles();
    }
    // берём число из текущей позиции и сдвигаем текущую позицию право
    result = doubles[pointer];
    pointer++;
    return false;
}

bool Chunk::Finish()
{
    // конец это когда мы прочитали все числа из файла
    return begin+pointer>=total;
}
