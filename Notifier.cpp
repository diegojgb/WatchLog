#include "Notifier.h"


Notifier::Notifier(QObject *parent, QString name, QString regexStr, QString title,
                   QString desc, QString imagePath, QString duration, bool toastEnabled, bool soundEnabled)
    : QObject{parent}, m_name{name}, m_regexStr{regexStr}, m_title{title},
      m_desc{desc}, m_imagePath{imagePath}, m_duration{duration}, m_toastEnabled{toastEnabled},
      m_soundEnabled{soundEnabled}, templ{WinToastTemplate(WinToastTemplate::ImageAndText02)}
{
    regex = std::regex(regexStr.toStdString());

    templ.setImagePath(imagePath.toStdWString());
    templ.setTextField(title.toStdWString(), WinToastTemplate::FirstLine);
    templ.setTextField(desc.toStdWString(), WinToastTemplate::SecondLine);
    templ.setDuration(toWinToastDuration(m_duration));
    templ.setAudioOption(mapAudioOption(soundEnabled));
}

WinToastTemplate::Duration Notifier::toWinToastDuration(const QString& duration) {
    if (duration == "Short")
        return WinToastTemplate::Duration::Short;
    else if (duration == "Long")
        return WinToastTemplate::Duration::Long;
    else if (duration == "System")
        return WinToastTemplate::Duration::System;
    else {
        QMessageBox::critical(nullptr, tr("WatchLog"), tr("Invalid toast duration value."));
    }
}

WinToastTemplate::AudioOption Notifier::mapAudioOption(bool soundEnabled) const
{
    return (soundEnabled) ? WinToastTemplate::AudioOption::Default : WinToastTemplate::AudioOption::Silent;
}

QString Notifier::title() const
{
    return m_title;
}

void Notifier::setTitle(const QString &newTitle)
{
    if (m_title == newTitle)
        return;

    m_title = newTitle;

    emit titleChanged();
}

QString Notifier::desc() const
{
    return m_desc;
}

void Notifier::setDesc(const QString &newDesc)
{
    if (m_desc == newDesc)
        return;

    m_desc = newDesc;

    emit descChanged();
}

QString Notifier::imagePath() const
{
    return m_imagePath;
}

void Notifier::setimagePath(const QString &newimagePath)
{
    if (m_imagePath == newimagePath)
        return;

    m_imagePath = newimagePath;

    emit imagePathChanged();
}

QString Notifier::duration() const
{
    return m_duration;
}

void Notifier::setDuration(QString newDuration)
{
    if (m_duration == newDuration)
        return;

    m_duration = newDuration;

    emit durationChanged();
}

QString Notifier::regexStr() const
{
    return m_regexStr;
}

void Notifier::setRegexStr(const QString &newRegexStr)
{
    if (m_regexStr == newRegexStr)
        return;

    m_regexStr = newRegexStr;

    emit regexStrChanged();
}

QString Notifier::name() const
{
    return m_name;
}

void Notifier::setName(const QString &newName)
{
    if (m_name == newName)
        return;

    m_name = newName;

    emit nameChanged();
}

bool Notifier::toastEnabled() const
{
    return m_toastEnabled;
}

void Notifier::setToastEnabled(bool newToastEnabled)
{
    if (m_toastEnabled == newToastEnabled)
        return;

    if (!m_soundEnabled && !m_toastEnabled)
    {
        emit enabled(this);
    }
    else if (!newToastEnabled && !m_soundEnabled)
    {
        emit disabled(this);
    }

    m_toastEnabled = newToastEnabled;

    emit toastEnabledChanged();
}

bool Notifier::soundEnabled() const
{
    return m_soundEnabled;
}

void Notifier::setSoundEnabled(bool newSoundEnabled)
{
    if (m_soundEnabled == newSoundEnabled)
        return;

    if (!m_soundEnabled && !m_toastEnabled)
    {
        emit enabled(this);
    }
    else if (!newSoundEnabled && !m_toastEnabled)
    {
        emit disabled(this);
    }

    m_soundEnabled = newSoundEnabled;
    templ.setAudioOption(mapAudioOption(newSoundEnabled));

    emit soundEnabledChanged();
}