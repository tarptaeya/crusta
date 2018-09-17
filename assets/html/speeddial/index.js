// ==UserScript==
// @run-at   document-end
// @include  qrc:/html/speeddial/index.html
// ==/UserScript==

var addDial;

function createAddDial() {
    addDial = document.createElement('div')
    addDial.id = 'add-dial-container'
    var blurryElement = document.createElement('div')
    blurryElement.id = 'add-dial-blurry-element'
    addDial.appendChild(blurryElement)
    var imageElement = document.createElement('img')
    imageElement.id = 'add-dial-image'
    imageElement.src = '../../icons/plus.svg'
    addDial.appendChild(imageElement)
    document.querySelector('#container').appendChild(addDial)
}

function blurAddDial() {
    var element = document.querySelector('#add-dial-blurry-element')
    var boundingRect = element.getBoundingClientRect()
    element.style.backgroundPosition = (-boundingRect.left) + 'px' + ' ' + (-boundingRect.top) + 'px'
}

function createDial(item) {
    var dial = {}
    dial.element = document.createElement('div')
    dial.element.className = 'dial'
    dial.element.style.backgroundImage = 'url("data:image/png;base64, ' + item.image + '")'
    dial.element.addEventListener('click', function(event) {
        var ripple = document.createElement('div')
        ripple.className = 'ripple'
        dial.element.appendChild(ripple)
        var maxSide = Math.max(dial.element.clientWidth, dial.element.clientHeight)
        ripple.style.width = maxSide + 'px'
        ripple.style.height = maxSide + 'px'
        var boundingRect = dial.element.getBoundingClientRect()
        ripple.style.left = (event.clientX - boundingRect.left - maxSide / 2) + 'px'
        ripple.style.top = (event.clientY - boundingRect.top - maxSide / 2) + 'px'
        setTimeout(function() {
            dial.element.removeChild(ripple)
        }, 2000)

        location.href = item.url
    })
    dial.removeButton = document.createElement('div')
    dial.removeButton.className = 'removebutton'
    dial.removeButton.innerHTML = '&times;'
    dial.removeButton.addEventListener('click', function(event) {
        event.stopPropagation();

        var result = confirm('Remove the dial?')
        if (result) {
            var speeddial = window.external.externalObject.speeddial
            speeddial.removeDial(item.url)

            document.querySelector('#container').removeChild(dial.element)
        }
    })
    dial.element.appendChild(dial.removeButton)

    dial.titlebar = document.createElement('div')
    dial.titlebar.className = 'titlebar'
    dial.titlebar.innerHTML = item.title
    dial.element.appendChild(dial.titlebar)

    document.querySelector('#container').insertBefore(dial.element, addDial)
}

function initChannel() {
    var speeddial = window.external.externalObject.speeddial

    addDial.addEventListener('click', function() {
        speeddial.addDial()
    })

    speeddial.dialAdded.connect(function(item) {
        createDial(item)
    })

    speeddial.dialsAdded.connect(function(items) {
        items.forEach(function(item) {
            createDial(item)
        })
    })

    speeddial.loadDialsFromDatabase()
}

window.onload = function() {
    createAddDial()
    blurAddDial()

    if (window.external.externalObject) {
        initChannel()
    } else {
        document.addEventListener('__init_webchannel', initChannel)
    }
}

window.onresize = function() {
    blurAddDial()
}
