#ifndef DIALOGINSERT_H
#define DIALOGINSERT_H

#include <QDialog>

#include "cdsystem.h"
#include "string"

using namespace std;
namespace Ui {
    class DialogInsert;
}

class DialogInsert : public QDialog {
    Q_OBJECT

public:

    CDSYSTEM *sys;

    explicit DialogInsert(CDSYSTEM *sys_, QWidget *parent = nullptr);

    ~DialogInsert();

    void clearCdFromComboBox();

    void removeCdFromComboBox(string name);

    void addCdToComboBox(string name);

    void on_pushButton_clicked();

public
    slots:
            void insert();
private:
    Ui::DialogInsert *ui;
};

#endif // DIALOGINSERT_H
