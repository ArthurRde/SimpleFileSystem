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
    if(arg1 == "Folder"){
        ui->lineEdit_3->setEnabled(false);
        isFolder = true;
    } else {
        ui->lineEdit_3->setEnabled(true);
        isFolder = false;
    }
    //qDebug() << " isFolder " << isFolder;
    if(hasName && hasAuthor && (hasData || isFolder)){
        QPushButton *okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
        okButton->setEnabled(true);
    } else {
        QPushButton *okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
        okButton->setEnabled(false);
    }
}


void DialogCreateFile::on_lineEdit_textChanged(const QString &arg1)
{
    if(!arg1.trimmed().isEmpty()){
    hasName = true;
    } else {
        hasName = false;
    }
    //qDebug() << " has name" << hasName;
    if(hasName && hasAuthor && (hasData || isFolder)){
        QPushButton *okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
        okButton->setEnabled(true);
    } else {
        QPushButton *okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
        okButton->setEnabled(false);
    }
}


void DialogCreateFile::on_lineEdit_2_textChanged(const QString &arg1)
{
    if(!arg1.trimmed().isEmpty()){
        hasAuthor = true;
    } else {
        hasAuthor = false;
    }

    //qDebug() << " has author" << hasAuthor;

    if(hasName && hasAuthor && (hasData || isFolder)){
        QPushButton *okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
        okButton->setEnabled(true);
    } else {
        QPushButton *okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
        okButton->setEnabled(false);
    }
}





void DialogCreateFile::on_lineEdit_3_textChanged(const QString &arg1)
{
    if(!arg1.trimmed().isEmpty()){
        hasData = true;
    } else {
        hasData = false;
    }



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

