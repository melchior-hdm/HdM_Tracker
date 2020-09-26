#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>


/* This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
   which provides a common 'type' for sensor data and some helper functions.

   To use this driver you will also need to download the Adafruit_Sensor
   library and include it in your libraries folder.

   You should also assign a unique ID to this sensor for use with
   the Adafruit Sensor API so that you can identify this particular
   sensor in any data logs, etc.  To assign a unique ID, simply
   provide an appropriate value in the constructor below (12345
   is used by default in this example).
*/

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (10)

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

/**************************************************************************/
/*
    Display sensor calibration status
*/
/**************************************************************************/
void displayCalStatus(void)
{
  /* Get the four calibration values (0..3) */
  /* Any sensor data reporting 0 should be ignored, */
  /* 3 means 'fully calibrated" */
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  /* The data should be ignored until the system calibration is > 0 */

  /* Display the individual values */
  Serial.print("Sys:");
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.print(mag, DEC);
  /* Display system calibration status */
  Serial.print(", ");
  Serial.print(system, DEC);
}

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
  Serial.begin(115200);
  Serial.println("Orientation Sensor Test"); Serial.println("");
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  bno.setExtCrystalUse(true);
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete
*/
/**************************************************************************/
void loop(void)
{

  /* Display the floating point data */
imu::Quaternion quat = bno.getQuat();

imu::Quaternion quatTmp ;
 char imu_data[64];
 char qW[8], qX[8], qY[8], qZ[8];

// Modification of orientation according to mounting   
quatTmp.w() = quat.w();
quatTmp.x() = quat.y();
quatTmp.y() = -quat.x();
quatTmp.z() = quat.z();
 
 dtostrf(quatTmp.w(), 7, 4, qW);
 dtostrf(quatTmp.x(), 7, 4, qX);
 dtostrf(quatTmp.y(), 7, 4, qY);
 dtostrf(quatTmp.z(), 7, 4, qZ);   
      
 strcpy(imu_data,qW);
 strcat(imu_data,",");
 strcat(imu_data,qX);
 strcat(imu_data,",");
 strcat(imu_data,qY);
 strcat(imu_data,",");
 strcat(imu_data,qZ);

 Serial.write(imu_data);
 Serial.write(";");

  /* Optional: Display calibration status */
  //displayCalStatus();

  /* Wait the specified delay before requesting nex data */
  delay(BNO055_SAMPLERATE_DELAY_MS);
}
