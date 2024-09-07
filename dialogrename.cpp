#include "dialogrename.h"
#include "ui_dialogrename.h"


DialogRename::DialogRename(string name, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogRename)
{
    ui->setupUi(this);
    ui->lineEdit->setText(QString::fromStdString(name));
}

DialogRename::~DialogRename()
{
    delete ui;
}

string DialogRename::getName(){
    return ui->lineEdit->text().toStdString();
}
