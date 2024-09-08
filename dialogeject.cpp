#include "dialogeject.h"
#include "ui_dialogeject.h"

using namespace std;
DialogEject::DialogEject( CDSYSTEM *sys_,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogEject)
{
    ui->setupUi(this);
    sys = sys_;

     connect(ui->pushButton, &QPushButton::clicked, this, &DialogEject::eject);
}

void DialogEject::addCdToComboBox(string name){
    ui->comboBox->addItem(QString::fromStdString(name));
}

void DialogEject::removeCdFromComboBox(string name){
    ui->comboBox->removeItem(ui->comboBox->findText(QString::fromStdString(name)));
}

void DialogEject::clearCdFromComboBox(){
    ui->comboBox->clear();
}
DialogEject::~DialogEject()
{
    delete ui;
}


void DialogEject::eject(){
    qDebug() << "push button";
    ui->comboBox->removeItem(ui->comboBox->findText(ui->comboBox->currentText()));

    for(int i = 0; i < sys->ejectedCds.size(); i++){
        if(sys->ejectedCds[i]->portName == ui->comboBox->currentText().toStdString()){
            sys->insertedCds.push_back(sys->ejectedCds[i]);
            sys->ejectedCds.erase(sys->ejectedCds.begin()+i);
        }
    }
}

void DialogEject::on_pushButton_clicked()
{
    ui->comboBox->removeItem(ui->comboBox->findText(ui->comboBox->currentText()));


    for(int i = 0; i < sys->insertedCds.size(); i++){
        if(sys->insertedCds[i]->portName == ui->comboBox->currentText().toStdString()){
            sys->ejectedCds.push_back(sys->insertedCds[i]);
            sys->insertedCds.erase(sys->insertedCds.begin()+i);
        }
    }


}

