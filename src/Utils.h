#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QMessageBox>


class Utils
{
public:
    static void throwError(QString errorStr);
    static void showInfo(QString infoStr);
    static void showCritical(QString errorStr);
};

#endif // UTILS_H
