#include "dialogshowfile.h"
#include "QtCore/qdatetime.h"
#include "ui_dialogshowfile.h"

DialogShowFile::DialogShowFile(string name, string author, QDateTime date, int size, string data, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogShowFile)
{
    ui->setupUi(this);
    ui->label_name->setText(QString::fromStdString(name));
    ui->label_author->setText(QString::fromStdString(author));
    if(size > 1000){
        int sizeKb = size/1000;
    ui->label_size->setText(QString::fromStdString((to_string(sizeKb) + " KB")));
    } else {
        ui->label_size->setText(QString::fromStdString((to_string(size) + " Byte")));
    }
    ui->label_date->setText(date.toString("dd.MM.yyyy HH:mm:ss"));
    ui->textEdit_data->setText(QString::fromStdString(data));
}


string DialogShowFile::getDataText() {
    return ui->textEdit_data->toPlainText().toStdString();
}
DialogShowFile::~DialogShowFile()
{
    delete ui;
}
