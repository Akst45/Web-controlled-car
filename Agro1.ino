#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define UP     1
#define DOWN   2
#define LEFT   3
#define RIGHT  4
#define UP2    5
#define UP2_1  6
#define WEED   7
#define SPEAKER 8
#define STOP   0

#define FORWARD   1
#define BACKWARD -1

struct MOTOR_PINS {
  int pinIN1;
  int pinIN2;
};

std::vector<MOTOR_PINS> motorPins = {
  {18},  // UP
  {5},   // DOWN
  {4},   // LEFT
  {19},  // RIGHT
  {13},  // UP2
  {14},  // UP2_1
  {21},  // WEED
  {22}   // SPEAKER
};

const char* ssid = "Your ssid";
const char* password = "Your password";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char* htmlHomePage PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
      <title>AGRO SPIDERBOT </title>

    <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
    <style>
      .arrows {
        font-size: 70px;
        color: green;
      }
      .circularArrows {
        font-size: 80px;
        color: blue;
      }
      td {
        background-color: black;
        border-radius: 25%;
        box-shadow: 5px 5px #888888;
      }
      td:active {
        transform: translate(5px, 5px);
        box-shadow: none;
      }
      .noselect {
        -webkit-touch-callout: none; /* iOS Safari */
        -webkit-user-select: none; /* Safari */
        -khtml-user-select: none; /* Konqueror HTML */
        -moz-user-select: none; /* Firefox */
        -ms-user-select: none; /* Internet Explorer/Edge */
        user-select: none; /* Non-prefixed version, currently supported by Chrome and Opera */
      }
    </style>
    <script>
      function fetchHumidityData() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("humidityValue").innerHTML = this.responseText;
          }
        };
        xhttp.open("GET", "/humidity", true);
        xhttp.send();
      }

      // Refresh humidity data every 5 seconds
      setInterval(fetchHumidityData, 5000);

      // Function to send commands via WebSocket
      function onTouchStartAndEnd(value) {
        websocket.send(value);
      }

      // WebSocket initialization
      var webSocketUrl = "ws:\/\/" + window.location.hostname + "/ws";
      var websocket;
      function initWebSocket() {
        websocket = new WebSocket(webSocketUrl);
        websocket.onopen = function(event) {};
        websocket.onclose = function(event) { setTimeout(initWebSocket, 2000); };
        websocket.onmessage = function(event) {};
      }
      window.onload = function() {
        initWebSocket();
        fetchHumidityData();
      };
    </script>
  </head>
  <body class="noselect" align="center" style="background-color: black">
    <h1 style="color: teal; text-align: center;">AGRO SPIDER DASHBOARD</h1>
    <h2 style="color: teal; text-align: center;">Wi-Fi &#128375; Control</h2>
    <table id="mainTable" style="width: 400px; margin: auto; table-layout: fixed" CELLSPACING=10>
<tr>
  <td style="visibility: hidden;"></td> <!-- Invisible cell -->
  <td ontouchstart='onTouchStartAndEnd("1")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows">&#8679;</span></td> <!-- Arrow pointing upwards -->
  <td style="visibility: hidden;"></td> <!-- Invisible cell -->
</tr>
<tr>
  <td ontouchstart='onTouchStartAndEnd("3")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows">&#8678;</span></td>
  <td></td>
  <td ontouchstart='onTouchStartAndEnd("4")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows">&#8680;</span></td>
</tr>
<tr>
  <td style="visibility: hidden;"></td> <!-- Invisible cell -->
  <td ontouchstart='onTouchStartAndEnd("2")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows">&#8681;</span></td>
  <td style="visibility: hidden;"></td> <!-- Invisible cell -->
</tr>
<tr>
  <td ontouchstart='onTouchStartAndEnd("5")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows">&#10506;</span></td>
  <td style="visibility: hidden;"></td> <!-- Invisible cell -->
  <td ontouchstart='onTouchStartAndEnd("6")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows">&#x290B;</span></td>
</tr>
<tr>
  <td ontouchstart='onTouchStartAndEnd("7")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows">&#127811;</span></td>
  <td ontouchstart='onTouchStartAndEnd("8")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows">&#129714;</span></td>
</tr>
    </table>

    <!-- Humidity display -->
    <div style="margin-top: 20px;">
      <h3>Humidity:</h3>
      <div id="humidityValue"></div>
    </div>
  </body>
</html>
)rawliteral";

void rotateMotor(int motorNumber, int motorDirection) {
  if (motorDirection == FORWARD) {
    digitalWrite(motorPins[motorNumber].pinIN1, HIGH);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);
  }
  else if (motorDirection == BACKWARD) {
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, HIGH);
  }
  else {
    // Stop motor
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);
  }
}

void rotateMotorUP2() {
  digitalWrite(13, LOW); // M2
  digitalWrite(14, HIGH); // M1
  delay(40);
  digitalWrite(14, LOW);
}

void rotateMotorUP2_1() {
  digitalWrite(14, LOW); // M1
  digitalWrite(13, HIGH); // M2
  delay(40);
  digitalWrite(13, LOW);
}

void processCarMovement(String inputValue) {
  int command = inputValue.toInt();
  switch (command) {
    case UP:
      rotateMotor(0, FORWARD); // Upward
      break;
    case DOWN:
      rotateMotor(1, FORWARD); // Downward
      break;
    case LEFT:
      rotateMotor(2, FORWARD); // Left
      break;
    case RIGHT:
      rotateMotor(3, FORWARD); // Right
      break;
    case UP2:
      rotateMotorUP2();
      break;
    case UP2_1:
      rotateMotorUP2_1();
      break;
    case WEED:
      rotateMotor(6, FORWARD); // Weed
      break;
    case SPEAKER:
      rotateMotor(7, FORWARD); // Speaker
      break;
    case STOP:
      for (size_t i = 0; i < motorPins.size(); ++i) {
        rotateMotor(i, STOP); // Stop all motors
      }
      break;
    default:
      for (size_t i = 0; i < motorPins.size(); ++i) {
        rotateMotor(i, STOP); // Stop all motors
      }
      break;
  }
}

void handleRoot(AsyncWebServerRequest *request) {
  request->send_P(200, "text/html", htmlHomePage);
}

void handleNotFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "File Not Found");
}

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      processCarMovement("0");
      break;
    case WS_EVT_DATA: {
        AwsFrameInfo *info = (AwsFrameInfo*)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
          std::string myData = "";
          myData.assign((char *)data, len);
          processCarMovement(myData.c_str());
        }
      }
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
    default:
      break;
  }
}

void setUpPinModes() {
  for (size_t i = 0; i < motorPins.size(); ++i) {
    pinMode(motorPins[i].pinIN1, OUTPUT);
    pinMode(motorPins[i].pinIN2, OUTPUT);
    rotateMotor(i, STOP);
  }
}

void setup(void) {
  setUpPinModes();
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.onNotFound(handleNotFound);

  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  ws.cleanupClients();
}
