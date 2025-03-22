#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    char telemetry[3];
    int message_counter;
} struct_message;

// Create a struct_message called myData
struct_message myData;
void readMacAddress(){
    uint8_t baseMac[6];
    esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
    if (ret == ESP_OK) {
        Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                      baseMac[0], baseMac[1], baseMac[2],
                      baseMac[3], baseMac[4], baseMac[5]);
    } else {
        Serial.println("Failed to read MAC address");
    }
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    memcpy(&myData, incomingData, sizeof(myData));
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print("Latitude ");
    Serial.println(myData.telemetry[0]);
    Serial.print("Longitude ");
    Serial.println(myData.telemetry[1]);
    Serial.print("Altitude ");
    Serial.println(myData.telemetry[2]);
    Serial.print("Counter ");
    Serial.println(myData.message_counter);
    Serial.println("---------------------");
}

void setup() {
    // Initialize Serial Monitor
    Serial.begin(115200);

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);
    delay(1000);
    Serial.print("[DEFAULT] ESP32 Board MAC Address: ");
    readMacAddress();

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Once ESPNow is successfully Init, we will register for recv CB to
    // get recv packer info
    esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {

}
