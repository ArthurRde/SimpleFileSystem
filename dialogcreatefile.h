#ifndef DIALOGCREATEFILE_H
#define DIALOGCREATEFILE_H

#include <QDialog>

namespace Ui {
    class DialogCreateFile;
}

/**
 * @brief The DialogCreateFile class represents a dialog window for creating a file or folder.
 *
 * This class provides a graphical interface to the user for inputting details about a file or folder,
 * such as name, author, and file type (folder or regular file).
 */
class DialogCreateFile : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructs a DialogCreateFile object.
     *
     * Initializes the UI and other settings for the dialog window.
     *
     * @param parent Pointer to the parent widget (defaults to nullptr).
     */
    explicit DialogCreateFile(QWidget *parent = nullptr);

    /**
     * @brief Destructor for DialogCreateFile.
     *
     * Cleans up the allocated resources, including the UI object.
     */
    ~DialogCreateFile();

    /**
     * @brief Returns the UI object associated with this dialog.
     *
     * @return Pointer to the Ui::DialogCreateFile object.
     */
    Ui::DialogCreateFile *getUi() const;

    /**
     * @brief Sets a new UI object for this dialog.
     *
     * @param newUi Pointer to the new Ui::DialogCreateFile object.
     */
    void setUi(Ui::DialogCreateFile *newUi);

    /**
     * @brief Checks if the input fields are complete to enable file creation.
     *
     * This method checks if all required fields (name, author, etc.) are filled.
     */
    void checkIfComplete();

private slots:
            /**
             * @brief Handles the event when the selected text in the combo box changes.
             *
             * This slot is triggered when the file type (file or folder) is changed by the user.
             *
             * @param arg1 The new text selected in the combo box.
             */
            void on_comboBox_currentTextChanged(const QString &arg1);

    /**
     * @brief Handles the event when the text in the first line edit field changes.
     *
     * This slot is triggered when the user changes the name of the file or folder.
     *
     * @param arg1 The new text entered in the first line edit field.
     */
    void on_lineEdit_textChanged(const QString &arg1);

    /**
     * @brief Handles the event when the text in the second line edit field changes.
     *
     * This slot is triggered when the user changes the author name.
     *
     * @param arg1 The new text entered in the second line edit field.
     */
    void on_lineEdit_2_textChanged(const QString &arg1);

    /**
     * @brief Handles the event when the text in the third line edit field changes.
     *
     * This slot is triggered when the user changes the file content or other data.
     *
     * @param arg1 The new text entered in the third line edit field.
     */
    void on_lineEdit_3_textChanged(const QString &arg1);

private:
    Ui::DialogCreateFile *ui;   ///< Pointer to the UI object for this dialog.
    bool isFolder;              ///< Flag indicating whether the dialog is for creating a folder.
    bool hasName;               ///< Flag indicating whether a valid name has been entered.
    bool hasAuthor;             ///< Flag indicating whether a valid author name has been entered.
    bool hasData;               ///< Flag indicating whether valid data has been entered.
};

#endif // DIALOGCREATEFILE_H
