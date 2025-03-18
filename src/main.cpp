#include "gps_driver.h"

void setup() {
    gps_init();
}

void loop() {
    gps_update();

    Serial.print("\r🌍 Lat: ");
    Serial.print(gps_get_latitude());
    Serial.print(" | Lon: ");
    Serial.print(gps_get_longitude());
    Serial.print(" | 🏔 Alt: ");
    Serial.print(gps_get_altitude());
    Serial.print("m    ");

    delay(100); // 10Hz Update-Rate
}
