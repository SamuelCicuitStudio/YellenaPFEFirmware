#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "JoystickHTML.h"  // Include the header file with HTML content
#include "Credential.h"

// Create an instance of the server
AsyncWebServer server(80);

// Handles requests to the root
void handleRoot(AsyncWebServerRequest *request) {
  request->send(200, "text/html", htmlContent);
}

// Handles joystick movement
void handleMove(AsyncWebServerRequest *request) {
  String xStr = request->getParam("x")->value();
  String yStr = request->getParam("y")->value();
  
  int x = xStr.toInt();
  int y = yStr.toInt();

  Serial.print("Joystick X: ");
  Serial.print(x);
  Serial.print(" | Y: ");
  Serial.println(y);

  request->send(200, "text/plain", "OK");
}


void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");

  // Print the IP address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Define routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/move", HTTP_GET, handleMove);

  // Start server
  server.begin();
}

void loop() {
  // No need to handle client connections manually with AsyncWebServer
}
