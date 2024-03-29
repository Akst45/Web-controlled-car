Esp CAR Control System



This Arduino sketch enables control of a Esp CAR equipped with various motors via a web interface. The system is built on an ESP32 microcontroller, allowing for wireless control over a local Wi-Fi network.

Features
Control of multiple motors for movement and other functions.
Real-time control via a web interface served by the ESP32.
WebSocket communication for low-latency interaction.
Humidity display for environmental monitoring.
Responsive design for seamless usage across devices.
Setup
Hardware Requirements:

ESP32 microcontroller board.
Motors and associated driver circuitry as per the defined pin configuration.
Stable power supply for the ESP32 and motors.
Software Requirements:

Arduino IDE with ESP32 board support.
Required libraries: WiFi.h, AsyncTCP.h, ESPAsyncWebServer.h.
Connections:

Connect motors according to the pin configuration specified in the code.
Wi-Fi Configuration:

Update the ssid and password variables in the code with your Wi-Fi credentials.
Upload the Sketch:

Open the Arduino IDE, load the sketch, and upload it to your ESP32 board.
Accessing the Interface:

Once connected to the same Wi-Fi network, open a web browser and navigate to the IP address of the ESP32.
Usage
Web Interface:

Upon accessing the ESP32's IP address, a web interface will be displayed with control buttons for motor movement.
Use the provided buttons to control the Agro Spiderbot's movement and other functions.
WebSocket Communication:

WebSocket protocol is used for real-time communication between the web interface and the microcontroller.
WebSocket events allow for immediate feedback and control over the connected devices.
Contributions
Contributions to enhance the functionality or improve the codebase are welcome. Feel free to submit issues or pull requests.

License
This project is licensed under the MIT License.

