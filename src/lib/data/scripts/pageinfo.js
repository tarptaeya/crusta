function getOpenSearch() {
    let openSearchData = document.querySelector('link[type="application/opensearchdescription+xml"]')
    if (openSearchData === null) {
        return
    }

    let openSearchLink = openSearchData.href
    let xhr = new XMLHttpRequest()
    xhr.onreadystatechange = function() {
        if (xhr.readyState === 4) {
            let parser = new window.DOMParser()
            let parsedXML = parser.parseFromString(xhr.responseText, "text/xml")

            let name = ""
            let queryUrl = ""
            let suggestionUrl = ""

            if (parsedXML.querySelector('ShortName') !== null) {
                name = parsedXML.querySelector('ShortName').innerHTML
            }
            if (parsedXML.querySelector('Url[type="text/html"]') !== null) {
                queryUrl = parsedXML.querySelector('Url[type="text/html"]').getAttribute('template')
            }
            if (parsedXML.querySelector('Url[type="application/x-suggestions+json"]') !== null) {
                suggestionUrl = parsedXML.querySelector('Url[type="application/x-suggestions+json"]').getAttribute('template')
            }

            window.external.crusta.searchEngine.openSearchData(name, queryUrl, suggestionUrl)
        }
    }

    xhr.open('GET', openSearchLink, true)
    xhr.send('')
}

function init() {
    getOpenSearch()
}

if (window.external.crusta === undefined) {
    document.addEventListener('__init_webChannel', init)
} else {
    init()
}
