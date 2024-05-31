#ifndef EXITDIALOG_H
#define EXITDIALOG_H

#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyle>


class ExitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExitDialog(QWidget *parent = nullptr);

signals:
    void noClicked();
};

#endif // EXITDIALOG_H
