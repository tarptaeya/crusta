var browser;

new QWebChannel(qt.webChannelTransport, function (channel) {
    browser = channel.objects.browser;

    browser.windows.create()
});
