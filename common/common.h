#ifndef COMMON_H
#define COMMON_H

#include <cstdlib>
#include <vector>
#include <string>

#if defined(_WIN32)
#include "windows.h"
#endif

class Common
{
public:
    static void ClearScreen();
    static unsigned int countStrings(std::string path);
    static std::string getPercentage( int numerator, int denominator );
    static std::string intToChunkFilename(unsigned int chunkNumber);
    static void Delete(std::string path);
    static bool fileExists(std::string path);

private:
#if defined(_WIN32)
    static BOOL DeleteFolderWindows(LPCWSTR szPath);
#else
    static void DeleteFolderLinux(const char *dirname);
#endif


};
#endif // COMMON_H
