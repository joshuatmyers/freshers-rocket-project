#include <Adafruit_MPU6050.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;
Adafruit_BMP085 bmp;

#define seaLevelPressure_hPa 1013.25//input here

int buzzer = 11;
int maxHeight = 0;

bool descent = False;

File myFile;
// change to match SD shield or module
const int chipSelect = 10;


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  SD.begin(4); // change pin later to match with the SD card reading
  Wire.setClock(400000);
  Serial.begin(115200);
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
  }
  Serial.println("MPU6050 Found!");
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {
      delay(10);
    }
  }
  Serial.println("BMP180 Found!");
  // Open serial communications and wait for port to open:
 
 
  Serial.print("Initializing SD card...");
  if (!SD.begin()) {
    Serial.println("initialization failed!");
    return;
  }

  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  Serial.println("initialization done.");

  // set accelerometer range to +-16G
	mpu.setAccelerometerRange(MPU6050_RANGE_16_G);

	// set gyro range to +- 500 deg/s
	mpu.setGyroRange(MPU6050_RANGE_500_DEG);

	// set filter bandwidth to 21 Hz
	mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // opens the file so that it can be written to during operation
  myFile = SD.open("test.txt", FILE_WRITE);
 
}

void loop() {
  // put your main code here, to run repeatedly:
  // bpm reading methods
  
  sensors_event_t a, g, temp;
	mpu.getEvent(&a, &g, &temp);
  while(descent=False) {
      
    //myFile.print("Acceleration X: ");  in m/s^2
    myFile.print(a.acceleration.x);
    myFile.print(",");
    myFile.print(a.acceleration.y);
    myFile.print(",");
    myFile.print(a.acceleration.z);
    myFile.print(",");

    //myFile.print("Rotation X: "); in rad/s
    myFile.print(g.gyro.x);
    myFile.print(",");
    myFile.print(g.gyro.y);
    myFile.print(",");
    myFile.print(g.gyro.z);
    myFile.print(",");

    //myFile.print("Temperature: "); in degrees celcius
    myFile.print(temp.temperature);
    myFile.print(",");
    myFile.print(bmp.readAltitude()); // altitude
    myFile.print(",");
    myFile.print(bmp.readAltitude(seaLevelPressure_hPa * 100)); // real altitude
    myFile.print(",");
    myFile.print(bmp.readPressure()); // pressure in Pa
    myFile.print(",");
    myFile.print(bmp.readSealevelPressure()); // pressure at sea level

    myFile.println();
    delay(100);

    // Check whether rocket has started descending
    if(bmp.readAltitude() > maxHeight) {
      maxHeight = bmp.readAltitude();
    }
    if(bmp.readAltitude() < maxHeight - 10) {
      myFile.close();
      descent=True;
      // turn on buzzer and/or change boolean to stop writing and close file
    }
    delay(500);
  }
  // turn on buzzer here

}
