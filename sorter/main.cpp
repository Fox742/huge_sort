#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <limits>

#include "PreciseTimer.h"
using namespace std;


//const int memory_limit=49000000;
//const int memory_limit=24000000;
const int subvectors_limits=6000000;

class FilePool
{
private:
    int MemoryLimit;
    string fileName;
    double * doubles;
    int pointer = 0;
    int end = 0;
    ifstream subfile;
    int total = 0;
    int begin = 0;

    void retrieveDoubles()
    {
        begin = end;
        if (total - pointer>MemoryLimit)
        {

            end = begin + MemoryLimit;
        }
        else
        {
            end = total;
        }
        if (begin<end)
        {
            int sx = subfile.readsome(reinterpret_cast<char*>(doubles),(end-begin)*8);
        }
        pointer = 0;
    }

public:
    FilePool(int fileNumber, int memoryLimit):fileName( to_string(fileNumber)+".dat"),MemoryLimit(memoryLimit)
    {
        doubles = new double[memoryLimit];
        subfile.open(fileName, std::ios::binary | std::ios::ate);
        total = ((float)subfile.tellg())/ (8.0);
        subfile.close();
        subfile.open(fileName,std::ios::binary);
    }

    ~FilePool()
    {
        subfile.close();
        // remove(fileName.c_str());
    }

    bool getNext(double & result)
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

    bool Finish()
    {
        return begin+pointer>=total;
    }

};

class FilePools
{
private:
    int filesNumber = -1;
    std::vector<FilePool*>_pools;
    std::vector<double>currentValues;

public:
    FilePools(int FilesNumber):filesNumber(FilesNumber)
    {
        for (int i=0;i<FilesNumber;i++)
        {
            _pools.push_back( new FilePool(i,subvectors_limits/FilesNumber) );
            currentValues.push_back(0);
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

    double getNext()
    {
        int currentPool;
        for (currentPool=0;currentPool<_pools.size();currentPool++)
        {
            if (!_pools[currentPool]->Finish())
            {
                break;
            }
        }

        int minIndex = currentPool;
        double minValue =currentValues[currentPool];

        for (int j=currentPool+1;j<_pools.size();j++)
        {
            if (!_pools[j]->Finish())
            {
                minValue=currentValues[j];
                minIndex=j;
            }
        }

        double nextVal = 0;
        _pools[minIndex]->getNext(nextVal);
        currentValues[minIndex]=nextVal;

        return minValue;

    }

    bool Finish()
    {
        for (int i=0;i<_pools.size();i++)
            if (!_pools[i]->Finish())
                return false;
        return true;
    }

    ~FilePools()
    {

        for (int i=0; i<filesNumber; i++)
        {
            delete _pools[i];
        }

    }
};

void SortSaveVector(double * vec, int size,int number)
{

    sort(vec,vec+size);


    ofstream ot(to_string(number)+".dat",ios::out|ios::binary);
    ot.write((char*)vec,sizeof(double)*size);
    ot.close();
}

int main(int argc, char* argv[])
{
        string fileIn = "huge_file.dat";
        string fileOut = "huge_file_sorted.dat";
        if (argc)
        {

        }


        clock_t start = clock();
        int subArrNumber = 0;
        //subArrNumber=13;

        double * vec;
        vec = new double[subvectors_limits * sizeof(double) ];
        int vecSize = 0;

        // -----

        fstream infile;
        infile.open(fileIn);
        double number = -1;
        int i=0;



        PreciseTimer pt;
        pt.startMeasure();
        std::string bufer;
        while (getline(infile,bufer))
        {
            if ( vecSize > subvectors_limits )
            {

                SortSaveVector(vec,vecSize,subArrNumber);
                std::cout << "Sorted: "<<pt.stopMeasure()<<std::endl;
                pt.startMeasure();
                subArrNumber++;
                vecSize = 0;
            }

            number = atof(bufer.c_str());
            vec[vecSize]=number;
            vecSize++;
        }
        infile.close();
        SortSaveVector(vec,vecSize,subArrNumber);
        subArrNumber++;
        vecSize = 0;

        delete vec;
        //----

        FilePools * pools = new FilePools(subArrNumber);
        long long TotalNumberOfDoubles=0;
        while (!pools->Finish())
        {


            if (vecSize>1000)
            {
                std::stringstream str;
                for (int i=0;i<vecSize;i++)
                {
                    str <<vec[i]<<std::endl;
                }

                std::ofstream out(fileOut, std::ios::app);
                out << str.str();
                out.close();

                vecSize = 0;
            }

            // Запись числа в выходной файл
            vec[vecSize]=pools->getNext();
            vecSize++;
            TotalNumberOfDoubles++;
            //out<<pools->getNext();
            //wroteNumbers++;

        }

        std::ofstream out_finishing(fileOut, std::ios::app);
        for (int i=0;i<vecSize;i++)
        {
            out_finishing << vec[i]<<std::endl;
        }
        out_finishing.close();

        delete pools;

        clock_t end = clock();
        double seconds = (double)(end - start) / CLOCKS_PER_SEC;
        cout<< "Done!"<< " Minutes were spent: " << seconds/60<<std::endl;





        cout<< "Press any key to quit"<<std::endl;
        getchar();

}


/*
#include <iostream>
#include "common.h"
#include <float.h>
#include <fstream>
#include "PreciseTimer.h"

using namespace std;
int commonLinesNumber = 0; // Никогда так не делайте!
unsigned int maxVectorSize = 50000;
int numberToIgnore = 0;
int lastNumberToIgnore = 0;

int findIndex(std::vector<double>& resultVector, double value)
{
    // Если размер максимален и число больше последнего элемента - добавлять его не нужно (оно будет добавлено в следующем прохоже)
    if (resultVector.size() >= maxVectorSize)
    {
        if (value >= resultVector[resultVector.size()-1] )
        {
            return -1;
        }
        // Уменьшаем размер вектора на 1
        resultVector.pop_back();
    }

    if ( !resultVector.size() || value < resultVector[0])
    {
        return 0;
    }

    if (value >= resultVector[resultVector.size()-1])
        return resultVector.size();

    unsigned int left=0;
    unsigned right = resultVector.size()-1;
    while ( right - left > 1 )
    {
        unsigned int middle = (left+right) / 2;
        if (value < resultVector[middle])
        {
            right = middle;
        }
        else
        {
            left = middle;
        }
    }
    return right;
}

void insertValue(std::vector<double>& resultVector, double value)
{
    // Двоичный поиск индекса, вместо которого нужно вставить
    int index = findIndex(resultVector,value);
    // Вставка
    if (index >=0)
    {
        resultVector.insert(resultVector.begin() + index,value);
    }
}

bool oneStep(std::string inputFile, std::string outputFile,int stepNumber, double beginningValue , std::vector<double>& resultVector)
{
    bool result = false;
    std::string bufer;

    unsigned int lastPrinted = 0;
    unsigned int currentLine = 0;

    ifstream fin(inputFile);
    while (getline(fin,bufer))
    {
        if (currentLine-lastPrinted > 10000)
        {
            Common::ClearScreen();
            std::cout << "Pass through: "<< stepNumber <<std::endl;
            std::cout << "Last Number to ignore: "<<lastNumberToIgnore<<std::endl;
            std::cout << "Lines passed: " << currentLine << "\t"<< "Total: "<< commonLinesNumber <<std::endl;
            lastPrinted = currentLine;
        }
        double currentNumber = atof(bufer.c_str());
        if (currentNumber>beginningValue)
        {
            result = true;
            insertValue(resultVector,currentNumber);
        }
        else
        {
            numberToIgnore++;
        }
        currentLine++;
    }
    fin.close();
    return result;
    //return false;
}


void Sort(std::string inputFile, std::string outputFile)
{

    int stepCounter=0;
    bool Continue = true;
    std::vector<double>sortedValues;
    double beginValue = -DBL_MAX;
    do
    {
        numberToIgnore = 0;
        Continue = oneStep(inputFile,outputFile,stepCounter,beginValue,sortedValues);
        beginValue = sortedValues[sortedValues.size()-1];
        stepCounter++;
        lastNumberToIgnore = numberToIgnore;
        sortedValues.clear();
    }
    while (Continue);
}


int main()
{


    std::string inputFile = "huge_file.dat";
    std::string outputFile = "huge_file_sorted.dat";

    commonLinesNumber = Common::countStrings(inputFile);

    PreciseTimer pt;
    pt.startMeasure();
    Sort(inputFile,outputFile);
    double timeSpent = pt.stopMeasure();


    std::cout << "Time spent: "<<timeSpent<<std::endl;


    std::cout << "Sorted!"<<std::endl;

    return 0;
}
*/
