#ifndef DIALOGEJECT_H
#define DIALOGEJECT_H

#include "cdsystem.h"
#include <QDialog>
#include <string>
using namespace std;


namespace Ui {
class DialogEject;
}

class DialogEject : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEject(CDSYSTEM *sys_,QWidget *parent = nullptr);
    ~DialogEject();
    CDSYSTEM* sys;
public slots:
    void addCdToComboBox(string name);
    void removeCdFromComboBox(string name);
    void clearCdFromComboBox();
    void eject();
private slots:
    void on_pushButton_clicked();

private:
    Ui::DialogEject *ui;
};

#endif // DIALOGEJECT_H
