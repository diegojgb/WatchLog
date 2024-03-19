#include "SystemMedia.h"


bool SystemMedia::m_initialized = false;
std::string SystemMedia::m_defaultSoundPath = "";

bool SystemMedia::initializeDefaultSoundPath()
{
    TCHAR windir[MAX_PATH];
    TCHAR tcharSoundPath[MAX_PATH];

    if (GetWindowsDirectory(windir, MAX_PATH) == 0)
        return false;

    if (PathCombine(tcharSoundPath, windir, TEXT("Media\\Windows Notify System Generic.wav")) == NULL)
        return false;

    #ifndef UNICODE
        m_defaultSoundPath = tcharSoundPath;
    #else
        using convert_type = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_type, wchar_t> converter;

        m_defaultSoundPath = converter.to_bytes(tcharSoundPath);
    #endif

    replaceAll(m_defaultSoundPath, "\\", "/");

    return true;
}

const std::string SystemMedia::getDefaultSound()
{
    if (!m_initialized) {
        if (!initializeDefaultSoundPath()) {
            throw std::runtime_error("SystemMedia: Error initializing default sound path.");
        }
        m_initialized = true;
    }

    return m_defaultSoundPath;
}

// From StackOverflow. Deemed as the most efficient answer.
void SystemMedia::replaceAll(std::string& source, const std::string& from, const std::string& to)
{
    std::string newString;
    newString.reserve(source.length());  // avoids a few memory allocations

    std::string::size_type lastPos = 0;
    std::string::size_type findPos;

    while(std::string::npos != (findPos = source.find(from, lastPos)))
    {
        newString.append(source, lastPos, findPos - lastPos);
        newString += to;
        lastPos = findPos + from.length();
    }

    // Care for the rest after last occurrence
    newString.append(source, lastPos, source.length() - lastPos);

    source.swap(newString);
}
