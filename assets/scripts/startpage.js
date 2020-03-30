// ==UserScript==
// @include browser://startpage/*
// ==/UserScript==

let dials = document.getElementById('dials');
let add_dial_button = document.getElementById('add-dial-button');
let modal = document.getElementById('modal');

async function init() {
    add_dial_button.addEventListener('click', toggle_modal);
    document.getElementById('save-dial-button').addEventListener('click', save_dial);

    let list = await browser.storage.get('startpage/dials') || [];
    list.forEach(function(s) { add_dial(s.title, s.address); })
}

if (window.browser) {
    init();
} else {
    document.addEventListener('__initwebchannel', init);
}

function toggle_modal() {
    if (modal.style.display == 'none')
        modal.style.display = 'block';
    else
        modal.style.display = 'none';
}

async function save_dial() {
    let title = document.getElementById('new-dial-title').value;
    let address = document.getElementById('new-dial-address').value;

    if (title == '' || address == '') {
        alert('Both title and address are required to save speed-dial.');
        return;
    }

    add_dial(title, address);

    let list = await browser.storage.get('startpage/dials') || [];
    list.push({title: title, address: address});
    browser.storage.set('startpage/dials', list);

    toggle_modal();

    document.getElementById('new-dial-title').value = '';
    document.getElementById('new-dial-address').value = '';
}

function add_dial(title, address) {
    let node = document.createElement('a');
    node.setAttribute('href', address);
    node.className = 'dial';
    let icon = document.createElement('div');
    icon.className = 'icon';
    let img = document.createElement('img');
    img.setAttribute('src', address + '/favicon.ico');
    img.setAttribute('width', '24px');
    img.setAttribute('height', 'auto');
    icon.appendChild(img);
    node.appendChild(icon);
    let text = document.createElement('div');
    text.textContent = title;
    text.className = 'text';
    node.appendChild(text);
    dials.insertBefore(node, add_dial_button);
}
