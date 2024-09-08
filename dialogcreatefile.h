#ifndef DIALOGCREATEFILE_H
#define DIALOGCREATEFILE_H

#include <QDialog>

namespace Ui {
    class DialogCreateFile;
}

class DialogCreateFile : public QDialog {
    Q_OBJECT

public:
    explicit DialogCreateFile(QWidget *parent = nullptr);

    ~DialogCreateFile();

    Ui::DialogCreateFile *getUi() const;

    void setUi(Ui::DialogCreateFile *newUi);

    void checkIfComplete();

private
    slots:
            void on_comboBox_currentTextChanged(
    const QString &arg1
    );

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_lineEdit_3_textChanged(const QString &arg1);

private:
    Ui::DialogCreateFile *ui;
    bool isFolder;
    bool hasName;
    bool hasAuthor;
    bool hasData;
};

#endif // DIALOGCREATEFILE_H
