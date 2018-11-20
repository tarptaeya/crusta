#include "siteinfopopup.h"

SiteInfoPopup::SiteInfoPopup(QWidget *parent)
    : ToolBarPopup(parent)
{
    position = Leading;

    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    setLayout(vBoxLayout);

    m_typeLabel = new QLabel();
    vBoxLayout->addWidget(m_typeLabel);
}

void SiteInfoPopup::setUrl(const QUrl &url)
{
    const QString scheme = url.scheme();
    if (scheme == QStringLiteral("https")) {
        m_typeLabel->setText(QStringLiteral("Secure connection"));
    } else if (scheme == QStringLiteral("http")) {
        m_typeLabel->setText(QStringLiteral("Inecure connection"));
    } else if (scheme == QStringLiteral("crusta")) {
        m_typeLabel->setText(QStringLiteral("Internal page"));
    } else {
        m_typeLabel->setText(QStringLiteral("Unknown connection state"));
    }
    // TODO: permissions
}
