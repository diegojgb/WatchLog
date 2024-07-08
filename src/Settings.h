#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class Settings : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool startMinimized READ startMinimized WRITE setStartMinimized NOTIFY startMinimizedChanged FINAL)

public:
    explicit Settings(QObject *parent = nullptr);

    bool startMinimized() const;
    void setStartMinimized(bool newStartMinimized);

signals:
    void startMinimizedChanged();

private:
    bool m_startMinimized = false;
};

#endif // SETTINGS_H
