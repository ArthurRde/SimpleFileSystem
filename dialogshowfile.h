#ifndef DIALOGSHOWFILE_H
#define DIALOGSHOWFILE_H

#include <QDialog>

using namespace std;
namespace Ui {
class DialogShowFile;
}

class DialogShowFile : public QDialog
{
    Q_OBJECT

public:
    explicit DialogShowFile(string name, string author, QDateTime date, int size, string data, QWidget *parent = nullptr);
    ~DialogShowFile();

    string getDataText();
private:
    Ui::DialogShowFile *ui;
};

#endif // DIALOGSHOWFILE_H
