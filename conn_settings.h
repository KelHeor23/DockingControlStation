#ifndef CONN_SETTINGS_H
#define CONN_SETTINGS_H

#include <QWidget>

namespace Ui {
class conn_settings;
}

class conn_settings : public QWidget
{
    Q_OBJECT

public:
    explicit conn_settings(QWidget *parent = nullptr);
    ~conn_settings();

private:
    Ui::conn_settings *ui;
};

#endif // CONN_SETTINGS_H
