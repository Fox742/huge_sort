#ifndef COMMON_H
#define COMMON_H

#include <cstdlib>
#include <vector>
#include <string>

class Common
{
public:
    static void ClearScreen();
    static unsigned int countStrings(std::string path);
    static std::string getPercentage( int numerator, int denominator );
    static std::string intToChunkFilename(unsigned int chunkNumber);
};
#endif // COMMON_H
