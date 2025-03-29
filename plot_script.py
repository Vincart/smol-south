import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import re
import time

# Serielle Verbindung konfigurieren
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

# Datenpuffer für Plot
samples = []
baro_values = []

# Maximale Anzahl an Samples (z. B. 600 für 600 letzte Messungen)
max_samples = 600

# Plot initialisieren
fig, ax = plt.subplots()
line, = ax.plot([], [], lw=2)
ax.set_ylim(-1, 10)
ax.set_ylabel("Baro Alt [m]")
ax.set_xlabel("Sample")

# Regex zum Extrahieren der Baro_Alt-Zahl
baro_regex = re.compile(r"Baro_Alt:\s*(-?\d+\.\d+)")

# Offset initialisieren
baro_offset = None

# Warten auf erste gültige Messung zum Setzen des Offsets
while baro_offset is None:
    if ser.in_waiting:
        line_raw = ser.readline().decode(errors='ignore').strip()
        print("Offset-Erkennung:", line_raw)
        match = baro_regex.search(line_raw)
        if match:
            value = float(match.group(1))
            if value >= -5.0:
                baro_offset = value
                print(f"Offset gesetzt auf: -{baro_offset:.2f}")


def update(frame):
    global samples, baro_values

    if ser.in_waiting:
        line_raw = ser.readline().decode(errors='ignore').strip()
        print(line_raw)  # Terminalausgabe

        match = baro_regex.search(line_raw)
        if match:
            value = float(match.group(1))
            if value >= -5.0:
                baro_values.append(value - baro_offset)

    # Nur die letzten max_samples behalten
    baro_values = baro_values[-max_samples:]
    samples = list(range(len(baro_values)))

    # Plotdaten aktualisieren
    ax.clear()
    ax.set_ylim(-1, 7)
    ax.set_xlim(0, max_samples)
    ax.set_xlabel("Sample")
    ax.set_ylabel("Baro Alt [m]")
    ax.plot(samples, baro_values)
    ax.set_title("Live Baro Alt")

ani = animation.FuncAnimation(fig, update, interval=100)
plt.show()
