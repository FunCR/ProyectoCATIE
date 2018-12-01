#include <SPI.h>
#include <SD.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>
#include <LIDARLite.h>

float myVal = 3.1415;

//GPS
#include "Ublox.h"
#define SERIAL_BAUD 115200
#define GPS_BAUD 9600
#define N_FLOATS 4

Ublox M8_Gps;
// Altitude - Latitude - Longitude - N Satellites
float gpsArray[N_FLOATS] = {0, 0, 0, 0};
//GPS


//Lidar
LIDARLite myLidarLite;

//BAROMETRO
Adafruit_BMP085 bmp;

// SD CARD Set the pins used
#define cardSelect 4

File logfile;

char filename[15];

// blink out an error code
void error(uint8_t errno) {
  while(1) {
    uint8_t i;
    for (i=0; i<errno; i++) {
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
      delay(100);
    }
    for (i=errno; i<10; i++) {
      delay(200);
    }
  }
}

void setup() {
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(SERIAL_BAUD);
  Serial1.begin(GPS_BAUD);
  // Serial.println("\r\nAnalog logger test");
  pinMode(13, OUTPUT);

  //LIDAR
  myLidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz
  myLidarLite.configure(0); // Change this number to try out alternate configurations

  if (!bmp.begin()) {
  // Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    error(2);
  }

  // see if the card is present and can be initialized:
  if (!SD.begin(cardSelect)) {
     Serial.println("Card init. failed!");
    error(2);
  }

  strcpy(filename, "CATIE_00.TXT");
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = '0' + i/10;
    filename[7] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }

  logfile = SD.open(filename, FILE_WRITE);
  if( ! logfile ) {
    // Serial.print("Couldnt create "); 
    // Serial.println(filename);
    error(3);
  }
  // Serial.print("Writing to "); 
  // Serial.println(filename);

  pinMode(13, OUTPUT);
  pinMode(8, OUTPUT);
  // Serial.println("Ready!");

}

uint8_t i=0;
void loop() {
  String dataString = "";
  
  while(Serial1.available()){
        char c = Serial1.read();
        //Serial.println(c);
         if (M8_Gps.encode(c)) {
          gpsArray[0] = M8_Gps.altitude;
          gpsArray[1] = M8_Gps.latitude;
          gpsArray[2] = M8_Gps.longitude;
           
          gpsArray[3] = M8_Gps.sats_in_use;

        }
  }

  //LEER BAROMETRO
  //dataString = bmp.readTemperature();
  //dataString += ",";
//  dataString += bmp.readPressure();
//  dataString += ",";
  dataString += String(gpsArray[1],10);
  dataString += ",";
  dataString += String(gpsArray[2],10);
  dataString += ",";
  dataString += String(gpsArray[0],10);
  dataString += ",";
  dataString +=bmp.readAltitude();
  dataString += ",";
//  dataString += bmp.readSealevelPressure();
//  dataString += ",";
//  dataString += bmp.readAltitude(101500);
//  dataString += ",";
  dataString += myLidarLite.distance(false);
  dataString += ",";
  dataString += gpsArray[3];
  
  digitalWrite(8, HIGH);

  //Guardamos datos
  logfile = SD.open(filename, FILE_WRITE);
  
  logfile.println(dataString);
  logfile.close();
  Serial.println(dataString);

}

