#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QMessageBox>


class Utils
{
public:
    static void throwError(std::string errorStr);
    static void showInfo(std::string infoStr);
    static void showCritical(std::string errorStr);
};

#endif // UTILS_H
