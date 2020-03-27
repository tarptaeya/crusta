import Crusta 1.0

PluginInterface {
    load: function() {
        return true
    }

    accept_navigation_request: function(url) {
        if (url.endsWith(".user.js")) {
            // TODO: the resource is userscript, install it
            return false;
        }

        return true;
    }
}
