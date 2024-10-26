#include "insert.h"        // Include the header file for database operations
#include "credentials.h"   // Include the credentials for the database and WiFi
#include <ESP32_MySQL.h>   // Include the ESP32 MySQL library
#include <WiFi.h>         // Include the WiFi library for network connectivity

#define _ESP32_MYSQL_LOGLEVEL_ 4 // Debugging level from 0 (off) to 4 (verbose)
#define ESP32_MYSQL_DEBUG_PORT Serial // Define the port for MySQL debug messages

// Create an instance of ESP32_MySQL_Connection
ESP32_MySQL_Connection conn((Client *)&client);
ESP32_MySQL_Query *query_mem; // Pointer for MySQL query object

// Function to insert data into the database
void insertData(int* moistures, int* watered, float* weather) {
    // Connect to WiFi
    ESP32_MYSQL_DISPLAY1("Connecting to", ssid);
    WiFi.begin(ssid, pass); // Begin WiFi connection with credentials
    while (WiFi.status() != WL_CONNECTED) {
        delay(500); // Wait until connected
        ESP32_MYSQL_DISPLAY0("."); // Display a dot for each connection attempt
    }

    // Connected to WiFi
    ESP32_MYSQL_DISPLAY1("Connected to network. My IP address is:", WiFi.localIP());
    ESP32_MYSQL_DISPLAY3("Connecting to SQL Server @", server, ", Port =", server_port);
    ESP32_MYSQL_DISPLAY5("User =", user, ", PW =", password, ", DB =", default_database);

    ESP32_MYSQL_DISPLAY("Connecting...");

    // Attempt to connect to the database
    if (conn.connectNonBlocking(server, server_port, user, password, default_database) != RESULT_FAIL) {
        delay(50); // Short delay after connection attempt

        // Create an instance of the MySQL query class
        ESP32_MySQL_Query query_mem = ESP32_MySQL_Query(&conn);

        if (conn.connected()) {
            ESP32_MYSQL_DISPLAY("Database connected. Inserting data...");

            // Declare a buffer for the SQL query
            char query[512];
            
            // Format the SQL query using snprintf
            snprintf(query, sizeof(query),
                "INSERT INTO weather (humidity, temperature) VALUES (%0.1f, %0.1f);"
                "INSERT INTO moistures (moisture, plant_id) VALUES (%i, 1), (%i, 2), (%i, 3);"
                "INSERT INTO watering (watered, plant_id) VALUES (%i, 1), (%i, 2), (%i, 3);",
                weather[0], weather[1], // Cast humidity to int
                moistures[0], moistures[1], moistures[2],
                watered[0], watered[1], watered[2]);
                
            delay(5000); // Delay before executing the query

            // Execute the SQL query
            if (!query_mem.execute(query)) {
                ESP32_MYSQL_DISPLAY("Insert error"); // Display error message if insert fails
            } else {
                ESP32_MYSQL_DISPLAY("Data Inserted."); // Confirm successful insertion
            }
        } else {
            ESP32_MYSQL_DISPLAY("Error connecting to Database. Can't insert."); // Display error if not connected
        }

        // Close the connection to the database
        conn.close();
    } else {
        ESP32_MYSQL_DISPLAY("\nConnect failed. Trying again on next iteration."); // Notify connection failure
    }

    // Prepare for sleep after the operation
    ESP32_MYSQL_DISPLAY("\nSleeping...");
    ESP32_MYSQL_DISPLAY("================================================");
}
