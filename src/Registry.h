#ifndef REGISTRY_H
#define REGISTRY_H

#include "Utils.h"

#include <Windows.h>
#include <tchar.h>


class Registry
{
public:
    static bool createRegistryKey(HKEY hKeyParent, LPCTSTR subkey);
    static bool writeStringInRegistry(HKEY hKeyParent, LPCTSTR subkey, LPCTSTR valueName, LPCTSTR strData);
};

#endif // REGISTRY_H
