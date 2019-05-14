function getOpenSearchElement() {
    return document.querySelector('link[rel="search"]');
}

function handleOpenSearchData(xmlDoc) {
    let name = xmlDoc.querySelector('ShortName');
    let description = xmlDoc.querySelector('Description');
    let url = xmlDoc.querySelector('Url[type="text/html"]');

    if (name) { name = name.textContent; }
    if (description) { description = description.textContent; }
    if (url) { url = url.getAttribute('template'); }

    window.external.browser.searchEngine.openSearchFound(name, description, url);
}

function init() {
    const el = getOpenSearchElement();
    if (el === null) {
        return;
    }

    const url = el.getAttribute('href');
    const xhr = new XMLHttpRequest();
    xhr.addEventListener('load', () => handleOpenSearchData(xhr.responseXML.documentElement));
    xhr.open('GET', url, true);
    xhr.responseType = 'document';
    xhr.send();
}

if (window.external.browser === undefined) {
    document.addEventListener('__init__webchannel__', init);
} else {
    init();
}
