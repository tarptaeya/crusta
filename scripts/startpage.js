// ==UserScript==
// @include crusta:startpage
// ==/UserScript==

function setSearchEngineIcon() {
    const el = document.querySelector('#engine-logo');
    window.external.browser.searchEngine.defaultSearchEngineFaviconUrl(function(url) {
        el.style.backgroundImage = `url(${url})`;
    });
}

function setupSearchBox() {
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

function setupSpeedDial() {
    const el = document.querySelector('#new-dial');
    el.addEventListener('click', function () {
        window.external.browser.startPage.newDialPopup(function(d) {
            if (d.address === undefined || d.title === undefined) {
                return;
            }

            if (d.address === '' || d.title === '') {
                return;
            }

            const dial = document.createElement('div');
            dial.className = 'dial';
            const image = document.createElement('div');
            image.className = 'image';
            image.style.backgroundImage = `url(${d.address}/favicon.ico)`;
            const text = document.createElement('div');
            text.textContent = d.title;
            dial.appendChild(image);
            dial.appendChild(text);

            document.querySelector('.dials').insertBefore(dial, el);
        });
    })
}

function init() {
    setSearchEngineIcon();
    setupSearchBox();
    setupSpeedDial();
}

if (window.external.browser === undefined) {
    document.addEventListener('__init__webchannel__', init);
} else {
    init();
}
