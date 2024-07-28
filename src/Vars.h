#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <SPIFFS.h>
#include <time.h>
#include <ArduinoJson.h>
#include "Credential.h"
#include "Loginhtml.h"        // Include the HTML header file
#include "DownloadHTML.h" // Include the file upload HTML header file
AsyncWebServer server(80);

const char* reservationFile = "/reservations.json";

// Set timezone offset (e.g., UTC+1 hour = 3600 seconds)
    const int tzOffset = 3600; // 1 hour offset

    String adminPassword = "Mypassword"; // Replace with your admin password