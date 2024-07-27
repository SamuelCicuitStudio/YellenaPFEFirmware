#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <SPIFFS.h>  // Include SPIFFS for file system
#include "Credential.h"
#include "DownloadHTML.h"

// Create an instance of the server
AsyncWebServer server(80);

// Function to handle file upload
void handleFileUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {
    static File uploadFile;
    if (!index) {
        // Starting the upload
        Serial.printf("Upload Start: %s\n", filename.c_str());
        uploadFile = SPIFFS.open("/" + filename, FILE_WRITE);
        if (!uploadFile) {
            Serial.println("Failed to open file for writing");
            request->send(500, "text/plain", "Failed to open file");
            return;
        }
    }

    if (len) {
        // Write the received data to the file
        uploadFile.write(data, len);
        Serial.printf("Received %d bytes\n", len);
    }

    if (final) {
        // Finalize the upload
        uploadFile.close();
        Serial.printf("Upload End: %s (%d bytes)\n", filename.c_str(), index + len);
        request->send(200, "text/plain", "File received successfully");
    }
}

void setup() {
    Serial.begin(9600);

    // Initialize SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("Failed to mount file system");
        return;
    }

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");

// Print the IP address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

    // Serve the HTML file upload page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", htmlContent);
    });

    // Handle file uploads
    server.on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "File received");
    }, handleFileUpload);

    // Start the server
    server.begin();
}

void loop() {
    // Nothing here
}
