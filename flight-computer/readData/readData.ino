#include <Adafruit_MPU6050.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_Sensor.h>
#include <EEPROM.h>

Adafruit_MPU6050 mpu;
Adafruit_BMP085 bmp;

const int EEPROM_SIZE = 1024; // size of EEPROM in bytes

int address = 0; // current address in EEPROM

void setup() {
  Wire.begin();
  SD.begin(4); // change pin later to match with the SD card reading
  Wire.setClock(400000);
  Serial.begin(9600);
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    }
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {
      delay(10);
    }
  }
}

void loop() {
  // check if there is data in the EEPROM
  if (address + 2 <= EEPROM_SIZE) {
    // read the two bytes from the EEPROM
    byte highByte = EEPROM.read(address);
    byte lowByte = EEPROM.read(address + 1);

    // combine the two bytes back into a 16-bit integer
    int value = (highByte << 8) | lowByte;
    // do something with the data
    Serial.print("Reading: ");
    Serial.println(value);
    address += 2;
  } else {
    Serial.println("Finished");
    
  }
  delay(1000);
}
