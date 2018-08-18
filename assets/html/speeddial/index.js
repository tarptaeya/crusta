// QtCreator doesn't supports ES6 syntax currently

function createAddDial() {
    var element = document.createElement('div')
    element.id = 'add-dial-container'
    var blurryElement = document.createElement('div')
    blurryElement.id = 'add-dial-blurry-element'
    element.appendChild(blurryElement)
    var imageElement = document.createElement('img')
    imageElement.id = 'add-dial-image'
    imageElement.src = '../../icons/plus.svg'
    element.appendChild(imageElement)
    document.querySelector('#container').appendChild(element)
}

function blurAddDial() {
    var element = document.querySelector('#add-dial-blurry-element')
    var boundingRect = element.getBoundingClientRect()
    element.style.backgroundPosition = (-boundingRect.left) + 'px' + ' ' + (-boundingRect.top) + 'px'
}

function createDial() {
    var dial = {}
    dial.element = document.createElement('div')
    dial.element.className = 'dial'
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
    })
    document.querySelector('#container').appendChild(dial.element)
}

window.onload = function() {
    createAddDial()
    blurAddDial()
}

window.onresize = function() {
    blurAddDial()
}
