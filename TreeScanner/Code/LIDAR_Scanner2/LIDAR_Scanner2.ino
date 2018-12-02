#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>
#include <LIDARLite.h>
#include <Servo.h>

#define ACCEL 150
#define SERVOPIN 13


/* Assign a unique ID to the sensors */
Adafruit_9DOF                dof   = Adafruit_9DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);

/* Update this with the correct SLP for accurate altitude measurements */
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

Servo servo;
LIDARLite myLidarLite;

/**************************************************************************/
/*!
    @brief  Initialises all the sensors used by this example
*/
/**************************************************************************/
void initSensors()
{
  if (!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while (1);
  }
  if (!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while (1);
  }
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void setup(void)
{
  Serial.begin(9600);
  Serial.println(F("Adafruit 9 DOF Pitch/Roll/Heading Example")); Serial.println("");

  /* Initialise the sensors */
  initSensors();
  //setMotors();
  myLidarLite.begin(0, true);
  myLidarLite.configure(5);
  servo.attach(SERVOPIN);
  pinMode(5, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(5, HIGH);
  digitalWrite(10, LOW);

}

/**************************************************************************/
/*!
    @brief  Constantly check the roll/pitch/heading/altitude/temperature
*/
/**************************************************************************/
void loop(void)
{
  servo.write(0);
  sensors_event_t mag_event;
  sensors_vec_t   orientation;


  /* Calculate the heading using the magnetometer */
  mag.getEvent(&mag_event);

  if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
  {
    /* 'orientation' should have valid .heading data now */
    Serial.print(orientation.heading);
    Serial.print(F(", "));
  }

  Serial.println(myLidarLite.distance());
  delay(100);
}
