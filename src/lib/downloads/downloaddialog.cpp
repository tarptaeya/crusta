#include "common-defs.h"
#include "downloaddialog.h"

#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

DownloadDialog::DownloadDialog(QWebEngineDownloadItem *item, QWidget *parent)
    : QDialog (parent)
{
    const QString fileName = getFileNameFromPath(item->path());

    setWindowTitle(QSL("Download %1?").arg(fileName));
    QVBoxLayout *vboxLayout = new QVBoxLayout;

    QLabel *message = new QLabel(QSL("Download <b>%1</b> of type <b>%2</b>?").arg(fileName, item->mimeType()));
    message->setTextFormat(Qt::RichText);

    QGroupBox *action = new QGroupBox(QSL("Download Options"));
    QRadioButton *justSave = new QRadioButton(QSL("Save File"));
    m_openAfterDownload = new QRadioButton(QSL("Open File"));
    QVBoxLayout *actionLayout = new QVBoxLayout;
    actionLayout->addWidget(justSave);
    actionLayout->addWidget(m_openAfterDownload);
    action->setLayout(actionLayout);

    QPushButton *cancel = new QPushButton(QSL("Cancel"));
    QPushButton *ok = new QPushButton(QSL("OK"));
    ok->setDefault(true);
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(new QWidget);
    hboxLayout->addWidget(cancel);
    hboxLayout->addWidget(ok);

    connect(cancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(ok, &QPushButton::clicked, this, &QDialog::accept);

    vboxLayout->addWidget(message);
    vboxLayout->addWidget(action);
    vboxLayout->addLayout(hboxLayout);

    setLayout(vboxLayout);
}

bool DownloadDialog::openAfterDownload()
{
    return m_openAfterDownload->isChecked();
}
