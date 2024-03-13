#ifndef TOASTHANDLER_H
#define TOASTHANDLER_H

#include "wintoastlib.h"

using namespace WinToastLib;


class ToastHandler: public IWinToastHandler {

public:
    virtual void toastActivated() const override {}
    virtual void toastActivated(int actionIndex) const override {}
    virtual void toastDismissed(WinToastDismissalReason state) const override {}
    virtual void toastFailed() const override {}
};

#endif // TOASTHANDLER_H
