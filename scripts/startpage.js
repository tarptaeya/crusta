// ==UserScript==
// @include crusta:startpage
// ==/UserScript==

function setSearchEngineIcon() {
    window.external.browser.searchEngine.defaultSearchEngineFaviconUrl(function(url) {
        document.querySelector('#engine-logo').style.backgroundImage = `url(${url})`;
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
        window.external.browser.startPage.newDialPopup();
    })
}

function setupMenuButton() {
    document.querySelector('#menu-button').addEventListener('click', function() {
        window.external.browser.startPage.dialSettingPopup();
    })
}

function createDial(d) {
    const dial = document.createElement('div');
    dial.className = 'dial';
    const image = document.createElement('div');
    image.className = 'image';
    image.style.backgroundImage = `url(${d.address}/favicon.ico)`;
    const text = document.createElement('div');
    text.textContent = d.title;
    dial.appendChild(image);
    dial.appendChild(text);

    dial.addEventListener('click', function() {
        window.location.href = d.address;
    })

    document.querySelector('.dials').insertBefore(dial, document.querySelector('#new-dial'));
}

function loadDials() {
    window.external.browser.startPage.dialAdded.connect(function(dial) {
        createDial(dial);
    });

    window.external.browser.startPage.loadAllDials();
}

function init() {
    setSearchEngineIcon();
    setupSearchBox();
    setupSpeedDial();
    setupMenuButton();

    loadDials();
}

if (window.external.browser === undefined) {
    document.addEventListener('__init__webchannel__', init);
} else {
    init();
}
