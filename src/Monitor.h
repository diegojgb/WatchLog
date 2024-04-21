#ifndef MONITOR_H
#define MONITOR_H

#include "nlohmann/json.hpp"
#include "Notifier.h"
#include "NotifierList.h"

#include <QObject>
#include <QVarLengthArray>
#include <fstream>

using json = nlohmann::json;


class Monitor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged FINAL)
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(NotifierList* notifiers READ notifiers CONSTANT)

public:
    std::ifstream m_file;
    QVarLengthArray<Notifier*> m_enabledNotifiers;
    bool manyPerUpdate;

    static json jsonFindByKey(const json &data, const std::string &key);

    explicit Monitor(QObject *parent, const json &monitorData);

    QString name() const;
    void setName(const QString &newName);

    QString filePath() const;
    void setFilePath(const QString &newFilePath);

    bool enabled() const;
    void setEnabled(bool newEnabled);

    void startFile();
    json toJSON() const;

    template<typename T>
    T jsonGetValue(const json &data, const std::string &key);

    template<typename T>
    T jsonGetValue(const json &data, const std::string &key, const T defaultValue);

    void showTypeError(json::type_error e, const std::string &key);

    NotifierList* notifiers();

public slots:
    void notifierDisabled(Notifier* notifier);
    void notifierEnabled(Notifier* notifier);
    void addEmptyNotifier();

signals:
    void nameChanged();
    void filePathChanged();
    void filePathChangedOverload(const QString& oldFilePath, const QString& newFilePath);
    void enabledChanged();
    void monitorEnabled(const Monitor* monitor);
    void monitorDisabled(const Monitor* monitor);

private:
    QString m_name;
    NotifierList m_notifiers;
    QString m_filePath;
    bool m_enabled;
    std::string m_defaultImage;

    void readNotifiers(const json &data);
};

#endif // MONITOR_H
