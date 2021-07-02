#include <Wire.h> //I2C Arduino Library
// //I2C Address for The HMC5883



#define QMC5883L_ADDR 0x0D

/* Register numbers */
#define QMC5883L_X_LSB 0
#define QMC5883L_X_MSB 1
#define QMC5883L_Y_LSB 2
#define QMC5883L_Y_MSB 3
#define QMC5883L_Z_LSB 4
#define QMC5883L_Z_MSB 5
#define QMC5883L_STATUS 6
#define QMC5883L_TEMP_LSB 7
#define QMC5883L_TEMP_MSB 8
#define QMC5883L_CONFIG 9
#define QMC5883L_CONFIG2 10
#define QMC5883L_RESET 11
#define QMC5883L_RESERVED 12
#define QMC5883L_CHIP_ID 13

/* Bit values for the STATUS register */
#define QMC5883L_STATUS_DRDY 1
#define QMC5883L_STATUS_OVL 2
#define QMC5883L_STATUS_DOR 4

/* Oversampling values for the CONFIG register */
#define QMC5883L_CONFIG_OS512 0b00000000
#define QMC5883L_CONFIG_OS256 0b01000000
#define QMC5883L_CONFIG_OS128 0b10000000
#define QMC5883L_CONFIG_OS64  0b11000000

/* Range values for the CONFIG register */
#define QMC5883L_CONFIG_2GAUSS 0b00000000
#define QMC5883L_CONFIG_8GAUSS 0b00010000

/* Rate values for the CONFIG register */
#define QMC5883L_CONFIG_10HZ   0b00000000
#define QMC5883L_CONFIG_50HZ   0b00000100
#define QMC5883L_CONFIG_100HZ  0b00001000
#define QMC5883L_CONFIG_200HZ  0b00001100

/* Mode values for the CONFIG register */
#define QMC5883L_CONFIG_STANDBY 0b00000000
#define QMC5883L_CONFIG_CONT    0b00000001
/* Apparently M_PI isn't available in all environments. */
#define M_PI 3.14159265358979323846264338327950288

uint8_t addr = 0x0D;
int16_t xhigh, xlow;
int16_t yhigh, ylow;
//uint8_t addr;
uint8_t mode;
uint8_t rate;
uint8_t range;
uint8_t oversampling;
//const int16_t I2C_MASTER = 0x42;
void reconfig()
{
  write_register(addr, QMC5883L_CONFIG, oversampling|range|rate|mode);
}
void reset()
{
  write_register(addr, QMC5883L_RESET, 0x01);
  reconfig();
}
static void write_register( int addr, int reg, int value )
{
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

static int read_register( int addr, int reg, int count )
{
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(addr, count);
  int n = Wire.available();
  if (n != count) return 0;

  return n;
}
void init1() {
  /* This assumes the wire library has been initialized. */
  addr = QMC5883L_ADDR;
  oversampling = QMC5883L_CONFIG_OS512;
  range = QMC5883L_CONFIG_2GAUSS;
  rate = QMC5883L_CONFIG_50HZ;
  mode = QMC5883L_CONFIG_CONT;
  reset();
}
void setup() {

  Serial.begin(115200);
  Serial.print("HMC5833L COMPASS SENSOR BEGIN");
  Serial.println();
  // Wire.begin(SDA_PIN, SCL_PIN, I2C_MASTER);


  init1();
  reset();

}
int ready()
{
  if(!read_register(addr,QMC5883L_STATUS,1)) return 0;
  uint8_t status = Wire.read();
  return status & QMC5883L_STATUS_DRDY; 
}
int readRaw( int16_t *x, int16_t *y, int16_t *z, int16_t *t )
{
  while(!ready()) {}

  if(!read_register(addr,QMC5883L_X_LSB,6)) return 0;

  *x = Wire.read() | (Wire.read()<<8);
  *y = Wire.read() | (Wire.read()<<8);
  *z = Wire.read() | (Wire.read()<<8);

  return 1;
}
int readHeading()
{
  int16_t x, y, z, t;

  if (!readRaw(&x, &y, &z, &t)) return 0;

  /* Update the observed boundaries of the measurements */

  if (x < xlow) xlow = x;
  if (x > xhigh) xhigh = x;
  if (y < ylow) ylow = y;
  if (y > yhigh) yhigh = y;

  /* Bail out if not enough data is available. */

  if ( xlow == xhigh || ylow == yhigh ) return 0;

  /* Recenter the measurement by subtracting the average */

  x -= (xhigh + xlow) / 2;
  y -= (yhigh + ylow) / 2;

  /* Rescale the measurement to the range observed. */

  float fx = (float)x / (xhigh - xlow);
  float fy = (float)y / (yhigh - ylow);

  int heading = 180.0 * atan2(fy, fx) / M_PI;
  if (heading <= 0) heading += 360;

  return heading;
}
void loop() {

  int heading = readHeading();
  if (heading == 0) {
    reset();
    /* Still calibrating, so measure but don't print */
  } else {
    Serial.println(heading);
  }
  delay(100);
}

