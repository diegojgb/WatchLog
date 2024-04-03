#ifndef SYSTEMMEDIA_H
#define SYSTEMMEDIA_H

#include "Utils.h"

#include <windows.h>
#include <shlwapi.h>
#include <stdexcept>
#include <locale>
#include <codecvt>


class SystemMedia
{
public:
    static const std::string getDefaultSound();

private:
    static std::string m_defaultSoundPath;
    static bool m_initialized;

    static bool initializeDefaultSoundPath();
    static void replaceAll(std::string& source, const std::string& from, const std::string& to);
};

#endif // SYSTEMMEDIA_H
