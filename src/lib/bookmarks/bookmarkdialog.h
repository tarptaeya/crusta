#ifndef CR_BOOKMARK_DIALOG_H
#define CR_BOOKMARK_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>

class BookmarkDialog : public QDialog
{
public:
    explicit BookmarkDialog(QWidget *parent = nullptr);

    void setAddress(const QString &address);
    void setTitle(const QString &title);

private:
    void accept();
    void reject();

    QString m_address;

    QLineEdit *m_titleLine = nullptr;
    QComboBox *m_folderCombo = nullptr;
};

#endif
