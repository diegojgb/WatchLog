#ifndef NOTIFIER_H
#define NOTIFIER_H

#include "wintoastlib.h"

#include <QObject>
#include <regex>
#include <QMessageBox>

using namespace WinToastLib;


class Notifier : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString regexStr READ regexStr WRITE setRegexStr NOTIFY regexStrChanged FINAL)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
    Q_PROPERTY(QString desc READ desc WRITE setDesc NOTIFY descChanged FINAL)
    Q_PROPERTY(QString imagePath READ imagePath WRITE setimagePath NOTIFY imagePathChanged FINAL)
    Q_PROPERTY(QString duration READ duration WRITE setDuration NOTIFY durationChanged FINAL)
    Q_PROPERTY(bool toastEnabled READ toastEnabled WRITE setToastEnabled NOTIFY toastEnabledChanged FINAL)
    Q_PROPERTY(bool soundEnabled READ soundEnabled WRITE setSoundEnabled NOTIFY soundEnabledChanged FINAL)

public:
    std::regex regex;
    WinToastTemplate templ;

    explicit Notifier(QObject *parent, QString name, QString regexStr, QString title,
                      QString desc, QString imagePath, QString duration, bool toastEnabled, bool soundEnabled);

    QString title() const;
    void setTitle(const QString &newTitle);

    QString desc() const;
    void setDesc(const QString &newDesc);

    QString imagePath() const;
    void setimagePath(const QString &newimagePath);

    QString duration() const;
    void setDuration(QString newDuration);

    QString regexStr() const;
    void setRegexStr(const QString &newRegexStr);

    QString name() const;
    void setName(const QString &newName);

    bool toastEnabled() const;
    void setToastEnabled(bool newToastEnabled);

    bool soundEnabled() const;
    void setSoundEnabled(bool newSoundEnabled);

signals:
    void titleChanged();
    void descChanged();
    void imagePathChanged();
    void durationChanged();
    void regexStrChanged();
    void nameChanged();
    void toastEnabledChanged();
    void soundEnabledChanged();
    void disabled(Notifier* notifier);
    void enabled(Notifier* notifier);

private:
    QString m_name;
    QString m_title;
    QString m_desc;
    QString m_imagePath;
    QString m_regexStr;
    QString m_duration;
    bool m_soundEnabled;
    bool m_toastEnabled;

    WinToastTemplate::Duration toWinToastDuration(const QString& duration);
    WinToastTemplate::AudioOption mapAudioOption(bool soundEnabled) const;
};

#endif // NOTIFIER_H