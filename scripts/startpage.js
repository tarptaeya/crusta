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
}

if (window.external.browser === undefined) {
    document.addEventListener('__init__webchannel__', init);
} else {
    init();
}
