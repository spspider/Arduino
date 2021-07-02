#include <QMC5883L.h>
#include <Wire.h>

QMC5883L compass;

void setup()
{
  Wire.begin();

  compass.init();
  compass.setSamplingRate(50);

  Serial.begin(115200);
  Serial.println("QMC5883L Compass Demo");
  Serial.println("Turn compass in all directions to calibrate....");

}

void loop()
{
  int heading = compass.readHeading();
   //int temp = compass.readTemp();
  if (heading == 0) {
      compass.reset();
    /* Still calibrating, so measure but don't print */
  } else {
    Serial.println(heading);
     Serial.print("temp:");
        Serial.println(compass.returnTemp());
  }
}
