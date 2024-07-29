#include "Vars.h" 

/********************************************************************************************************************************************/
// Initialize the file system (SPIFFS) and handle any errors if mounting fails
void initFS();
// Get the current date and time as a string in the format "YYYY-MM-DD HH:MM:SS"
String getCurrentDate();
// Handle the reservation form submission. Processes POST requests, saves reservation data
// to the SPIFFS file system, and responds with a success message or an error.
void handleReservation(AsyncWebServerRequest *request);

// Display the list of reservations. Reads reservation data from SPIFFS, formats it into
// an HTML table, and sends it as a response to GET requests.
void handleGetReservations(AsyncWebServerRequest *request);

// Handle file upload requests. Manages the upload process, writes received data to a file
// on the SPIFFS file system, and sends a success message upon completion.
void handleFileUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final);
// Function to clear the reservation file in the flash of the ESP32
void clearReservationFile();
// Initialize NTP client for time synchronization
void initTime();
void storeString(int startingAddress, String data);
String readString(int addrOffset);
// Function to initialize EEPROM if not already initialized
void initEEPROM();
// Function to load stored parameters from EEPROM
void LoadStoredParameter();
bool connectToWiFi();
void startAPMode();
/********************************************************************************************************************************************/
// Initialize the file system
void initFS() {
    if (!SPIFFS.begin(true)) {
        Serial.println("An error occurred while mounting SPIFFS");
        return;
    }
}
/********************************************************************************************************************************************/
// Get the current date and time as a string
String getCurrentDate() {
    time_t now;
    struct tm timeinfo;
    char buffer[20];
    
    // Get the current time
    time(&now);
    
    // Localize the time to your time zone
    localtime_r(&now, &timeinfo);
    
    // Format the time into a string
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    
    return String(buffer);
}
;
/********************************************************************************************************************************************/
// Handle reservation form submission
void handleReservation(AsyncWebServerRequest *request) {
    if (request->method() == HTTP_POST) {
        String name = request->arg("name");
        String lastName = request->arg("lastName");
        String email = request->arg("email");
        String id = request->arg("id");
        String date = getCurrentDate();

        DynamicJsonDocument doc(2048); // Increased size to handle more data

        // Read existing reservations
        File file = SPIFFS.open(reservationFile, "r");
        if (file) {
            deserializeJson(doc, file);
            file.close();
        }

        // If the file is empty or does not exist, initialize a new JSON array
        if (!doc.is<JsonArray>()) {
            doc.to<JsonArray>();
        }

        // Add new reservation
        JsonArray reservations = doc.as<JsonArray>();
        JsonObject reservation = reservations.createNestedObject();
        reservation["name"] = name;
        reservation["lastName"] = lastName;
        reservation["email"] = email;
        reservation["id"] = id;
        reservation["date"] = date;

        // Write updated reservations
        file = SPIFFS.open(reservationFile, "w");
        if (!file) {
            Serial.println("Failed to open file for writing");
            request->send(500, "text/plain", "Internal Server Error");
            return;
        }
        serializeJson(doc, file);
        file.close();

        // Send back the HTML page from confirmreservation.h
        request->send(200, "text/html", confirmReservationPage);
    } else {
        request->send(405, "text/plain", "Method Not Allowed");
    }
}
/********************************************************************************************************************************************/
// Display reservations
void handleGetReservations(AsyncWebServerRequest *request) {
    DynamicJsonDocument doc(2048);
    File file = SPIFFS.open(reservationFile, "r");
    if (file) {
        deserializeJson(doc, file);
        file.close();
    }

    String rows;
    JsonArray reservations = doc.as<JsonArray>();
    for (JsonObject reservation : reservations) {
        rows += "<tr>"
                "<td>" + reservation["name"].as<String>() + "</td>"
                "<td>" + reservation["lastName"].as<String>() + "</td>"
                "<td>" + reservation["email"].as<String>() + "</td>"
                "<td>" + reservation["id"].as<String>() + "</td>"
                "<td>" + reservation["date"].as<String>() + "</td>"
                "</tr>";
    }

    String response = reservationsHtmlContent;
    response.replace("%RESERVATION_ROWS%", rows);
    request->send(200, "text/html", response);
}
/********************************************************************************************************************************************/
// Handle file upload
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
/********************************************************************************************************************************************/
// Function to clear the reservation file in the flash of the ESP32
void clearReservationFile() {
    // Open the reservation file for writing (this will truncate the file to zero length)
    File file = SPIFFS.open(reservationFile, FILE_WRITE);
    
    if (file) {
        // Successfully opened the file; truncate it (effectively clearing it)
        file.close();  // Close the file to save changes
        
        Serial.println("Reservation file cleared successfully.");
    } else {
        // Failed to open the file
        Serial.println("Failed to open reservation file for clearing.");
    }
}
/********************************************************************************************************************************************/
// Initialize NTP client for time synchronization
void initTime() {
    // Initialize NTP Client to get time from the network
    configTime(tzOffset, 0, "pool.ntp.org"); // Configure time with NTP server
}

/**********************************************************************************************************************************/
// Functions to store a string into EEPROM
void storeString(int startingAddress, String data) {
  byte len = data.length(); // Get the length of the string

  EEPROM.write(startingAddress, len); // Write the length of the string
  
  for (int i = 0; i < len; i++) {
    EEPROM.write(startingAddress + 1 + i, data[i]); // Write each character of the string
  }

  EEPROM.commit(); // Commit the changes to EEPROM
  delay(1000);
  //LoadStoredParameter();
}
/**********************************************************************************************************************************/
// Functions to read a string from EEPROM
String readString(int addrOffset) {
  int newStrLen = EEPROM.read(addrOffset); // Read the length of the stored string
  char data[newStrLen + 1]; // Create a character array to hold the string data

  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i); // Read each character of the string
  }
  data[newStrLen] = '\0'; // Null-terminate the character array

  return String(data); // Convert the character array to a String object
}

/**********************************************************************************************************************************/
// Function to initialize EEPROM if not already initialized
void initEEPROM() {
  while (!isEEPROMInitialized) {
    if (EEPROM.begin(EEPROM_SIZE)) {
      isEEPROMInitialized = true;
      Serial.println("EEPROM initialized successfully.");
    } else {
      Serial.println("EEPROM initialization failed. Retrying...");
      delay(1000); // Wait for 1 second before retrying
    }
  }
 }
 /**********************************************************************************************************************************/
 // Function to load stored parameters from EEPROM
void LoadStoredParameter() {
  Serial.println("Executing LoadStoredParameter");
  WIFI_SSID = readString(WIFI_SSID_ADD);
  WIFI_PASS = readString(WIFI_PASS_ADD);
  Serial.println(" Done Executing LoadStoredParameter");

 }

 /**********************************************************************************************************************************/
bool connectToWiFi() {
  uint8_t maxRetries = 5;
  // Turn on the WiFi module and set to station mode
  Serial.println("Enabling WiFi STA...");
  WiFi.mode(WIFI_STA);//turnon the wifi module
  delay(1000);
  for (int attempt = 0; attempt < maxRetries; attempt++) {
    WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());
    Serial.print("Connecting to WiFi");
    int retries = 0;
    
    while (WiFi.status() != WL_CONNECTED && retries < 20) {
      delay(500);
      Serial.print(".");
      retries++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected!");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
      return true;
    } else {
      Serial.println("Failed to connect to WiFi. Retrying...");     
    }
  }
  Serial.println("Failed to connect to WiFi after multiple attempts. starting AP mode...");
  startAPMode();
  //esp_restart();//restart esp32
  return false;
}

/**********************************************************************************************************************************/
void startAPMode() {
  WiFi.softAP(AP_SSID, AP_PASS);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = String(apmodehtmlContent);
    html.replace("%SSID%", WIFI_SSID);
    html.replace("%PASS%", WIFI_PASS);

    request->send(200, "text/html", html);
  });

 server.on("/get", HTTP_POST, [](AsyncWebServerRequest *request) {
   
    // Check if the parameters are present
    if (request->hasParam("ssid", true) && request->hasParam("pass", true)) {
        
        Serial.println("Saving parameters!");
        delay(1000);
        storeString(WIFI_SSID_ADD,   request->getParam("ssid", true)->value());
        storeString(WIFI_PASS_ADD,   request->getParam("pass", true)->value());
        Serial.println("Parameters saved.");
        delay(1000);
        Serial.println("Restarting the device!");
        delay(1000);
        esp_restart();//restart esp32
    }    
    // Send a response back to the client
    request->send(200, "text/html", "Settings Saved. ESP32 is restarting...");
 });
  server.begin();
 }
