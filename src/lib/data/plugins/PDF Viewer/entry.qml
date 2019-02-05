import io.crusta.extension 1.0

PluginInterface {
    property string pdfjsPath: 'file:///--TODO--'

    load: function () {
        console.log('PDFewr plugin loaded')
    }

    acceptDownloadRequest: function(download) {
        if (download.mimeType !== 'application/pdf') {
            return
        }

        download.webPage().load(pdfjsPath + '/web/viewer.html?file=' + download.url)
        download.cancel()
    }

    unload: function() {
        console.log('PDFewr plugin unloaded')
    }
}
