#ifndef SYSTEMMEDIA_H
#define SYSTEMMEDIA_H

#include <windows.h>
#include <shlwapi.h>
#include <stdexcept>
#include <locale>
#include <codecvt>

class SystemMedia
{
public:
    static const TCHAR* getDefaultSound();
    static const std::string getDefaultSoundAsStdString();

private:
    static TCHAR m_defaultSoundPath[MAX_PATH];
    static bool initializeDefaultSoundPath();
    static bool m_initialized;
};

#endif // SYSTEMMEDIA_H
