/**
 * ----------------------------------------------------------------------------
 * Incubator Remote Control
 * ----------------------------------------------------------------------------
 * © 2023 Richard Hopper
 * ----------------------------------------------------------------------------
 */

var gateway = `ws://${window.location.hostname}/ws`;
var websocket;

// ----------------------------------------------------------------------------
// Initialization
// ----------------------------------------------------------------------------

window.addEventListener('load', onLoad);

function onLoad(event) {
    initWebSocket();
    initButton();
}

// ----------------------------------------------------------------------------
// WebSocket handling
// ----------------------------------------------------------------------------

function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage;
}

function onOpen(event) {
    console.log('Connection opened');
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}

function onMessage(event) {
    let data = JSON.parse(event.data);
    document.getElementById('led').className = data.status;
}

// ----------------------------------------------------------------------------
// Button handling
// ----------------------------------------------------------------------------

function initButton() {
    document.getElementById('load').addEventListener('click', onToggle);
}

function onToggle(event) {
    const jsonObj = tableToJson(stepTable);  
    const jsonStr = JSON.stringify(jsonObj);
	document.getElementById("demo").innerHTML = jsonStr;
    websocket.send(jsonStr);
}

function tableToJson(table) {
    
 	// first row needs to be headers
    myObj = {};
	// go through columns
 	for (var j=1; j<table.rows[0].cells.length; j++) {
  	var header = table.rows[0].cells[j].id;
    
		var dataArray = []
		// go through rows
   	for (var i=1; i<table.rows.length; i++) {
    	var tableRow = table.rows[i];        
     	dataArray.push(tableRow.cells[j].querySelector('input').value);
		}       
		myObj[header] = dataArray;
	}
  
	return myObj;
}