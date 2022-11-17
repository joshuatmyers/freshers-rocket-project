#include <Adafruit_MPU6050.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
Adafruit_MPU6050 mpu;
Adafruit_BMP085 bmp;
#define seaLevelPressure_hPa 1013.25//input here
int buzzer = 11;
#include <Adafruit_Sensor.h>

File myFile;

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
  /*if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {
      delay(10);
    }
  }*/
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
  
  myFile.print("Acceleration X: ");
	myFile.print(a.acceleration.x);
	myFile.print(", Y: ");
	myFile.print(a.acceleration.y);
	myFile.print(", Z: ");
	myFile.print(a.acceleration.z);
	myFile.println(" m/s^2");

	myFile.print("Rotation X: ");
	myFile.print(g.gyro.x);
	myFile.print(", Y: ");
	myFile.print(g.gyro.y);
	myFile.print(", Z: ");
	myFile.print(g.gyro.z);
	myFile.println(" rad/s");

	myFile.print("Temperature: ");
	myFile.print(temp.temperature);
	myFile.println(" degC");

	myFile.println("");
	delay(100);

  /* 
  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");
  
  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  Serial.print("Altitude = ");
  Serial.print(bmp.readAltitude());
  Serial.println(" meters");

  Serial.print("Pressure at sealevel (calculated) = ");
  Serial.print(bmp.readSealevelPressure());
  Serial.println(" Pa");

  Serial.print("Real altitude = ");
  Serial.print(bmp.readAltitude(seaLevelPressure_hPa * 100));
  Serial.println(" meters");
  
  Serial.println();
  delay(500);
  */
  
  
}
