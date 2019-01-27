function registerWebChannel() {
    new QWebChannel(qt.webChannelTransport, (channel) => {
        if (!window.external) {
            window.external = {}
        }
        window.external = channel.objects
        let event = new Event('__init_webChannel')
        document.dispatchEvent(event)
    })
}

registerWebChannel()
