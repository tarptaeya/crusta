new QWebChannel(qt.webChannelTransport, function (channel) {
    if (window.external === undefined) {
        window.external = {}
    }

    Object.keys(channel.objects).forEach(key => window.external[key] = channel.objects[key]);

    let event = new Event('__init__webchannel__');
    document.dispatchEvent(event);
});
