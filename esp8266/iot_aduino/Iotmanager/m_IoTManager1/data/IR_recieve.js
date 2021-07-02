document.addEventListener("DOMContentLoaded", load);
var websocketServerLocation = "'ws://' + location.hostname + ':81/', ['arduino']";
//var connection = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);
var connection;
function load(){
document.getElementById("IRcode").value ="";
second_passed();
start('ws://' + location.hostname + ':81/', ['arduino']);
}

function second_passed(){
  if (connection){
  document.getElementById("demo").innerHTML =connection.readyState;}
  else{
     document.getElementById("demo").innerHTML ="Not connected";
     //connection = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);
  }
  
  setTimeout(second_passed, 1000);
}
function start(websocketServerLocation){
    connection = new WebSocket(websocketServerLocation);
    connection.onmessage = function(evt) { handleMessage(evt.data); };
    connection.onclose = function(){
        //try to reconnect in 5 seconds
        setTimeout(function(){start(websocketServerLocation)}, 5000);
    };
    connection.onopen = function() {
      document.getElementById("IRcode").value ="Connected";
   };
 document.getElementById("demo").innerHTML =connection.readyState;
}
/*
connection.onopen = function() {

};
connection.onerror = function(error) {

  console.log('WebSocket Error ', error);
  document.getElementById("demo").innerHTML += 'WebSocket Error ' + error;

};

connection.onmessage = function(evt) {
document.getElementById("IRcode").value =evt.data;


};

*/
function handleMessage(data){
  document.getElementById("IRcode").value =data;
}
   function set_ssid(submit){
    server = "/ssid?ssid="+val('ssid')+"&password="+encodeURIComponent(val('password'));
    send_request(submit,server);
    alert("Измененя вступят в силу после перезагрузки. Пожалуйта перезагрузите устройство.");
   }
