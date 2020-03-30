let _ = new QWebChannel(qt.webChannelTransport, function(channel) {
    Object.keys(channel.objects).forEach(function(key) {
        window[key] = channel.objects[key];
    });

    let event = new Event("__initwebchannel");
    document.dispatchEvent(event);
});
