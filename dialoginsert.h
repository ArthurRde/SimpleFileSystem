#ifndef DIALOGINSERT_H
#define DIALOGINSERT_H

#include <QDialog>
#include "cdsystem.h"
#include "string"

using namespace std;

namespace Ui {
    class DialogInsert;
}

/**
 * @class DialogInsert
 * @brief A dialog for inserting CDs into the system.
 */
class DialogInsert : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Pointer to the CDSYSTEM.
     */
    CDSYSTEM *sys;

    /**
     * @brief Constructor for DialogInsert.
     * @param sys_ Pointer to the CDSYSTEM.
     * @param parent Pointer to the parent widget.
     */
    explicit DialogInsert(CDSYSTEM *sys_, QWidget *parent = nullptr);

    /**
     * @brief Destructor for DialogInsert.
     */
    ~DialogInsert();

    /**
     * @brief Clears the CD entries from the combo box.
     */
    void clearCdFromComboBox();

    /**
     * @brief Removes a CD entry from the combo box.
     * @param name Name of the CD to remove.
     */
    void removeCdFromComboBox(string name);

    /**
     * @brief Adds a CD entry to the combo box.
     * @param name Name of the CD to add.
     */
    void addCdToComboBox(string name);

    /**
     * @brief Slot for handling the push button click event.
     */
    void on_pushButton_clicked();

public slots:
            /**
             * @brief Slot for inserting a CD.
             */
            void insert();

private:
    /**
     * @brief Pointer to the UI.
     */
    Ui::DialogInsert *ui;
};

#endif // DIALOGINSERT_H