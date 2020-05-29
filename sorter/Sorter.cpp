#include "Sorter.h"
#include "common.h"
#include "PreciseTimer.h"
#include <iostream>
#include <fstream>
#include "Chunk.h"
#include "stdlib.h"
#include <algorithm>
#include "ChunkPool.h"
#include <string.h>
#include <sstream>
#include "common.h"

void Sorter::SortSaveVector(std::vector<double>&chunk, int numberOfCurrentChunk)
{
    // Сортируем чанк
    sort( chunk.begin(),chunk.end() );
    ofstream ot;
    std::string chunkFileName = Common::intToChunkFilename(numberOfCurrentChunk);

    // Если файл с чанком до этого существовал - надо удалить старый
    if (Common::fileExists(chunkFileName))
    {
        Common::Delete(chunkFileName);
    }

    // Открывается ли файл?
    ot.open(chunkFileName,std::ios::app|ios::binary);
    if (!ot.is_open())
    {
        throw std::string ("Could not open file for chunk: " + chunkFileName);
    }

    // Скидываем чанк в файл
    ot.write((char*)&chunk[0],sizeof(double)*(chunk.size()+1));
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
    // Проходим по входному неотсортированному файлу
    while (getline(infile,oneLine))
    {
         if (lineNumber - lastLinePrinted > 500000)
        {
            this->printChunks(lineNumber,chunksAmount,doublesAmount,fileIn,fileOut);
            lastLinePrinted = lineNumber;
        }
        // Размер чанка ограничен чтобы не выходил за ограничение в 100 мб
        if ( buffer.size() > chunk_size )
        {
            // Сортируем буфер и записываем каждый чанк в отдельный файл с именем, соответствующему номеру чанка
            this->SortSaveVector(buffer,chunksAmount);
            chunksAmount++;
           buffer.clear();
        }
        // Заносим текущее значение в вектор
        buffer.push_back(atof(oneLine.c_str()));
        lineNumber++;
    }
    infile.close();

    // Сортируем последний чанк
    this->SortSaveVector(buffer,chunksAmount);
    chunksAmount++;
    buffer.clear();
    this->printChunks(lineNumber,chunksAmount,doublesAmount,fileIn,fileOut);
    return chunksAmount;
}


void Sorter::mergeChunks(std::string fileIn, std::string fileOut, unsigned int doublesAmount, unsigned int chunkAmount)
{
    // Создаём пул из чанков. Каждый пул содержит файловый поток на файл, в котором мы сохранили чанк и вектор, в котором мы храним часть чанка
    ChunkPool pools(chunkAmount);
    std::stringstream sstr;

    unsigned int bufferAmount=0, totalAmount = 0,lastPrintedStatistics=0;

    std::ofstream out;

    // Пока хотя бы в одном чанке остались не добавленные числа
    while (!pools.Finish())
    {
        if (totalAmount - lastPrintedStatistics >= 100000)
        {
            this->printSorting(totalAmount,doublesAmount,fileIn,fileOut);
            lastPrintedStatistics = totalAmount;
        }

        // Скидываем буферную строку в выходной файл
        if (bufferAmount >= this->concatBuferSize)
        {
            out.open(fileOut, std::ios::app);
            out << sstr.str();
            out.close();
            sstr.str(std::string());
            bufferAmount=0;
        }
        // Берём значение из пула чанков - и записываем в выходную строку
        //   После того как в строке набирается некоторое большое количество чисел -> скидываем её в выходной отсортированный файл
        sstr << pools.getNext()<<std::endl;
        bufferAmount++;
        totalAmount++;
    }

    // Скидываем в выходной файл последную буферную строку
    out.open(fileOut, std::ios::app);
    out << sstr.str();
    out.close();
    this->printSorting(totalAmount,doublesAmount,fileIn,fileOut);
}

void Sorter::Sort(std::string fileIn, std::string fileOut)
{
    // Если выходной файл уже есть - удаляем его
    if (Common::fileExists(fileOut))
    {
        Common::Delete(fileOut);
    }

    PreciseTimer pt;
    pt.start();
    // Считаем строки
    unsigned int doublesAmount = Common::countStrings(fileIn);
    // Создаём чанки, сортируем их и сохраняем каждый из них в отдельный файл
    unsigned int chunksAmount = makeSortedChunks( fileIn, fileOut, doublesAmount);
    // Смёрживаем чанки
    mergeChunks(fileIn, fileOut, doublesAmount,chunksAmount);

    std::cout << "Performance time: " <<pt.stop()<<std::endl;

}
