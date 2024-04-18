#include "Utils.h"


void Utils::throwError(std::string errorStr)
{
    QMessageBox::critical(nullptr, QObject::tr("WatchLog"), QObject::tr(errorStr.c_str()));
    throw std::runtime_error(errorStr);
}

void Utils::showInfo(std::string infoStr)
{
    QMessageBox::information(nullptr, QObject::tr("WatchLog"), QObject::tr(infoStr.c_str()));
}
