#ifndef DIALOGWRITECD_H
#define DIALOGWRITECD_H

#include <QDialog>

namespace Ui {
    class DialogWriteCd;
}

/**
 * @class DialogWriteCd
 * @brief A dialog for writing CDs.
 */
class DialogWriteCd : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor for DialogWriteCd.
     * @param parent Pointer to the parent widget.
     */
    explicit DialogWriteCd(QWidget *parent = nullptr);

    /**
     * @brief Destructor for DialogWriteCd.
     */
    ~DialogWriteCd();

private:
    /**
     * @brief Pointer to the UI.
     */
    Ui::DialogWriteCd *ui;
};

#endif // DIALOGWRITECD_H