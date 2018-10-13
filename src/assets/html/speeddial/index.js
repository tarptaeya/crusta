/* ============================================================
* Crusta - Qt5 webengine browser
* Copyright (C) 2017-2018 Anmol Gautam <tarptaeya@gmail.com>
*
* THIS FILE IS A PART OF CRUSTA
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */

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
    element.style.backgroundImage = document.body.style.backgroundImage

    var width = document.body.clientWidth
    var height = document.body.clientHeight

    var imageProxy = new Image()
    // https://stackoverflow.com/a/6397564/7526789
    imageProxy.src = document.body.style.backgroundImage.replace(/^url\(["']?/, '').replace(/["']?\)$/, '')
    var imageWidth = imageProxy.width
    var imageHeight = imageProxy.height

    var size

    if (width * imageHeight > height * imageWidth) {
        size = width + 'px ' + ((width * imageHeight) / imageWidth) + 'px'
        document.body.style.backgroundSize = size
        element.style.backgroundSize = size
    } else {
        size = ((imageWidth * height) / imageHeight) +'px ' + height + 'px'
        document.body.style.backgroundSize = size
        element.style.backgroundSize = size
    }

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
            blurAddDial()
        }
    })
    dial.element.appendChild(dial.removeButton)

    dial.titlebar = document.createElement('div')
    dial.titlebar.className = 'titlebar'
    dial.titlebar.innerHTML = item.title
    dial.element.appendChild(dial.titlebar)

    document.querySelector('#container').insertBefore(dial.element, addDial)
    blurAddDial()
}

function initChannel() {
    var speeddial = window.external.externalObject.speeddial
    var modalContainer = document.querySelector('#modal-container')

    addDial.addEventListener('click', function() {
        speeddial.addDial()
    })

    document.querySelector('#settings-button').addEventListener('click', function() {
        modalContainer.style.display = 'block'
    })

    document.querySelectorAll('.modal-tab-tab').forEach(function(tab) {
        tab.addEventListener('click', function(event) {
            document.querySelectorAll('.modal-tab-tab').forEach(function(tab) {
                tab.className = 'modal-tab-tab'
            })
            tab.className = 'modal-tab-tab pressed'

            document.querySelectorAll('.modal-tab-area').forEach(function(area) {
                area.style.display = 'none'
            })

            if (tab.id === 'use-image') {
                document.querySelector('#use-image-area').style.display = 'block'
            } else if (tab.id === 'use-solid-color') {
                document.querySelector('#use-solid-color-area').style.display = 'table-cell'
            }
        })
    })

    document.querySelector('#use-image').click()

    document.querySelectorAll('.modal-tab-area').forEach(function(area) {
        if (area.id === 'use-image-area') {
            area.addEventListener('click', function() {
                var fileInput = document.querySelector('#use-image-area-file')
                fileInput.click();
                fileInput.addEventListener('change', function() {
                    if (!fileInput.files[0]) {
                        return
                    }

                    var fileReader = new FileReader();
                    fileReader.onload = function(event) {
                        document.body.style.backgroundImage = 'url(' + event.target.result + ')'
                        speeddial.saveSetting({background: event.target.result})
                        blurAddDial()
                    }

                    fileReader.readAsDataURL(fileInput.files[0])
                })
            })
        }
    })

    window.addEventListener('click', function(event) {
        if (event.target === modalContainer) {
            modalContainer.style.display = 'none'
        }
    })

    speeddial.loadSettings(function(map) {
        document.body.style.backgroundImage = 'url(\'' + map.background + '\')'
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
    setTimeout(blurAddDial, 200)

    if (window.external.externalObject) {
        initChannel()
    } else {
        document.addEventListener('__init_webchannel', initChannel)
    }
}

var timeout
window.onresize = function() {
    clearTimeout(timeout)
    timeout = setTimeout(blurAddDial, 10)
}
