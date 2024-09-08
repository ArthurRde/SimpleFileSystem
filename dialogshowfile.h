#ifndef DIALOGSHOWFILE_H
#define DIALOGSHOWFILE_H

#include <QDialog>

using namespace std;

namespace Ui {
    class DialogShowFile;
}

/**
 * @class DialogShowFile
 * @brief A dialog for showing file details.
 */
class DialogShowFile : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor for DialogShowFile.
     * @param name Name of the file.
     * @param author Author of the file.
     * @param date Date of the file.
     * @param size Size of the file.
     * @param data Data of the file.
     * @param parent Pointer to the parent widget.
     */
    explicit DialogShowFile(string name, string author, QDateTime date, int size, string data,
                            QWidget *parent = nullptr);

    /**
     * @brief Destructor for DialogShowFile.
     */
    ~DialogShowFile();

    /**
     * @brief Gets the data text of the file.
     * @return Data text as a string.
     */
    string getDataText();

private:
    /**
     * @brief Pointer to the UI.
     */
    Ui::DialogShowFile *ui;
};

#endif // DIALOGSHOWFILE_H