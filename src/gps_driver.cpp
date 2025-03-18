#include "gps_driver.h"

HardwareSerial MySerial(GPS_SERIAL_NUM);

static char latitude[15] = "N/A";
static char longitude[15] = "N/A";
static char altitude[10] = "N/A";

const char* CMD_BAUD_115200 = "$PMTK251,115200*1F\r\n";
const char* CMD_ENABLE_GGA_RMC = "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n";
const char* CMD_10HZ_UPDATE = "$PMTK220,100*2F\r\n";

void gps_init() {
    Serial.begin(115200);
    MySerial.begin(9600, SERIAL_8N1, GPS_TX_PIN, GPS_RX_PIN);

    delay(500);
    MySerial.print(CMD_BAUD_115200);
    delay(500);

    MySerial.flush();
    MySerial.end();
    MySerial.begin(115200, SERIAL_8N1, GPS_TX_PIN, GPS_RX_PIN);

    MySerial.print(CMD_ENABLE_GGA_RMC);
    delay(100);
    MySerial.print(CMD_10HZ_UPDATE);
    delay(100);

    Serial.println("\n🚀 GPS initialisiert: 115200 Baud, 10Hz, GGA + RMC aktiviert\n");
}

void gps_update() {
    if (MySerial.available()) {
        String sentence = MySerial.readStringUntil('\n');
        if (sentence.startsWith("$GPGGA")) {
            int commaIndex[15];
            int index = 0;
            for (int i = 0; i < sentence.length(); i++) {
                if (sentence[i] == ',') {
                    commaIndex[index++] = i;
                    if (index >= 15) break;
                }
            }

            if (index >= 9) {
                sentence.substring(commaIndex[1] + 1, commaIndex[2]).toCharArray(latitude, 15);
                sentence.substring(commaIndex[3] + 1, commaIndex[4]).toCharArray(longitude, 15);
                sentence.substring(commaIndex[8] + 1, commaIndex[9]).toCharArray(altitude, 10);
            }
        }
    }
}

const char* gps_get_latitude() {
    return latitude;
}

const char* gps_get_longitude() {
    return longitude;
}

const char* gps_get_altitude() {
    return altitude;
}
