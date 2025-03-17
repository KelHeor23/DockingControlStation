#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "conn_settings.h"
#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void openConnectionSettings();

private:
    Ui::MainWindow *ui;
    conn_settings *conn_set;

    QLabel *drone1_ConnectionStatus;
    QLabel *drone2_ConnectionStatus;
};
#endif // MAINWINDOW_H
