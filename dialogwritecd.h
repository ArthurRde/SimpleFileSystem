#ifndef DIALOGWRITECD_H
#define DIALOGWRITECD_H

#include <QDialog>

namespace Ui {
    class DialogWriteCd;
}

class DialogWriteCd : public QDialog {
    Q_OBJECT

public:
    explicit DialogWriteCd(QWidget *parent = nullptr);

    ~DialogWriteCd();

private:
    Ui::DialogWriteCd *ui;
};

#endif // DIALOGWRITECD_H
