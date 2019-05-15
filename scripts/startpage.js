// ==UserScript==
// @include crusta://startpage
// ==/UserScript==

function setSearchEngineIcon() {
    const el = document.querySelector('#engine-logo');
    window.external.browser.searchEngine.defaultSearchEngineFaviconUrl(function(url) {
        el.style.backgroundImage = `url(${url})`;
    });
}

function init() {
    setSearchEngineIcon();

    const el = document.querySelector('#search-box')
    el.addEventListener('keyup', function(event) {
        if (event.keyCode === 13) {
            const query = el.value;
            window.external.browser.searchEngine.searchUrlFromQuery(query, function(url) {
                window.location.href = url;
            })
        }
    });
}

if (window.external.browser === undefined) {
    document.addEventListener('__init__webchannel__', init);
} else {
    init();
}
