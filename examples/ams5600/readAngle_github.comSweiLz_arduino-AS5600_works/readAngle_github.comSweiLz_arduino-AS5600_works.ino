#include <AS5600.h>

AS5600 encoder;

int ang, lang = 0;

void setup()
{
  Serial.begin(115200);
}

void loop()
{

  ang = encoder.getAngle();

  if (ang != lang)
  {
    Serial.println(ang);
    lang = ang;
  }
}
