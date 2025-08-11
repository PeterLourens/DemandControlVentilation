const char websocket_js[] = R"rawliteral(

var gateway = 'ws://' + window.location.hostname + ':8080/ws';
var websocket;
var page;

// Init web socket when the page loads
window.addEventListener('load', onload);

function onload(event) {
    initWebSocket();
}

function initWebSocket() {
    console.log('Trying to open a WebSocket connection…');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

// When websocket is established, call the get_json() function
function onOpen(event) {
    console.log('Connection opened');
    get_json();
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}

// Request to server for readings
function get_json(){
    //const page_name = window.location.pathname.split("/").pop() || "index";
    //const page_name = window.location.pathname;
    $(document).on('click', 'ul.pure-menu-list li a', function (event) {
        var page_name = $(this).attr('href');
    });

    console.log(page_name);
    websocket.send(page_name);
}

// Function that receives the message from the ESP32 with the readings and matches ids of the json with ids in the html
function onMessage(event) {
    //console.log(event.data);
    var myObj = JSON.parse(event.data);
    var keys = Object.keys(myObj);

    for (const key in myObj) {
      const element = document.getElementById(key);
      if (element) {
        element.textContent = myObj[key];
      }
    }
};


)rawliteral";