#include <Adafruit_MPU6050.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_Sensor.h>
#include <EEPROM.h>


Adafruit_MPU6050 mpu;
Adafruit_BMP085 bmp;
#define seaLevelPressure_hPa 1013.25//input here
int buzzer = 11;

const int READING_INTERVAL = 100; // interval in milliseconds between readings
const int VALUE_SIZE = 2; // size of value in bytes
const int EEPROM_SIZE = 1024; // size of EEPROM in bytes EEPROM.length() returns 1024

int address = 0; // current address in EEPROM

void setup() {
  // put your setup code here, to run once:
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

  pinMode(buzzer, OUTPUT);

  // set accelerometer range to +-16G
	mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

	// set gyro range to +- 500 deg/s
	mpu.setGyroRange(MPU6050_RANGE_500_DEG);

	// set filter bandwidth to 21 Hz
	mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  // put your main code here, to run repeatedly:
  int reading = bmp.readAltitude();

  /*
  Serial.print("Altitude = ");
  Serial.print(reading);
  Serial.println(" meters");

	Serial.println("");
	delay(100);
  */

  if (address + VALUE_SIZE <= EEPROM_SIZE) {
    // split the 16-bit integer into two 8-bit bytes
    byte highByte = highByte(reading);
    byte lowByte = lowByte(reading);

    // write the two bytes to the EEPROM
    EEPROM.write(address, highByte);
    EEPROM.write(address + 1, lowByte);
    EEPROM.update(address, highByte);
    EEPROM.update(address + 1, lowByte);
    Serial.println("writing...");
    address += VALUE_SIZE;
  } else {
    // EEPROM is full
    digitalWrite(buzzer, HIGH);
    EEPROM.end();
  }
  delay(READING_INTERVAL);
  
}
