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

/********************************************************************************************************************************************/
// Handle reservation form submission
void handleReservation(AsyncWebServerRequest *request) {
    if (request->method() == HTTP_POST) {
        String name = request->arg("name");
        String lastName = request->arg("lastName");
        String email = request->arg("email");
        String id = request->arg("id");
        String date = getCurrentDate();

        DynamicJsonDocument doc(1024);

        // Read existing reservations
        File file = SPIFFS.open(reservationFile, "r");
        if (file) {
            deserializeJson(doc, file);
            file.close();
        }

        // Add new reservation
        JsonArray reservations = doc.to<JsonArray>();
        JsonObject reservation = reservations.createNestedObject();
        reservation["name"] = name;
        reservation["lastName"] = lastName;
        reservation["email"] = email;
        reservation["id"] = id;
        reservation["date"] = date;

        // Write updated reservations
        file = SPIFFS.open(reservationFile, "w");
        serializeJson(doc, file);
        file.close();

        request->send(200, "text/html", "Reservation successful! <a href='/'>Go back</a>");
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
