import io.crusta.extension 1.0

PluginInterface {
    load: function () {
        console.log('PDFewr plugin loaded')
    }

    unload: function() {
        console.log('PDFewr plugin unloaded')
    }
}
