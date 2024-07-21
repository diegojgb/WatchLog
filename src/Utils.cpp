#include "Utils.h"


void Utils::showCritical(QString errorStr)
{
    QMessageBox::critical(nullptr, "WatchLog", errorStr);
}

void Utils::throwError(QString errorStr)
{
    showCritical(errorStr);
    throw std::runtime_error(errorStr.toStdString());
}

void Utils::showInfo(QString infoStr)
{
    QMessageBox::information(nullptr, "WatchLog", infoStr);
}
