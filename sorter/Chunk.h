#ifndef FILEPOOL_H
#define FILEPOOL_H

#include <string.h>

#include <iostream>
#include <fstream>
#include <vector>

const int subvectors_limits=10000000;

using namespace std;

class Chunk
{
private:
    int MemoryLimit;                                                    // Количество даблов, которое каждому из чанков разрешается хранить в памяти
                                                                        //(помним, что по условию задачи мы не можем превышать 100 Мб оперативной памяти)

    std::vector<double>doubles;                                         // Буфер, который мы храним в памяти
    unsigned int begin = 0, end = 0, pointer = 0, total = 0;            // begin, end - позиция начала и конца части файла, которую мы прочитали в буфер
                                                                        // pointer - номер позиции в doubles, в котором находится текущее значение
                                                                        // total - общее количество чисел в файле (вычисляется в конструкторе)

    ifstream subfile;                                                   // Указатель на файловый поток от файла, в котором мы храним чанк
    std::string _fileName;                                              // Имя файла, из которого мы читаем текущий чанк

    void retrieveDoubles();                                             // Загрузить очередную порцию чисел из файла
                                                                        //    (вызывается при достижения pointer-ом doubles.size )

public:
    Chunk(int fileNumber, int memoryLimit);
    ~Chunk();

    bool getNext(double & result);                                      // Получить следующее число из буфера (или загрузить из файла)
    bool Finish();

};

#endif // FILEPOOL_H
