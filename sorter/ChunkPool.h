#ifndef FILEPOOLS_H
#define FILEPOOLS_H

#include "Chunk.h"
#include <memory>

class ChunkPool
{
    // Хранилище чанков. При содержит объекты всех чанков и предоставляет общий интерфейс по работе с ними
private:
    std::vector<std::shared_ptr<Chunk>>_pools; // Сами чанки
    std::vector<double>currentValues; // Текущие значения каждого из чанков (i-тое currentValue соответствует i-тому чанку в векторе _pools )

public:
    ChunkPool(int FilesNumber); // Количество чанков. Конструктор в цикле от нуля до FilesNumber создаёт для каждого значения объект Chunk
    double getNext(); // Получить следующее значение (см описание внутри метода)
    bool Finish(); // Значения во всех чанках закончились

};

#endif // FILEPOOLS_H
