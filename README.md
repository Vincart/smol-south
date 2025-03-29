# Smol South
Educational South Rocket Model and telemetry for "Tag der Raumfahrt". Uses ESP32 for 802.11 based connectivity.

## Steps to reproduce
1. Gather 2 ESP32 dev modules or nodeMCU
2. Hook up BMP180 and Quecktel GPS (or similar NMEA), add battery
    2.1. you might need to change the LDO to a pin compatible LDO with lower dropout to directly use it with the battery
3. Flash firmware (receiver code in `receiver` branch)
4. Print 3D cones and tape on empty toilet roll
5. mount on pressure-operated potato cannon
6.

## Receiver setup
1. Connect receiver ESP32 to laptop via USB and start python script. You might need to change the serial device in the script.
