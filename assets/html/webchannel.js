function registerWebChannel() {
    // @disable-check M127
    new QWebChannel(qt.webChannelTransport, function(channel) {
        if (window.external) {
            window.external = channel.objects
        } else {
            window.external = {}
            window.external = channel.objects
        }
        var event = new Event('__init_webchannel')
        document.dispatchEvent(event)
    });
}

registerWebChannel()
