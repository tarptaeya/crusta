import Crusta 1.0

PluginInterface {
    load: function() {
        console.log("Plugin loaded");
    }

    unload: function() {
        console.log("Plugin unloaded");
    }
}
