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
  //createSchedTable();
  initButton();
  initWebSocket();
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
  var jsonRx = JSON.parse(event.data);
  
  if (jsonRx.sensor.temperature) {
    document.getElementById("control-sensor-temperature").innerText = parseInt(jsonRx.sensor.temperature);
  }
  
  if (jsonRx.sensor.humidity) {
    document.getElementById("control-sensor-humidity").innerText = parseInt(jsonRx.sensor.humidity);
  }

}

// ----------------------------------------------------------------------------
// Button handling
// ----------------------------------------------------------------------------

function initButton() {
  document.getElementById('ctrl-on-button').addEventListener('click', controlOn);
  document.getElementById('ctrl-off-button').addEventListener('click', controlOff);
  document.getElementById('ctrl-setpoint-button').addEventListener('click', controlSetpoint);
}

function sendSched() {
  var jsonObj = {sched:[]};
  t = document.getElementById('sched-table');
  jsonObj.sched = tableToJson(t);  
  jsonStr = JSON.stringify(jsonObj);
  console.log(jsonStr);
  //document.getElementById("demo").innerHTML = jsonStr;
  //websocket.send(jsonStr);
}

function startSched() {
  var jsonObj = {sched:{enable:true}}; 
  jsonStr = JSON.stringify(jsonObj);
  console.log(jsonStr);
  //document.getElementById("demo").innerHTML = jsonStr;
  //websocket.send(jsonStr);
}

function stopSched() {
  var jsonObj = {sched:{enable:false}}; 
  jsonStr = JSON.stringify(jsonObj);
  console.log(jsonStr);
  //document.getElementById("demo").innerHTML = jsonStr;
  //websocket.send(jsonStr);
}

function controlOn() {
  var jsonObj = { "enable": true }; 
  sendJsonToSrv(jsonObj);
}

function controlOff() {
  var jsonObj = { "disable": true }; 
  sendJsonToSrv(jsonObj);
}

function controlSetpoint() {
  var setpoint = document.getElementById("ctrl-setpoint-input").valueAsNumber;
  var jsonObj = { "setpoint": setpoint }; 
  sendJsonToSrv(jsonObj);
}

function sendJsonToSrv(jsonObj) {
  jsonStr = JSON.stringify(jsonObj);
  console.log(jsonStr);
  websocket.send(jsonStr);
}

function test() {
  var stepData = {
    step: []
  };
  stepData.step.push({
    "period": 1,
    "setpoint": 25                        
  });
  stepData.step.push({
    "period": 3,
    "setpoint": 30                        
  });
  createSchedTable(stepData);
}

function tableToJson(table) {
    
  var sched = {duration:[], setpoint:[]};
  // go through columns
  var j = 1;
 	for (key in sched) {
                
    var dataArray = []
    // go through rows
    for (var i=1; i<table.rows.length; i++) {
      var tableRow = table.rows[i];        
      dataArray.push(tableRow.cells[j].querySelector('input').value);
    }       
    sched[key] = dataArray;
    j++;
  }
  
	return sched;
}

function clearSchedTable() {
  createNewSchedTable();
  var jsonObj = {sched:{enable:false}}; 
  jsonStr = JSON.stringify(jsonObj);
  console.log(jsonStr);
}

function createSchedTable(stepData) {
  var table = document.getElementById("sched-table");
  var columns = 4;  
  
  addRow();
  
}

function addRow() {
  var table = document.getElementById("sched-table");

  // Create row
  var row = document.createElement("tr");
  
  // Create Column 1
  var cell = document.createElement("td");
  cell.innerHTML = table.rows.length;
  cell.setAttribute("contenteditable", "false");
  row.appendChild(cell);
  
  // Create Column 2
  var cell = document.createElement("td");
  var dataInput = document.createElement("input");
  dataInput.setAttribute("class", "sched-table-input");
  dataInput.setAttribute("type", "number");
  dataInput.setAttribute("value", 10);
  cell.appendChild(dataInput);
  row.appendChild(cell);

  // Create Column 2
  var cell = document.createElement("td");
  var dataInput = document.createElement("input");
  dataInput.setAttribute("class", "sched-table-input");
  dataInput.setAttribute("type", "number");
  dataInput.setAttribute("value", 25);
  cell.appendChild(dataInput);
  row.appendChild(cell);

  // Create Column 3
  var cell = document.createElement("td");
  var dataInput = document.createElement("input");
  dataInput.setAttribute("class", "select-checkbox");
  dataInput.setAttribute("type", "checkbox");
  cell.appendChild(dataInput);
  row.appendChild(cell);
  
  table.appendChild(row);
}

function deleteRows() {
  let table = document.getElementById("sched-table");
  for (i = (table.rows.length - 1); i >= 1; i--) {
    let row = table.rows[i];
    let checkBox = row.cells[3].children[0];
    if (checkBox.checked) {
      if (table.rows.length > 2) {
        table.deleteRow(i);
      }
    }
  }

  /* Renumber rows */
  for (i = 1; i < table.rows.length; i++) {
    table.rows[i].cells[0].innerHTML = i;
  }
}

function schedSelectAll(element) {
  var selectCheckbox = document.getElementsByClassName("select-checkbox");
  for (var i = 0; i < selectCheckbox.length; i++) {
    selectCheckbox[i].checked = element.checked;
  }
}
