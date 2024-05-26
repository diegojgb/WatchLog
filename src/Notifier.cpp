#include "Notifier.h"
#include "Monitor.h"


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

Notifier::Notifier(QObject *parent, WinFileManager& winFileManager, QString name, QString regexStr,
                   QString title, QString desc, QString imagePath, QString soundPath, QString duration,
                   bool toastEnabled, bool soundEnabled, bool sticky)
    : QObject{parent},
      m_monitor{qobject_cast<Monitor*>(parent)},
      m_winFileManager{winFileManager},
      m_templ{WinToastTemplate(WinToastTemplate::ImageAndText02)}
{
    setName(name);
    setRegexStr(regexStr);
    setToastEnabled(toastEnabled);
    setSoundEnabled(soundEnabled);
    setDuration(duration);
    setSoundPath(soundPath);
    setImagePath(imagePath);
    setTitle(title);
    setDesc(desc);
    setSticky(sticky);

    m_initialized = true;
}

const std::regex &Notifier::getRegex() const
{
    return m_regex;
}

const WinToastTemplate &Notifier::getTempl() const
{
    return m_templ;
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
    if (m_soundPath != m_monitor->defaultSound())
        obj["soundFile"] = m_soundPath.toStdString();
    if (m_imagePath != m_monitor->defaultImage())
        obj["image"] = m_imagePath.toStdString();
    if (m_duration != "System")
        obj["duration"] = m_duration.toStdString();
    obj["toast"] = m_toastEnabled;
    obj["sound"] = m_soundEnabled;
    obj["sticky"] = m_sticky;

    return obj;
}

void Notifier::reset()
{
    setName("Add notification");
    setRegexStr("");
    setTitle(Notifier::getDefaultTitle());
    setDesc(Notifier::getDefaultDesc());
    setImagePath(Notifier::getDefaultImg());
    setSoundPath(QString::fromStdString(SystemMedia::getDefaultSound()));
    setDuration("System");
    setToastEnabled(false);
    setSoundEnabled(false);
    setSticky(false);
}

void Notifier::updateSticky() {
    if (m_sticky) {
        m_templ.addAction(L"Dismiss");
        m_templ.setScenario(WinToastTemplate::Scenario::Reminder);
    } else {
        m_templ.removeAction(L"Dismiss");
        m_templ.setScenario(WinToastTemplate::Scenario::Default);
    }
}

WinToastTemplate::Duration Notifier::toWinToastDuration(const QString& duration) {
    if (duration == "Short")
        return WinToastTemplate::Duration::Short;
    if (duration == "Long")
        return WinToastTemplate::Duration::Long;
    if (duration == "System")
        return WinToastTemplate::Duration::System;

    Utils::throwError("Invalid toast duration value.");
    return WinToastTemplate::Duration::System; // To prevent compiler warnings.
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
    if (m_initialized && m_title == newTitle)
        return;

    m_title = newTitle;

    QString titleCopy = newTitle;
    m_templ.setTextField(titleCopy
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
    if (m_initialized && m_desc == newDesc)
        return;

    m_desc = newDesc;

    QString descCopy = newDesc;
    m_templ.setTextField(descCopy
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
    if (m_initialized && m_imagePath == newImagePath)
        return;

    if (m_initialized) {
        auto* oldFileStatus = m_winFileManager.find(m_imagePath);

        if (oldFileStatus == nullptr)
            Utils::showCritical("Error: the following file path wasn't being monitored: " + m_imagePath.toStdString());
        else
            QObject::disconnect(oldFileStatus, &FileStatus::statusChanged, this, &Notifier::setImageFileError);
    }

    auto imageExtension = std::filesystem::path(newImagePath.toStdString()).extension();

    if (imageExtension != ".jpg" && imageExtension != ".jpeg" && imageExtension != ".png"
            || !std::filesystem::exists(newImagePath.toStdString())) {
        setImageFileError(true);
    } else {
        setImageFileError(false);

        auto* newFileStatus = m_winFileManager.findOrCreate(newImagePath);
        QObject::connect(newFileStatus, &FileStatus::statusChanged, this, &Notifier::setImageFileError);
    }

    m_imagePath = newImagePath;
    m_templ.setImagePath(newImagePath.toStdWString());

    emit imagePathChanged();
}

QString Notifier::duration() const
{
    return m_duration;
}

void Notifier::setDuration(QString newDuration)
{
    if (m_initialized && m_duration == newDuration)
        return;

    if (newDuration != "System" && newDuration != "Short" && newDuration != "Long")
        Utils::throwError("Invalid duration value: must be either \"System\", \"Short\" or \"Long\"");

    m_duration = newDuration;
    m_templ.setDuration(toWinToastDuration(newDuration));

    emit durationChanged();
}

QString Notifier::regexStr() const
{
    return m_regexStr;
}

void Notifier::setRegexStr(const QString &newRegexStr)
{
    if (m_initialized && m_regexStr == newRegexStr)
        return;

    m_regexStr = newRegexStr;

    if (m_title.contains("${regex}")) {
        QString titleCopy = m_title;
        titleCopy.replace("${regex}", newRegexStr);
        m_templ.setTextField(titleCopy.toStdWString(), WinToastTemplate::FirstLine);
    }

    if (m_desc.contains("${regex}")) {
        QString descCopy = m_desc;
        descCopy.replace("${regex}", newRegexStr);
        m_templ.setTextField(descCopy.toStdWString(), WinToastTemplate::SecondLine);
    }

    if (newRegexStr == "") {
        setRegexError(true);
    } else {
        try {
            m_regex = std::regex(newRegexStr.toStdString());
            setRegexError(false);
        } catch (const std::exception& e) {
            std::cerr << "Invalid regex: " << e.what() << std::endl;
            setRegexError(true);
        }
    }

    emit regexStrChanged();
}

QString Notifier::name() const
{
    return m_name;
}

void Notifier::setName(const QString &newName)
{
    if (m_initialized && m_name == newName)
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

    if (newToastEnabled) {
        if (m_regexStr == "") {
            Utils::showInfo("Can't enable a Notification with an empty regex pattern.");
            return;
        }

        if (m_regexError || m_soundFileError || m_imageFileError)
            return;
    }

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

    if (newSoundEnabled) {
        if (m_regexStr == "") {
            Utils::showInfo("Can't enable a Notification with an empty regex pattern.");
            return;
        }

        if (m_regexError || m_soundFileError || m_imageFileError)
            return;
    }

    if (!m_soundEnabled && !m_toastEnabled)
    {
        emit enabled(this);
    }
    else if (!newSoundEnabled && !m_toastEnabled)
    {
        emit disabled(this);
    }

    m_soundEnabled = newSoundEnabled;
    m_templ.setAudioOption(mapAudioOption(newSoundEnabled));

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
    if (m_initialized && m_soundPath == newSoundPath)
        return;

    if (m_initialized) {
        auto* oldFileStatus = m_winFileManager.find(m_soundPath);

        if (oldFileStatus == nullptr)
            Utils::showCritical("Error: the following file path wasn't being monitored: " + m_soundPath.toStdString());
        else
            QObject::disconnect(oldFileStatus, &FileStatus::statusChanged, this, &Notifier::setSoundFileError);
    }

    if (std::filesystem::path(newSoundPath.toStdString()).extension() != ".wav"
            || !std::filesystem::exists(newSoundPath.toStdString())) {
        setSoundFileError(true);
    } else {
        setSoundFileError(false);

        auto* newFileStatus = m_winFileManager.findOrCreate(newSoundPath);
        QObject::connect(newFileStatus, &FileStatus::statusChanged, this, &Notifier::setSoundFileError);
    }

    m_soundPath = newSoundPath;

    emit soundPathChanged();
}

bool Notifier::regexError() const
{
    return m_regexError;
}

void Notifier::setRegexError(bool newRegexError)
{
    if (m_regexError == newRegexError)
        return;

    m_regexError = newRegexError;

    emit regexErrorChanged();
}

bool Notifier::soundFileError() const
{
    return m_soundFileError;
}

void Notifier::setSoundFileError(bool newSoundFileError)
{
    if (m_soundFileError == newSoundFileError)
        return;

    m_soundFileError = newSoundFileError;

    emit soundFileErrorChanged();
}

bool Notifier::imageFileError() const
{
    return m_imageFileError;
}

void Notifier::setImageFileError(bool newImageFileError)
{
    if (m_imageFileError == newImageFileError)
        return;

    m_imageFileError = newImageFileError;

    emit imageFileErrorChanged();
}
