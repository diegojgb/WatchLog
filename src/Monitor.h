#ifndef MONITOR_H
#define MONITOR_H

#include "nlohmann/json.hpp"
#include "Notifier.h"
#include "Utils.h"

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

public slots:
    int nListLength() const;
    Notifier* nListAt(int i) const;

    void notifierDisabled(Notifier* notifier);
    void notifierEnabled(Notifier* notifier);

signals:
    void nameChanged();
    void filePathChanged();
    void filePathChangedOverload(const QString& oldFilePath, const QString& newFilePath);
    void enabledChanged();
    void monitorEnabled(const Monitor* monitor);
    void monitorDisabled(const Monitor* monitor);

private:
    QString m_name;
    QList<Notifier*> m_notifiers;
    QString m_filePath;
    bool m_enabled;

    void readNotifiers(const json &data);
};

#endif // MONITOR_H
