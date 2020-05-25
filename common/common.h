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
};
#endif // COMMON_H
