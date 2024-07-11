#include "Settings.h"

Settings::Settings(QObject* parent)
    : QObject{parent}
{}

bool Settings::startMinimized() const
{
    return m_startMinimized;
}

void Settings::setStartMinimized(bool newStartMinimized)
{
    if (m_startMinimized == newStartMinimized)
        return;

    m_startMinimized = newStartMinimized;

    emit startMinimizedChanged();
}
