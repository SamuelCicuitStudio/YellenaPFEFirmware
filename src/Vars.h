#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <SPIFFS.h>
#include <time.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include "Credential.h"
#include "Loginhtml.h"        // Include the HTML header file
#include "DownloadHTML.h" // Include the file upload HTML header file
#include "APmodeHTML.h"

#define EEPROM_SIZE 256 // Size of EEPROM in bytes
#define WIFI_SSID_ADD 10
#define WIFI_PASS_ADD 50
#define AP_SSID "YelliWifi"
#define AP_PASS "1234567890"

String WIFI_SSID = "";
String WIFI_PASS = "";

AsyncWebServer server(80);
const char* reservationFile = "/reservations.json";

// Set timezone offset (e.g., UTC+1 hour = 3600 seconds)
const int tzOffset = 3600; // 1 hour offset

String adminPassword = "Mypassword"; // Replace with your admin password
String SentAdminPassword = ""; // From the server

bool isEEPROMInitialized = false; // Flag to track if EEPROM has been initialized


