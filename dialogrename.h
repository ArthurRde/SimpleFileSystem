#ifndef DIALOGRENAME_H
#define DIALOGRENAME_H

#include <QDialog>
#include "string"

using namespace std;
namespace Ui {
    class DialogRename;
}

class DialogRename : public QDialog {
    Q_OBJECT

public:
    explicit DialogRename(string name, QWidget *parent = nullptr);

    ~DialogRename();

    string getName();

private:
    Ui::DialogRename *ui;
};

#endif // DIALOGRENAME_H
