#include "speeddial.h"
#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QUrl>

Speeddial::Speeddial(QObject *parent, QWebEnginePage *page)
    : QObject(parent)
{
    m_page = page;
}

void Speeddial::addDial()
{
    QDialog dialog;
#ifdef Q_OS_MAC
    dialog.setParent(m_page->view());
    dialog.setWindowFlag(Qt::Sheet);
#else
    dialog.setWindowTitle(tr("Add new dial"));
#endif
    QVBoxLayout vBoxLayout;

    QLineEdit urlLineEdit;
    urlLineEdit.setPlaceholderText(tr("Enter URL of new dial"));
    vBoxLayout.addWidget(&urlLineEdit);

    QLabel errorLabel;
    QPalette palette;
    palette.setColor(errorLabel.foregroundRole(), Qt::red);
    errorLabel.setPalette(palette);
    vBoxLayout.addWidget(&errorLabel);

    QWidget spacer;
    QPushButton okButton(tr("Ok"));
    okButton.setDefault(true);
    QPushButton cancelButton(tr("Cancel"));
    QHBoxLayout hBoxLayout;
    hBoxLayout.addWidget(&spacer);
    hBoxLayout.addWidget(&okButton);
    hBoxLayout.addWidget(&cancelButton);
    hBoxLayout.setStretch(0, 1);
    hBoxLayout.setStretch(1, 0);
    hBoxLayout.setStretch(2, 0);
    hBoxLayout.setContentsMargins(0, 0, 0, 0);
    vBoxLayout.addLayout(&hBoxLayout);

    dialog.setLayout(&vBoxLayout);

    connect(&okButton, &QPushButton::clicked, this, [&dialog, &urlLineEdit, &errorLabel]{
        if (QUrl::fromUserInput(urlLineEdit.text()).isValid()) {
            dialog.accept();
        } else {
            errorLabel.setText(tr("Please enter a valid URL"));
        }
    });

    connect(&cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.setMinimumWidth(300);
    if (dialog.exec() == QDialog::Accepted) {
        emit dialAdded(urlLineEdit.text());
    }
}
