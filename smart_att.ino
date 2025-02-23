#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Define pins for MFRC522
#define RST_PIN 22      // Reset pin for MFRC522 (GPIO 22)
#define SS_PIN 21       // SDA (SS) pin for MFRC522 (GPIO 21)

// Create MFRC522 instance
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Define the RFID UIDs of two students/employees
byte uid1[] = {0xF0, 0xB1, 0x17, 0x7C};  
byte uid2[] = {0x60, 0x5C, 0xF2, 0x79};  

// Array to store attendance status
bool attendance[] = {false, false};  // false = absent, true = present

// Create LCD object
LiquidCrystal_I2C lcd(0x27, 16, 2);

// WiFi credentials
const char* ssid = "TP-Link_A6F8";
const char* password = "29311545";

// ThingSpeak settings
const char* server = "http://api.thingspeak.com/update";
const char* apiKey = "NLJLEZF4HV79WD2B";

// Function to compare two UIDs
bool compareUID(byte* uid1, byte* uid2) {
  for (byte i = 0; i < 4; i++) {
    if (uid1[i] != uid2[i]) {
      return false;  // UIDs do not match
    }
  }
  return true;  // UIDs match
}

void setup() {
  Serial.begin(115200);  // Start serial communication
  SPI.begin();           // Initialize SPI bus
  mfrc522.PCD_Init();    // Initialize MFRC522

  // Initialize the LCD
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Place RFID card");
  lcd.setCursor(0, 1);
  lcd.print("near the reader");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  lcd.clear();
  lcd.print("WiFi connected");
  delay(2000);
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.print("Card UID: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    lcd.clear();
    lcd.setCursor(0, 0);

    // Compare UID with stored UIDs
    if (compareUID(mfrc522.uid.uidByte, uid1)) {
      attendance[0] = true;
      Serial.println("Attendance marked for Student/Employee 1");
      lcd.print("Student 1 Present");
    } else if (compareUID(mfrc522.uid.uidByte, uid2)) {
      attendance[1] = true;
      Serial.println("Attendance marked for Student/Employee 2");
      lcd.print("Student 2 Present");
    } else {
      Serial.println("Unknown card. Access denied.");
      lcd.print("Unknown card");
    }

    // Update ThingSpeak
    updateThingSpeak();

    delay(2000);
    lcd.clear();
    lcd.print("Scan next card");
    mfrc522.PICC_HaltA();  // Halt the card
  }

  delay(1000);
}

// Function to update ThingSpeak
void updateThingSpeak() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(server) + "?api_key=" + apiKey +
                 "&field1=" + (attendance[0] ? "1" : "0") +
                 "&field2=" + (attendance[1] ? "1" : "0");
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.println("ThingSpeak updated successfully.");
      Serial.println("Response: " + http.getString());
    } else {
      Serial.println("Failed to update ThingSpeak.");
    }
    http.end();
  } else {
    Serial.println("WiFi not connected. Cannot update ThingSpeak.");
  }
}

