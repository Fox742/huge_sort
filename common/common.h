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
    // Класс с общими для всех компонентов проекта
public:
    static void ClearScreen(); // Очистка экрана
    static unsigned int countStrings(std::string path); // Посчитать строки в файле для отображение статуса процесса сортировки/проверки сортированного файла
    static std::string getPercentage( int numerator, int denominator ); // Вычсление процентов nominator-а от denominator-а и преобразовние результата в строку
    static std::string intToChunkFilename(unsigned int chunkNumber); // Генерация имени чанка из номера
    static void Delete(std::string path); // Удаление файла/папки по его path-у
    static bool fileExists(std::string path); // Существует ли файл?

private:
    static bool isDirectory(std::string path); // path является ли папкой?

#if defined(_WIN32) // Фукнции для удаления файла/папки для разных операционных систем
    static BOOL DeleteFolderWindows(LPCWSTR szPath);
#else
    static void DeleteFolderLinux(const char *dirname);
#endif


};
#endif // COMMON_H
