
var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
window.addEventListener("load", onload);

function onload(event) {
  initWebSocket();
}

function getValues() {
  websocket.send("getValues");
}

function initWebSocket() {
  console.log("Trying to open a WebSocket connection…");
  websocket = new WebSocket(gateway);
  websocket.onopen = onOpen;
  websocket.onclose = onClose;
  websocket.onmessage = onMessage;
}

function onOpen(event) {
  console.log("Connection opened");
  getValues();
}

function onClose(event) {
  console.log("Connection closed");
  setTimeout(initWebSocket, 2000);
}

function updateSliderPWM(element) {
  // Actualizar el valor del input de posición
  var positionInput = document.getElementById("positionInput");

  var sliderNumber = element.id.charAt(element.id.length - 1);
  var sliderValue = document.getElementById(element.id).value;
  positionInput.value = sliderValue + "°";
  document.getElementById("sliderValue" + sliderNumber).innerHTML = sliderValue;
  console.log(sliderValue);
  websocket.send(sliderNumber + "s" + sliderValue.toString());
  var sliderValue = slider.value + "°";
}

function onMessage(event) {
  console.log(event.data);
  var myObj = JSON.parse(event.data);
  var keys = Object.keys(myObj);

  for (var i = 0; i < keys.length; i++) {
    var key = keys[i];
    document.getElementById(key).innerHTML = myObj[key];
    document.getElementById("slider" + (i + 1).toString()).value = myObj[key];
  }
}

// function updateSliderPWM(slider) {
//   // Obtener el valor del slider

// }

function updateSliderFromInput(input) {
  // Obtener el valor del input de posición

  var positionValueSlider = input.value.replace("°", "");

  // Actualizar el valor del slider
  var slider = document.getElementById("sliderValue1");
  slider.value = positionValueSlider;
}

var positionInput = document.getElementById("positionInput");

// Agregar el evento blur al input
positionInput.addEventListener("blur", function () {
  updatePositionValue();
});

// Agregar el evento keydown para detectar la tecla Enter
positionInput.addEventListener("keydown", function (event) {
  if (event.key === "Enter") {
    updatePositionValue();
  }
});

var positionValueWithoutDegreeBU;

function updatePositionValue() {
  // Obtener el valor actual del input
  var positionValue = positionInput.value;

  // Verificar si el valor no tiene el símbolo de grados
  if (!positionValue.endsWith("°")) {
    // Agregar el símbolo de grados al valor
    positionValue += "°";

    // Actualizar el valor del input de posición
    positionInput.value = positionValue;
  }

  // Eliminar el símbolo de grados para enviar el valor por websocket
  var positionValueWithoutDegree = positionValue.replace("°", "");
  console.log(positionValueWithoutDegree);

  if (positionValueWithoutDegree != positionValueWithoutDegreeBU) {
    websocket.send(1 + "s" + positionValueWithoutDegree.toString());
    positionValueWithoutDegreeBU = positionValueWithoutDegree;
    console.log(positionValueWithoutDegreeBU);
  }
}
