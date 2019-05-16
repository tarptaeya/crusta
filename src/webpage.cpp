#include "webobject.h"
#include "webpage.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWebChannel>
#include <QWebEngineScript>

WebPage::WebPage(QWebEngineProfile *profile, QObject *parent)
    : QWebEnginePage (profile, parent)
{
    QWebChannel *channel = new QWebChannel(this);
    WebObject *webObject = new WebObject(this);
    channel->registerObject(QStringLiteral("browser"), webObject);

    setWebChannel(channel, QWebEngineScript::ApplicationWorld);

    connect(webObject, &WebObject::popupRequested, [this] (QWidget *widget) { emit popupRequested(widget); });

    connect(this, &WebPage::featurePermissionRequested, [this] (const QUrl &securityOrigin, WebPage::Feature feature) {
        QWidget *widget = featureWidget(this, securityOrigin, feature);
        emit popupRequested(widget);
    });
}

QWidget *WebPage::featureWidget(QWebEnginePage *page, const QUrl &securityOrigin, WebPage::Feature feature)
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
