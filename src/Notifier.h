#ifndef NOTIFIER_H
#define NOTIFIER_H

#include "nlohmann/json.hpp"
#include "wintoastlib.h"
#include "SystemMedia.h"
#include "Utils.h"

#include <QObject>
#include <QMessageBox>
#include <QApplication>
#include <regex>
#include <filesystem>

using namespace WinToastLib;
using json = nlohmann::json;


class Notifier : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString regexStr READ regexStr WRITE setRegexStr NOTIFY regexStrChanged FINAL)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
    Q_PROPERTY(QString desc READ desc WRITE setDesc NOTIFY descChanged FINAL)
    Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath NOTIFY imagePathChanged FINAL)
    Q_PROPERTY(QString soundPath READ soundPath WRITE setSoundPath NOTIFY soundPathChanged FINAL)
    Q_PROPERTY(QString duration READ duration WRITE setDuration NOTIFY durationChanged FINAL)
    Q_PROPERTY(bool toastEnabled READ toastEnabled WRITE setToastEnabled NOTIFY toastEnabledChanged FINAL)
    Q_PROPERTY(bool soundEnabled READ soundEnabled WRITE setSoundEnabled NOTIFY soundEnabledChanged FINAL)
    Q_PROPERTY(bool sticky READ sticky WRITE setSticky NOTIFY stickyChanged FINAL)
    Q_PROPERTY(bool regexError READ regexError WRITE setRegexError NOTIFY regexErrorChanged FINAL)
    Q_PROPERTY(bool soundFileError READ soundFileError WRITE setSoundFileError NOTIFY soundFileErrorChanged FINAL)
    Q_PROPERTY(bool imageFileError READ imageFileError WRITE setImageFileError NOTIFY imageFileErrorChanged FINAL)

public:
    static const QString& getDefaultTitle();
    static const QString& getDefaultDesc();
    static const QString& getDefaultImg();

    std::regex regex;
    WinToastTemplate templ;

    explicit Notifier(QObject *parent,
                      QString name = "Add notification",
                      QString regexStr = "",
                      QString title = Notifier::getDefaultTitle(),
                      QString desc = Notifier::getDefaultDesc(),
                      QString imagePath = Notifier::getDefaultImg(),
                      QString soundPath = QString::fromStdString(SystemMedia::getDefaultSound()),
                      QString duration = "System",
                      bool toastEnabled = false,
                      bool soundEnabled = false,
                      bool sticky = false);

    QString title() const;
    void setTitle(const QString &newTitle);

    QString desc() const;
    void setDesc(const QString &newDesc);

    QString imagePath() const;
    void setImagePath(const QString &newimagePath);

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

    bool sticky() const;
    void setSticky(bool newSticky);

    QString soundPath() const;
    void setSoundPath(const QString &newSoundPath);

    bool regexError() const;
    void setRegexError(bool newRegexError);

    bool soundFileError() const;
    void setSoundFileError(bool newSoundFileError);

    bool imageFileError() const;
    void setImageFileError(bool newImageFileError);

    json toJSON() const;

public slots:
    void reset();

signals:
    void titleChanged();
    void descChanged();
    void imagePathChanged();
    void soundPathChanged();
    void durationChanged();
    void regexStrChanged();
    void nameChanged();
    void toastEnabledChanged();
    void soundEnabledChanged();
    void stickyChanged();
    void regexErrorChanged();
    void soundFileErrorChanged();
    void imageFileErrorChanged();

    void disabled(Notifier* notifier);
    void enabled(Notifier* notifier);

private:
    static QString defaultTitle;
    static QString defaultDesc;
    static QString defaultImg;

    static void initializeConstants();

    QString m_name;
    QString m_title;
    QString m_desc;
    QString m_imagePath;
    QString m_soundPath;
    QString m_regexStr;
    QString m_duration;
    bool m_soundEnabled;
    bool m_toastEnabled;
    bool m_sticky;
    bool m_regexError = false;
    bool m_soundFileError = false;
    bool m_imageFileError = false;
    bool m_initialized = false;

    WinToastTemplate::Duration toWinToastDuration(const QString& duration);
    WinToastTemplate::AudioOption mapAudioOption(bool soundEnabled) const;
    void updateSticky();
};

#endif // NOTIFIER_H
