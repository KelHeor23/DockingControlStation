#include "conn_settings.h"
#include "ui_conn_settings.h"

conn_settings::conn_settings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::conn_settings)
{
    ui->setupUi(this);
}

conn_settings::~conn_settings()
{
    delete ui;
}
