#include "Registry.h"

bool Registry::createRegistryKey(HKEY hKeyParent, LPCTSTR subkey) {
    DWORD dwDisposition; // Verify if a new key is created or an existing key is opened
    HKEY  hKey;
    DWORD Ret;

    Ret = RegCreateKeyEx(
            hKeyParent,
            subkey,
            0,
            NULL,
            REG_OPTION_NON_VOLATILE,
            KEY_ALL_ACCESS,
            NULL,
            &hKey,
            &dwDisposition);

    if (Ret != ERROR_SUCCESS)
        return false;

    RegCloseKey(hKey); //close the key
    return true;
}

bool Registry::writeStringInRegistry(HKEY hKeyParent, LPCTSTR subkey, LPCTSTR valueName, LPCTSTR strData)
{
    DWORD Ret;
    HKEY hKey;

    Ret = RegOpenKeyEx(
              hKeyParent,
              subkey,
              0,
              KEY_WRITE,
              &hKey);

    // Check if the registry key exists
    if (Ret == ERROR_SUCCESS) {
        auto result = RegSetValueEx(
                        hKey,
                        valueName,
                        0,
                        REG_SZ,
                        (LPBYTE)strData,
                        (_tcslen(strData)+1) * sizeof(TCHAR));

        if (ERROR_SUCCESS != result) {
            RegCloseKey(hKey);
            return false;
        }

        RegCloseKey(hKey);
        return true;
    }

    return false;
}

