#include "Function.h" 


void setup() {
    Serial.begin(9600);
    initTime();    // Initialize time
    initEEPROM();// Initialize EEPROM
    LoadStoredParameter();// Load Wifi credential From EEPROM
    connectToWiFi();
    initFS();// init FS
   

    // Serve the reservation HTML page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", htmlContent);
    });

    // Handle reservation form submission
    server.on("/reserve", HTTP_POST, handleReservation);

    // Serve the file upload HTML page
    server.on("/upload", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", DownloadhtmlContent);
    });

    // Handle file uploads
    server.on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "File received");
    }, handleFileUpload);

    // Display reservations
    server.on("/reservations", HTTP_GET, handleGetReservations);

        // Handle the request to check the admin password
    server.on("/check-password", HTTP_POST, [](AsyncWebServerRequest *request) {
        String password = "";
        if (request->hasParam("password", true)) {
            password = request->getParam("password", true)->value();
            Serial.println(password);
        }

        if (password == adminPassword) {
            SentAdminPassword = password;
            String response = "{\"success\": true}";
            request->send(200, "application/json", response);
        } else {
            String response = "{\"success\": false}";
            request->send(401, "application/json", response); // Unauthorized
        }
    });

    // Handle the request to clear the reservation file
    server.on("/clear-reservation-file", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (SentAdminPassword == adminPassword) {
            SentAdminPassword = "";
            clearReservationFile();
            request->send(200, "text/plain", "Reservation file cleared");
        } else {
            request->send(401, "text/plain", "Unauthorized");
            SentAdminPassword = "";
        }
    });

    server.begin();
}

void loop() {
    // Nothing needed here
}
