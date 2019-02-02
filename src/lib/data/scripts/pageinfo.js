let openSearchData = document.querySelector('link[type="application/opensearchdescription+xml"]')

function init() {
}

if (window.external.crusta === undefined) {
    document.addEventListener('__init_webChannel', init)
} else {
    init()
}
