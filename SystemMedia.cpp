#include "SystemMedia.h"


bool SystemMedia::m_initialized = false;
TCHAR SystemMedia::m_defaultSoundPath[MAX_PATH];

bool SystemMedia::initializeDefaultSoundPath() {
    TCHAR windir[MAX_PATH];

    if (GetWindowsDirectory(windir, MAX_PATH) == 0)
        return false;

    if (PathCombine(m_defaultSoundPath, windir, TEXT("Media\\Windows Notify System Generic.wav")) == NULL)
        return false;

    return true;
}

const TCHAR* SystemMedia::getDefaultSound()
{
    if (!m_initialized) {
        if (!initializeDefaultSoundPath()) {
            throw std::runtime_error("SystemMedia: Error initializing default sound path.");
        }
        m_initialized = true;
    }

    return m_defaultSoundPath;
}

const std::string SystemMedia::getDefaultSoundAsStdString()
{
    if (!m_initialized) {
        if (!initializeDefaultSoundPath()) {
            throw std::runtime_error("SystemMedia: Error initializing default sound path.");
        }
        m_initialized = true;
    }

    std::string str;

    #ifndef UNICODE
        str = m_defaultSoundPath;
    #else
        using convert_type = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_type, wchar_t> converter;

        str = converter.to_bytes(m_defaultSoundPath);
    #endif

    return str;
}
