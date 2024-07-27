#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "Joystick.h"  // Include the header file with HTML content
#include "Credential.h"  // Include the header file with HTML content


AsyncWebServer server(80);

// Handles requests to the root
void handleRoot(AsyncWebServerRequest *request) {
  request->send(200, "text/html", htmlContent);
}

// Handles joystick movement
void handleMove(AsyncWebServerRequest *request) {
  String x;
  String y;

  if (request->hasParam("x")) {
    x = request->getParam("x")->value();
  }

  if (request->hasParam("y")) {
    y = request->getParam("y")->value();
  }

  Serial.print("Joystick X: ");
  Serial.print(x);
  Serial.print(" | Y: ");
  Serial.println(y);

  request->send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);

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
  // No need to handle client requests manually with AsyncWebServer
}
