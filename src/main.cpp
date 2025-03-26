#include "gps_driver.h"
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

//Connection Baro
//SDA	A4
//SCL	A5

#define MEAN_HUBLAND_SL_ALTITUDE 275 //Calibration Altitude for SL pressure

int counter = 0;

int initial_sea_l_press = -1;
int initial_baro_altitude = -1;

// Receiver MAC Address
uint8_t broadcastAddress[] = {0x9c, 0x9c, 0x1f, 0xc4, 0xc4, 0x94};

typedef struct struct_message {
    int message_counter;
    char latitude[15];
    char longitude[15];
    char altitude[10];
} struct_message;

// Data packages
struct_message myData;

esp_now_peer_info_t peerInfo;

//Baro ->

void setup() {
    Serial.begin(9600);

}

void loop() {
    Serial.print("Temperatur = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print("Luftdruck = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.println();
    delay(2000);
}

// ESP-NOW: callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if(status != ESP_NOW_SEND_SUCCESS) Serial.println("Delivery Fail");
}

void setup() {
    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);
    
    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    
    // Once ESPNow is successfully Init, we will register for Send CB to
    // get the status of Trasnmitted packet
    esp_now_register_send_cb(OnDataSent);
    
    // Register peer
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
    
    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }

    //GPS
    gps_init();

    // Baro init
    if (!bmp.begin()) {
        Serial.println("Sensor not found... Retrying");
        delay(500);
        while (1) {}
    }
    initial_sea_l_press = readSealevelPressure(HUBLAND_SL_ALTITUDE);
    initial_baro_altitude = readAltitude(initial_sea_l_press);
}

void loop() {
    gps_update();
    int baro_altitude = readAltitude(initial_sea_l_press);
    Serial.print("\r🌍 Lat: ");
    Serial.print(gps_get_latitude());
    Serial.print(" | Lon: ");
    Serial.print(gps_get_longitude());
    Serial.print(" | 🏔 Alt: ");
    Serial.print(gps_get_altitude());
    Serial.print("m    BaroDiff: ");
    Serial.print(baro_altitude - initial_baro_altitude);
    Serial.print("m    ");

    // Set values to send
    strcpy(myData.latitude, gps_get_latitude());
    strcpy(myData.longitude, gps_get_longitude());
    strcpy(myData.altitude, gps_get_altitude());

    myData.message_counter = counter;

    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    if (result == ESP_OK) {
        // Serial.println("Sent with success");
    }
    else {
        Serial.println("Error sending the data");
    }

    counter += 1;

    delay(50); // 20Hz Update-Rate
}
