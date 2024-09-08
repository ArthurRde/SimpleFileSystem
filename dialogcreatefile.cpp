#include "dialogcreatefile.h"
#include "QtWidgets/qpushbutton.h"
#include "ui_dialogcreatefile.h"

DialogCreateFile::DialogCreateFile(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogCreateFile)
{
    ui->setupUi(this);

    QPushButton *okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);

    ui->lineEdit_3->setEnabled(false);
    isFolder = true;

}

DialogCreateFile::~DialogCreateFile()
{
    delete ui;
}

void DialogCreateFile::on_comboBox_currentTextChanged(const QString &arg1)
{

    //qDebug() << " isFolder " << isFolder;

}


void DialogCreateFile::on_lineEdit_textChanged(const QString &arg1)
{

    //qDebug() << " has name" << hasName;
    checkIfComplete();
}


void DialogCreateFile::on_lineEdit_2_textChanged(const QString &arg1)
{

    //qDebug() << " has author" << hasAuthor;

     checkIfComplete();
}





void DialogCreateFile::on_lineEdit_3_textChanged(const QString &arg1)
{

    checkIfComplete();
}

void DialogCreateFile::checkIfComplete(){
    if(!ui->lineEdit_3->text().trimmed().isEmpty()){
        hasData = true;
    } else {
        hasData = false;
    }

    qDebug() << "has Data" << hasData;
    if(!ui->lineEdit_2->text().trimmed().isEmpty()){
        hasAuthor = true;
    } else {
        hasAuthor = false;
    }
    qDebug() << "has Aut" << hasAuthor;
    if(!ui->lineEdit->text().trimmed().isEmpty()){
        hasName = true;
    } else {
        hasName = false;
    }
    qDebug() << "has Name" << hasName;
    if(ui->comboBox->currentText() == "Folder"){
        ui->lineEdit_3->setEnabled(false);
        isFolder = true;
    } else {
        ui->lineEdit_3->setEnabled(true);
        isFolder = false;
    }

    qDebug() << "isFol" << isFolder;

    if(hasName && hasAuthor && (hasData || isFolder)){
        QPushButton *okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
        okButton->setEnabled(true);
    } else {
        QPushButton *okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
        okButton->setEnabled(false);
    }
}

Ui::DialogCreateFile *DialogCreateFile::getUi() const
{
    return ui;
}

void DialogCreateFile::setUi(Ui::DialogCreateFile *newUi)
{
    ui = newUi;
}

