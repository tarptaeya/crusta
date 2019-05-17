#include "tab.h"
#include "utils.h"
#include "webview.h"
#include "webpage.h"

#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>

Tab::Tab(QWidget *parent)
    : QWidget (parent)
{
    m_webView = new WebView;
    m_layout = new QVBoxLayout;

    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->addWidget(m_webView);
    setLayout(m_layout);
}

WebView *Tab::webView() const
{
    return m_webView;
}

bool Tab::isInResponsiveMode()
{
    return m_isInResponsiveMode;
}

void Tab::enterResponsiveMode(int width, int height)
{
    m_isInResponsiveMode = true;

    m_responsiveWidth = width;
    m_responsiveHeight = height;

    resizeWebView(width, height);
}

void Tab::exitResponsiveMode()
{
    m_isInResponsiveMode = false;
    resizeWebView(width(), height());
}

void Tab::resizeEvent(QResizeEvent *resize)
{
    QWidget::resizeEvent(resize);

    if (m_isInResponsiveMode) {
        resizeWebView(m_responsiveWidth, m_responsiveHeight);
    }
}

void Tab::responsiveControllPane(Tab *tab)
{
    QWidget *pane = new QWidget;
    pane->setAttribute(Qt::WA_DeleteOnClose);
    pane->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    QGridLayout *grid = new QGridLayout;
    pane->setLayout(grid);

    QComboBox *device = new QComboBox;
    QComboBox *orientation = new QComboBox;
    QPushButton *exitButton = new QPushButton(QStringLiteral("Exit Responsive Design Mode"));

    device->addItem(QStringLiteral("iPhone XR"), QSize(414, 896));
    device->addItem(QStringLiteral("iPhone XS"), QSize(315, 812));
    device->addItem(QStringLiteral("iPhone XS Max"), QSize(414, 896));
    device->addItem(QStringLiteral("iPhone X"), QSize(375, 812));
    device->addItem(QStringLiteral("iPhone 8 Plus"), QSize(414, 736));
    device->addItem(QStringLiteral("iPhone 8"), QSize(375, 667));
    device->addItem(QStringLiteral("iPhone 7 Plus"), QSize(414, 736));
    device->addItem(QStringLiteral("iPhone 7"), QSize(375, 667));
    device->insertSeparator(device->count());
    device->addItem(QStringLiteral("Google Pixel 3 XL"), QSize(412, 847));
    device->addItem(QStringLiteral("Google Pixel 3"), QSize(412, 824));
    device->addItem(QStringLiteral("Google Pixel 2 XL"), QSize(412, 732));
    device->addItem(QStringLiteral("Google Pixel XL"), QSize(412, 824));
    device->addItem(QStringLiteral("Google Pixel"), QSize(412, 732));

    orientation->addItem(QStringLiteral("Portrait"));
    orientation->addItem(QStringLiteral("Landscape"));

    exitButton->setFlat(true);

    grid->addWidget(device, 1, 1);
    grid->addWidget(orientation, 2, 1);
    grid->addWidget(exitButton, 3, 1);

    const QString originalUserAgent = tab->webView()->page()->profile()->httpUserAgent();
    const QString iPhoneUserAgent = QStringLiteral("Mozilla/5.0 (iPhone; CPU iPhone OS 12_1_3 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/12.0 Mobile/15E148 Safari/604.1");
    const QString androidUserAgent = QStringLiteral("Mozilla/5.0 (Linux; Android 8.1.0; Pixel Build/OPP6.171019.012) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.158 Mobile Safari/537.36");

    auto apply = [tab, device, orientation, iPhoneUserAgent, androidUserAgent] {
        QSize size = device->itemData(device->currentIndex()).toSize();
        orientation->currentText() == QStringLiteral("Portrait") ? tab->enterResponsiveMode(size.width(), size.height()) : tab->enterResponsiveMode(size.height(), size.width());
        const QString userAgent = device->currentText().startsWith(QStringLiteral("iPhone")) ? iPhoneUserAgent : androidUserAgent;
        tab->webView()->page()->profile()->setHttpUserAgent(userAgent);
        tab->webView()->reload();
    };

    apply();

    connect(device, &QComboBox::currentTextChanged, apply);
    connect(orientation, &QComboBox::currentTextChanged, apply);

    connect(exitButton, &QPushButton::clicked, [pane, tab, originalUserAgent] {
        tab->exitResponsiveMode();
        tab->webView()->page()->profile()->setHttpUserAgent(originalUserAgent);
        tab->webView()->reload();
        pane->close();
    });

    Utils::setMovableByBackground(pane->winId());

    pane->show();
}

void Tab::resizeWebView(int width, int height)
{
    int backgroundWidth = this->width();
    int backgroundHeight = this->height();

    int leftPadding = (backgroundWidth - width) / 2;
    int topPadding = (backgroundHeight - height) / 2;

    m_layout->setContentsMargins(leftPadding, topPadding, leftPadding, topPadding);
}
