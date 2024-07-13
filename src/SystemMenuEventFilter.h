#ifndef SYSTEMMENUEVENTFILTER_H
#define SYSTEMMENUEVENTFILTER_H

#include <QAbstractNativeEventFilter>
#include <QApplication>
#include <QDebug>
#include <windows.h>

#define IDM_ABOUTBOX 0x0010

class SystemMenuEventFilter: public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT

public:
    bool nativeEventFilter(const QByteArray &eventType, void* message, qintptr*) override
    {
        if (eventType == "windows_generic_MSG") {
            MSG* msg = static_cast<MSG*>(message);

            if (msg->message == WM_SYSCOMMAND && (msg->wParam & 0xfff0) == IDM_ABOUTBOX) {
                emit aboutClicked();
                return true; // Event handled
            }
        }
        return false; // Event not handled
    }

signals:
    void aboutClicked();
};

#endif // SYSTEMMENUEVENTFILTER_H
