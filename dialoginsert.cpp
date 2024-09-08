#include "dialoginsert.h"
#include "ui_dialoginsert.h"

DialogInsert::DialogInsert(CDSYSTEM *sys_, QWidget *parent)
        : QDialog(parent), ui(new Ui::DialogInsert) {
    ui->setupUi(this);
    sys = sys_;

    connect(ui->pushButton, &QPushButton::clicked, this, &DialogInsert::insert);
}

DialogInsert::~DialogInsert() {
    delete ui;
}

void DialogInsert::addCdToComboBox(string name) {
    ui->comboBox->addItem(QString::fromStdString(name));
}

void DialogInsert::removeCdFromComboBox(string name) {
    ui->comboBox->removeItem(ui->comboBox->findText(QString::fromStdString(name)));
}

void DialogInsert::clearCdFromComboBox() {
    ui->comboBox->clear();
}

void DialogInsert::insert() {


    for (int i = 0; i < sys->ejectedCds.size(); i++) {
        if (sys->ejectedCds[i]->portName == ui->comboBox->currentText().toStdString()) {
            ui->comboBox->removeItem(ui->comboBox->findText(ui->comboBox->currentText()));
            sys->insertedCds.push_back(sys->ejectedCds[i]);
            sys->ejectedCds.erase(sys->ejectedCds.begin() + i);
        }
    }
}

void DialogInsert::on_pushButton_clicked() {
    ui->comboBox->removeItem(ui->comboBox->findText(ui->comboBox->currentText()));

    for (int i = 0; i < sys->ejectedCds.size(); i++) {
        if (sys->ejectedCds[i]->portName == ui->comboBox->currentText().toStdString()) {
            sys->insertedCds.push_back(sys->ejectedCds[i]);
            sys->ejectedCds.erase(sys->ejectedCds.begin() + i);
        }
    }
}
