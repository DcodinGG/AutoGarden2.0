#include <Arduino.h>
#include <DHT.h>
#include <insert.h>
#include "soc/soc.h"          // For access to hardware registers
#include "soc/rtc_cntl_reg.h" // For the brownout register
#include <esp_sleep.h>
#define DHTPIN 4     // Pin where the DHT22 is connected 
#define DHTTYPE DHT22   // DHT 22
DHT dht(DHTPIN, DHTTYPE); // Create the DHT instance

// Read and return the moistures
int* readMoistures() {
    // Define the pins where the sensors are connected
    const int MoisSens1 = 32; // Moisture sensor number 1 connected to pin 32
    const int MoisSens2 = 33; // Moisture sensor number 2 connected to pin 33
    const int MoisSens3 = 34; // Moisture sensor number 3 connected to pin 34

    static int mappedValues[3]; // Array to hold mapped values for three sensors

    // Read, map, and constrain the analog values for each sensor
    mappedValues[0] = constrain(map(analogRead(MoisSens1), 0, 4095, 0, 10), 0, 10);
    mappedValues[1] = constrain(map(analogRead(MoisSens2), 0, 4095, 0, 10), 0, 10);
    mappedValues[2] = constrain(map(analogRead(MoisSens3), 0, 4095, 0, 10), 0, 10);

    Serial.printf("Moisture 1= %d \nMoisture 2 = %d\nMoisture 3 = %d\n", mappedValues[0], mappedValues[1], mappedValues[2]); // Print moistures
    
    return mappedValues; // Return the array of mapped values
}

// Watering the plant if necessary
int* hasBeenWatered(int* moistures) {
    float wateringTime = 10; // Seconds with the valves open
    int trigger = 3; // Moisture level necessary to activate the valves
    int valves[3] = {5, 6, 7}; // Pins where the valves are connected
    static int hasBeenWatered[3] = {0, 0, 0}; // Array to store the watering status

    for (int i = 0; i < 3; i++) { // Iterate over each moisture sensor
        if (moistures[i] < trigger) {
            digitalWrite(valves[i], HIGH); // Open the corresponding valve
            delay(1000 * wateringTime); // Keep it open for the watering time
            digitalWrite(valves[i], LOW); // Close the valve
            hasBeenWatered[i] = 1; // Update the watering status
        } else {
            hasBeenWatered[i] = 0; // Update the watering status
        }
    }
     // Print the watering status for each sensor
    Serial.print("Watered status for sensor 1: ");
    Serial.println(hasBeenWatered[0]); // Print status for sensor 1
    Serial.print("Watered status for sensor 2: ");
    Serial.println(hasBeenWatered[1]); // Print status for sensor 2
    Serial.print("Watered status for sensor 3: ");
    Serial.println(hasBeenWatered[2]); // Print status for sensor 3
    return hasBeenWatered; // Return the watering status
}

// Read and return the weather
float* readWeather() { 
    static float values[2]; // Array to hold the weather values (humidity, temperature)

    values[0] = dht.readHumidity(); // Read garden humidity
    Serial.printf("Humidity = %.2f%%\n", values[0]); // Print humidity with 2 decimal places

    values[1] = dht.readTemperature(); // Read garden temperature
    Serial.printf("Temperature = %.2f°C\n", values[1]); // Print temperature with 2 decimal places

    return values;
}

// Sleep for 1h
void mimir() {
    esp_sleep_enable_timer_wakeup(1 * 3600000000); // Sleep for 1h
    Serial.print("To sleep for 1h");
    digitalWrite(2, LOW); // Blue LED OFF when sleeping
    esp_deep_sleep_start(); // Go to sleep
}

void setup() {
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Disable brownout detector
    Serial.begin(115200);
    dht.begin();
    pinMode(2, OUTPUT); // Pin for sleep mode
    pinMode(3, OUTPUT); // Pin for valve 1
    pinMode(4, OUTPUT); // Pin for valve 2
    pinMode(5, OUTPUT); // Pin for valve 3
    digitalWrite(2, LOW); // Blue LED ON when NOT sleeping
}

void loop() {
    digitalWrite(2, HIGH); // Blue LED on when running

    int* moistures = readMoistures(); // Read moistures
    
    int* watered = hasBeenWatered(moistures); // Check if watering is needed

    float* weather = readWeather(); // Read weather

    insertData(moistures, watered, weather); // Insert data to DB

    mimir(); // ESP32 sleeps for 1h
}
