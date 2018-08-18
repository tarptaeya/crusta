var _ = new QWebChannel(qt.webChannelTransport, function(channel) {
    window.external = channel.objects
    var event = new Event('__init_webchannel')
    document.dispatchEvent(event)
});
