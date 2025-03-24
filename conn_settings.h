#ifndef CONN_SETTINGS_H
#define CONN_SETTINGS_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class conn_settings;
}

class conn_settings : public QWidget
{
    Q_OBJECT

public:
    explicit conn_settings(QWidget *parent = nullptr);
    ~conn_settings();

    QString getDrone1IP() const;

    quint16 getDrone1Port() const;

    QString getDrone2IP() const;

    quint16 getDrone2Port() const;

private:
    bool isValidIPAddress(const QString &ip);
    bool isValidIPPort(const QString &port);
    void setDefaultBorders();

private slots:
    void setConnValue();
    void loadSettings();
    void saveSettings();

signals:
    void newConnSettings();

private:
    Ui::conn_settings *ui;

    QString drone1IP = "localhost";
    quint16 drone1Port = 8021;
    QString drone2IP = "localhost";
    quint16 drone2Port = 8022;

    QSettings *conSettings;
};

#endif // CONN_SETTINGS_H
