#include "webpage.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

WebPage::WebPage(QWebEngineProfile *profile, QObject *parent)
    : QWebEnginePage (profile, parent)
{
}

QWidget *WebPage::featureWidget(QWebEnginePage *page, const QUrl &securityOrigin, QWebEnginePage::Feature feature)
{
    QWidget *widget = new QWidget;
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    widget->setLayout(vboxLayout);

    QString featureString;
    switch (feature) {
    case QWebEnginePage::Geolocation: featureString = QStringLiteral("accessing location"); break;
    case QWebEnginePage::MediaAudioCapture: featureString = QStringLiteral("accessing microphone"); break;
    case QWebEnginePage::MediaVideoCapture: featureString = QStringLiteral("accessing camera"); break;
    case QWebEnginePage::MediaAudioVideoCapture: featureString = QStringLiteral("accessing microphone and camera"); break;
    case QWebEnginePage::MouseLock: featureString = QStringLiteral("locking mouse"); break;
    case QWebEnginePage::DesktopVideoCapture: featureString = QStringLiteral("desktop video capture"); break;
    case QWebEnginePage::DesktopAudioVideoCapture: featureString = QStringLiteral("desktop audio and video capture"); break;
    default: break;
    }

    QLabel *label = new QLabel(QStringLiteral("Page at <b>%1</b> requests permission for <b>%2</b>")
                               .arg(securityOrigin.host())
                               .arg(featureString));
    label->setWordWrap(true);

    vboxLayout->addWidget(label);

    QCheckBox *rememberBox = new QCheckBox(QStringLiteral("Remember my choice"));
    vboxLayout->addWidget(rememberBox);

    QHBoxLayout *hboxLayout = new QHBoxLayout;
    vboxLayout->addLayout(hboxLayout);

    QPushButton *allow = new QPushButton(QStringLiteral("Allow"));
    QPushButton *reject = new QPushButton(QStringLiteral("Reject"));

    hboxLayout->addWidget(new QWidget);
    hboxLayout->addWidget(reject);
    hboxLayout->addWidget(allow);

    connect(reject, &QPushButton::clicked, page, [widget, page, securityOrigin, feature] {
        page->setFeaturePermission(securityOrigin, feature, QWebEnginePage::PermissionDeniedByUser);
        widget->close();
    });
    connect(allow, &QPushButton::clicked, page, [widget, page, securityOrigin, feature] {
        page->setFeaturePermission(securityOrigin, feature, QWebEnginePage::PermissionGrantedByUser);
        widget->close();
    });

    widget->setMinimumWidth(250);

    return widget;
}
