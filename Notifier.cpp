#include "Notifier.h"


QString Notifier::defaultTitle;
QString Notifier::defaultDesc;
QString Notifier::defaultImg;

const QString& Notifier::getDefaultTitle() {
    initializeConstants();
    return defaultTitle;
}

const QString& Notifier::getDefaultDesc() {
    initializeConstants();
    return defaultDesc;
}

const QString& Notifier::getDefaultImg() {
    initializeConstants();
    return defaultImg;
}

void Notifier::initializeConstants() {
    if (defaultTitle.isEmpty() || defaultDesc.isEmpty() || defaultImg.isEmpty()) {
        defaultTitle = "Match found in ${name}";
        defaultDesc = "For regex: ${regex}";
        defaultImg = QCoreApplication::applicationDirPath() + "/assets/information.png";
    }
}

Notifier::Notifier(QObject *parent, QString name, QString regexStr, QString title,
                   QString desc, QString imagePath, QString soundPath, QString duration, bool toastEnabled, bool soundEnabled, bool sticky)
    : QObject{parent}, m_name{name}, m_regexStr{regexStr}, m_title{title},
      m_desc{desc}, m_imagePath{imagePath}, m_soundPath{soundPath}, m_duration{duration}, m_toastEnabled{toastEnabled},
      m_soundEnabled{soundEnabled}, m_sticky{sticky}, templ{WinToastTemplate(WinToastTemplate::ImageAndText02)}
{
    if (duration != "System" && duration != "Short" && duration != "Long") {
        Utils::throwError("Invalid duration value: must be either \"System\", \"Short\" or \"Long\"");
    }
    if (std::filesystem::path(soundPath.toStdString()).extension() != ".wav") {
        Utils::throwError("Invalid soundFile type (must be .wav): " + soundPath.toStdString());
    }
    if (!std::filesystem::exists(soundPath.toStdString())) {
        Utils::throwError("Specified soundFile doesn't exist: " + soundPath.toStdString());
    }
    auto imageExtension = std::filesystem::path(imagePath.toStdString()).extension();
    if (imageExtension != ".jpg" && imageExtension != ".jpeg" && imageExtension != ".png") {
        Utils::throwError("Invalid image file type (must be .jpg/jpeg/png): " + imagePath.toStdString());
    }
    if (!std::filesystem::exists(imagePath.toStdString())) {
        Utils::throwError("Specified image file doesn't exist: " + imagePath.toStdString());
    }

    regex = std::regex(regexStr.toStdString());

    templ.setImagePath(imagePath.toStdWString());
    templ.setTextField(title
                       .replace("${regex}", regexStr)
                       .replace("${name}", name)
                       .toStdWString(), WinToastTemplate::FirstLine);
    templ.setTextField(desc
                       .replace("${regex}", regexStr)
                       .replace("${name}", name)
                       .toStdWString(), WinToastTemplate::SecondLine);
    templ.setDuration(toWinToastDuration(m_duration));
    templ.setAudioOption(mapAudioOption(soundEnabled));
    updateSticky();
}

json Notifier::toJSON() const
{
    json obj;

    obj["name"] = m_name.toStdString();
    obj["pattern"] = m_regexStr.toStdString();
    if (m_title != defaultTitle)
        obj["title"] = m_title.toStdString();
    if (m_desc != defaultDesc)
        obj["desc"] = m_desc.toStdString();
    if (m_soundPath.toStdString() != SystemMedia::getDefaultSound())
        obj["soundFile"] = m_soundPath.toStdString();
    if (m_imagePath != defaultImg)
        obj["image"] = m_imagePath.toStdString();
    if (m_duration != "System")
        obj["duration"] = m_duration.toStdString();
    obj["toast"] = m_toastEnabled;
    obj["sound"] = m_soundEnabled;
    obj["sticky"] = m_sticky;

    return obj;
}

void Notifier::updateSticky() {
    if (m_sticky)
    {
        templ.addAction(L"Dismiss");
        templ.setScenario(WinToastTemplate::Scenario::Reminder);
    }
    else
    {
        templ.removeAction(L"Dismiss");
        templ.setScenario(WinToastTemplate::Scenario::Default);
    }
}

WinToastTemplate::Duration Notifier::toWinToastDuration(const QString& duration) {
    if (duration == "Short")
        return WinToastTemplate::Duration::Short;
    else if (duration == "Long")
        return WinToastTemplate::Duration::Long;
    else if (duration == "System")
        return WinToastTemplate::Duration::System;
    else {
        Utils::throwError("Invalid toast duration value.");
        return WinToastTemplate::Duration::System; // Default return.
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

    QString titleCopy = newTitle;
    templ.setTextField(titleCopy
                       .replace("${regex}", m_regexStr)
                       .replace("${name}", m_name)
                       .toStdWString(), WinToastTemplate::FirstLine);

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

    QString descCopy = newDesc;
    templ.setTextField(descCopy
                       .replace("${regex}", m_regexStr)
                       .replace("${name}", m_name)
                       .toStdWString(), WinToastTemplate::SecondLine);

    emit descChanged();
}

QString Notifier::imagePath() const
{
    return m_imagePath;
}

void Notifier::setImagePath(const QString &newImagePath)
{
    if (m_imagePath == newImagePath)
        return;

    m_imagePath = newImagePath;
    templ.setImagePath(newImagePath.toStdWString());

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
    templ.setDuration(toWinToastDuration(newDuration));

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

    if (m_title.contains("${regex}")) {
        QString titleCopy = m_title;
        titleCopy.replace("${regex}", newRegexStr);
        templ.setTextField(titleCopy.toStdWString(), WinToastTemplate::FirstLine);
    }

    if (m_desc.contains("${regex}")) {
        QString descCopy = m_desc;
        descCopy.replace("${regex}", newRegexStr);
        templ.setTextField(descCopy.toStdWString(), WinToastTemplate::SecondLine);
    }

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

bool Notifier::sticky() const
{
    return m_sticky;
}

void Notifier::setSticky(bool newSticky)
{
    if (m_sticky == newSticky)
        return;

    m_sticky = newSticky;
    updateSticky();

    emit stickyChanged();
}

QString Notifier::soundPath() const
{
    return m_soundPath;
}

void Notifier::setSoundPath(const QString &newSoundPath)
{
    if (m_soundPath == newSoundPath)
        return;

    m_soundPath = newSoundPath;

    emit soundPathChanged();
}
