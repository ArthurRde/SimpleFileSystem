#include "dialogwritecd.h"
#include "ui_dialogwritecd.h"

DialogWriteCd::DialogWriteCd(QWidget *parent)
        : QDialog(parent), ui(new Ui::DialogWriteCd) {
    ui->setupUi(this);
}

DialogWriteCd::~DialogWriteCd() {
    delete ui;
}
