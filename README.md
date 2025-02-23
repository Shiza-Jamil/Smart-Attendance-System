# Smart-Attendance-System
This project is designed to automate the attendance marking process for students or employees using RFID cards. Using the ESP32 microcontroller, MFRC522 RFID module, and an LCD, the system marks attendance when an authorized RFID card is scanned and displays the status on the screen.

## System Requirements:
1.	ESP32 Microcontroller – Serves as the brain of the system, controlling all the operations.

2.	MFRC522 RFID Module – This module reads the UID of RFID cards.

3.	RFID Cards (or Tags) – Used for identification. Each student or employee is assigned a unique RFID card.

4.	16x2 I2C LCD Display – Displays information like "Place RFID card near the reader", attendance status, etc.

5.	Jumper Wires and Breadboard – For connecting the components.


## Hardware Connections:

### 1.	MFRC522 RFID Module to ESP32:

o	SDA (SS) → GPIO 21

o	SCK (Clock) → GPIO 18

o	MOSI (Master Out Slave In) → GPIO 23

o	MISO (Master In Slave Out) → GPIO 19

o	RST (Reset) → GPIO 22

o	GND → GND

o	3.3V → 3.3V

### 2.	LCD I2C to ESP32:

o	SDA → GPIO 21

o	SCL → GPIO 22

o	VCC → 3.3V

o	GND → GND
