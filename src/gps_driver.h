#ifndef GPS_DRIVER_H
#define GPS_DRIVER_H

#include <Arduino.h>
#include <HardwareSerial.h>

// UART2 für GPS (Pins anpassen falls nötig)
#define GPS_SERIAL_NUM 2
#define GPS_TX_PIN 17
#define GPS_RX_PIN 16

// GPS-Funktionsprototypen
void gps_init();
void gps_update();
const char* gps_get_latitude();
const char* gps_get_longitude();
const char* gps_get_altitude();

#endif // GPS_DRIVER_H
