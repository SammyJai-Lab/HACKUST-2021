var start = false
var ledSwitch = false

function buttonOnOff(){
    start = !start;
    if (start == true) {
        document.getElementById("startButton").innerHTML = "Stop";
    }
    if (start == false) {
        document.getElementById("startButton").innerHTML = "Start";
    }
    getValues();
}

function buttonLedOnOff(){
    ledSwitch = !ledSwitch;
    if (ledSwitch == true) {
        document.getElementById("ledButton").innerHTML = "Turn off led";
    }
    if (ledSwitch == false) {
        document.getElementById("ledButton").innerHTML = "Turn on led";
    }
    switchLed();
}

function switchLed() {
    eel.switchLed();
}

function getValues() {
    if (start) {
        eel.getValues()(callBack);
        setTimeout(getValues, 1000);
    }
}

function callBack(result) {
    document.getElementById("serveState").innerHTML="Serve State&#9:  " + result[0].toString();
    document.getElementById("resetPending").innerHTML="Reset Pending&#9:  " + result[1].toString();
    document.getElementById("ledGreen").innerHTML="Led Green&#9&#9:  " + result[2].toString();
    document.getElementById("ledRed").innerHTML="Led Red&#9&#9:  " + result[3].toString();
}

function sleep(milliseconds) {
    var start = new Date().getTime();
    for (var i = 0; i < 1e7; i++) {
      if ((new Date().getTime() - start) > milliseconds){
        break;
      }
    }
  }