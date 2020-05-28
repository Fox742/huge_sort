#include "common.h"
#include "iostream"
#include <fstream>

#if defined(_WIN32)
#include "windows.h"
#else
#include <unistd.h>
#endif

using namespace std;

void Common::ClearScreen()
{
#if defined(_WIN32)
    std::system("cls");
#else
    std::system ("clear");
#endif
}

std::string Common::getPercentage( int numerator, int denominator )
{
    int resultNumber = 0;
    if (denominator!=0)
    {
        resultNumber = (int)(  ((float)numerator / (float)denominator)*100   );
    }
    return to_string(resultNumber)+" %";

}

unsigned int Common::countStrings(std::string path)
{
    ClearScreen();

    unsigned int result=0;

    ifstream fin;
    fin.open(path);
    if (!fin.is_open())
    {
        throw std::string("Could not open file I should check");
    }
    std::string bufer;
    unsigned int lastCountedLines = 0;
    while (getline(fin,bufer))
    {
        if (result-lastCountedLines > 500000)
        {
            ClearScreen();
            std::cout << "Counting file lines amount:"<<std::endl;
            std::cout << "Lines amount: " << result<<std::endl;
            lastCountedLines = result;
        }

        result++;
    }

    fin.close();
    return result;
}


std::string Common::intToChunkFilename(unsigned int chunkNumber)
{
    return to_string(chunkNumber)+".huge_sort.chunk";
}

#if defined(_WIN32)
//Рекурсивное удаление каталога
BOOL Common::DeleteFolderWindows(LPCWSTR szPath)
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError,dwAttrs;
    BOOL bRes;
    int nLength;
    wchar_t cPath[MAX_PATH],cCurrentFile[MAX_PATH];

    if (szPath == NULL)
        return FALSE;

    if (lstrcmpi(szPath + 1, TEXT(":")) == 0 || szPath[0] == '\\' || szPath[0] == '\0')
        return FALSE;

    //Объекта уже нет
    dwAttrs = GetFileAttributes(szPath);
    if (dwAttrs == INVALID_FILE_ATTRIBUTES)
        return TRUE;

    //Если объект не директория
    if (~dwAttrs & FILE_ATTRIBUTE_DIRECTORY)
            DeleteFile (szPath);

    SetLastError(0);

    //Пробуем сразу удалить директорию
    bRes = RemoveDirectory(szPath);
    if (bRes == TRUE)
        return TRUE;

    if (bRes == FALSE  && GetLastError() != ERROR_DIR_NOT_EMPTY)
        return FALSE;

    nLength = lstrlen(szPath);

    if (nLength + lstrlen(L"\\*.*") + 1> MAX_PATH)
        return FALSE;

    if (szPath[nLength - 1] == '\\')
        wsprintf(cPath,L"%s*.*",szPath);
    else
        wsprintf(cPath,L"%s\\*.*",szPath);

    hFind = FindFirstFile(cPath, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE)
        return FALSE;

    lstrcpy(cPath,szPath);

    if (cPath[nLength - 1] == '\\')
        cPath[nLength - 1] = TEXT('\0');

    do
    {
        //Опускаем начальные . и ..
        if (lstrcmpi(FindFileData.cFileName, L".") == 0 || lstrcmpi(FindFileData.cFileName, L"..") == 0)
            continue;

        //Пропускаем слишком длинные имена файлов
        if (lstrlen(cPath) + lstrlen(L"") +  lstrlen(FindFileData.cFileName) + 1 > MAX_PATH)
            continue;

        wsprintf(cCurrentFile,L"%s\\%s",cPath,FindFileData.cFileName);
        //Удаляем папку
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            //Сбрасываем аттрибуты
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
            {
                FindFileData.dwFileAttributes &= ~FILE_ATTRIBUTE_READONLY;
                SetFileAttributes(cCurrentFile,FindFileData.dwFileAttributes);
            }

            bRes = DeleteFolderWindows(cCurrentFile);
        }
        //Иначе удаляем файл
        else
        {

            if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) ||
                (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
                SetFileAttributes (cCurrentFile, FILE_ATTRIBUTE_NORMAL);

            DeleteFile (cCurrentFile);
        }
    }
    while (FindNextFile(hFind,&FindFileData));

    dwError = GetLastError();

    if (hFind != INVALID_HANDLE_VALUE)
        FindClose(hFind);

    if (dwError != ERROR_NO_MORE_FILES)
        return FALSE;

    bRes = RemoveDirectory(szPath);

    return bRes;
}
#else
void FileSystem::DeleteFolderLinux(const char *dirname)
{
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];

    if (path == NULL)
    {
        return;
    }

    if (!isDirectory(dirname))
    {
        unlink(dirname);
    }

    dir = opendir(dirname);
    if (dir == NULL)
    {
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
        {
            snprintf(path, (size_t) PATH_MAX, "%s/%s", dirname, entry->d_name);
            if (entry->d_type == DT_DIR)
            {
                DeleteFolderLinux(path);
                rmdir(path);
            }
            else
            {
                // Удаляем файл
                unlink(path);
            }
        }

    }
    closedir(dir);
    rmdir(dirname);
}
#endif


void Common::Delete(std::string path)
{
#if defined (_WIN32)
    std::wstring stemp = std::wstring(path.begin(),path.end());
    LPCWSTR pathW = stemp.c_str();
    Common::DeleteFolderWindows(pathW);
#else
    Common::DeleteFolderLinux(path.c_str());
#endif
}

bool Common::fileExists(std::string path)
{
    ifstream file(path);
    if (!file)
    {
        return false;
    }
    return true;
}
