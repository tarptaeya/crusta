#include "common-defs.h"
#include "notifier.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

Notifier::Notifier(QWidget *parent)
    : QDialog (parent)
{
    m_featureLabel = new QLabel;
    m_acceptButton = new QPushButton(QSL("Accept"));
    m_rejectButton = new QPushButton(QSL("Reject"));

    QVBoxLayout *vboxLayout = new QVBoxLayout;

    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(new QWidget, 1);
    hboxLayout->addWidget(m_rejectButton);
    hboxLayout->addWidget(m_acceptButton);

    vboxLayout->addWidget(m_featureLabel);
    vboxLayout->addLayout(hboxLayout);
    setLayout(vboxLayout);

    m_featureLabel->setWordWrap(true);
    m_acceptButton->setDefault(true);

    connect(m_acceptButton, &QPushButton::clicked, this, &Notifier::accept);
    connect(m_rejectButton, &QPushButton::clicked, this, &Notifier::reject);
}

void Notifier::setData(const QUrl &origin, QWebEnginePage::Feature feature)
{
    switch (feature) {
    case QWebEnginePage::Geolocation:
        m_featureLabel->setText(QSL("Allow Geolocation on %1").arg(origin.toDisplayString()));
        break;
    case QWebEnginePage::MediaAudioCapture:
        m_featureLabel->setText(QSL("Allow Audio Capture on %1").arg(origin.toDisplayString()));
        break;
    case QWebEnginePage::MediaVideoCapture:
        m_featureLabel->setText(QSL("Allow Video Capture on %1").arg(origin.toDisplayString()));
        break;
    case QWebEnginePage::MediaAudioVideoCapture:
        m_featureLabel->setText(QSL("Allow Audio-Video Capture on %1").arg(origin.toDisplayString()));
        break;
    case QWebEnginePage::MouseLock:
        m_featureLabel->setText(QSL("Allow Mouse Lock on %1").arg(origin.toDisplayString()));
        break;
    case QWebEnginePage::DesktopVideoCapture:
        m_featureLabel->setText(QSL("Allow Desktop Video Capture on %1").arg(origin.toDisplayString()));
        break;
    case QWebEnginePage::DesktopAudioVideoCapture:
        m_featureLabel->setText(QSL("Allow Desktop Audio-Video Capture on %1").arg(origin.toDisplayString()));
        break;
    case QWebEnginePage::Notifications:
        m_featureLabel->setText(QSL("Allow Notifications on %1").arg(origin.toDisplayString()));
        break;
    }
}
