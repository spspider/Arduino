/*
Подключения:
NodeMCU    -> Matrix
MOSI-D7-GPIO13  -> DIN
CLK-D5-GPIO14   -> Clk
GPIO0-D3        -> LOAD
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <ArduinoJson.h>

// =======================================================================
// Конфигурация устройства:
// =======================================================================
const char* ssid     = "Musa Pro";                      // SSID
const char* password = "Gigabyte2014";                    // пароль
String weatherKey = "1d230c9a48414be04746d6d5b08a3919";  // Чтобы получить API ключь, перейдите по ссылке http://openweathermap.org/api
String weatherLang = "&lang=ru";
String cityID = "479561"; //Ufa
// =======================================================================


WiFiClient client;

String weatherMain = "";
String weatherDescription = "";
String weatherLocation = "";
String country;
int humidity;
int pressure;
float temp;
float tempMin, tempMax;
int clouds;
float windSpeed;
String date;
String currencyRates;
String weatherString;

long period;
int offset=1,refresh=0;
int pinCS = 0; // Подключение пина CS
int numberOfHorizontalDisplays = 4; // Количество светодиодных матриц по Горизонтали
int numberOfVerticalDisplays = 1; // Количество светодиодных матриц по Вертикали
String decodedMsg;
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);
//matrix.cp437(true);

int wait = 50; // скорость бегущей строки

int spacer = 2;
int width = 5 + spacer; // Регулируем расстояние между символами

void setup(void) {

matrix.setIntensity(13); // Яркость матрицы от 0 до 15


// начальные координаты матриц 8*8
  matrix.setRotation(0, 1);        // 1 матрица
  matrix.setRotation(1, 1);        // 2 матрица
  matrix.setRotation(2, 1);        // 3 матрица
  matrix.setRotation(3, 1);        // 4 матрица


  Serial.begin(115200);                           // Дебаг
  WiFi.mode(WIFI_STA);                           
  WiFi.begin(ssid, password);                         // Подключаемся к WIFI
  while (WiFi.status() != WL_CONNECTED) {         // Ждем до посинения
    delay(500);
    Serial.print(".");
  }



}

// =======================================================================
#define MAX_DIGITS 16
byte dig[MAX_DIGITS]={0};
byte digold[MAX_DIGITS]={0};
byte digtrans[MAX_DIGITS]={0};
int updCnt = 0;
int dots = 0;
long dotTime = 0;
long clkTime = 0;
int dx=0;
int dy=0;
byte del=0;
int h,m,s;
// =======================================================================
void loop(void) {




if(updCnt<=0) { // каждые 10 циклов получаем данные времени и погоды
    updCnt = 10;
    Serial.println("Getting data ...");
    getWeatherData();
    getTime();
    Serial.println("Data loaded");
    clkTime = millis();
  }

  if(millis()-clkTime > 15000 && !del && dots) { //каждые 15 секунд запускаем бегущую строку
//ScrollText(utf8rus("ARDUINO это просто!")); //тут текст строки, потом будет погода и т.д.
ScrollText(utf8rus(weatherString));
    updCnt--;
    clkTime = millis();
  }


  
DisplayTime();
  if(millis()-dotTime > 500) {
    dotTime = millis();
    dots = !dots;
  }


}

// =======================================================================
void DisplayTime(){
    updateTime();
    matrix.fillScreen(LOW);
    int y = (matrix.height() - 8) / 2; // Центрируем текст по Вертикали

    
    if(s & 1){matrix.drawChar(14, y, (String(":"))[0], HIGH, LOW, 1);} //каждую четную секунду печатаем двоеточие по центру (чтобы мигало)
    else{matrix.drawChar(14, y, (String(" "))[0], HIGH, LOW, 1);}
    
    String hour1 = String (h/10);
    String hour2 = String (h%10);
    String min1 = String (m/10);
    String min2 = String (m%10);
    String sec1 = String (s/10);
    String sec2 = String (s%10);
    int xh = 2;
    int xm = 19;
//    int xs = 28;

    matrix.drawChar(xh, y, hour1[0], HIGH, LOW, 1);
    matrix.drawChar(xh+6, y, hour2[0], HIGH, LOW, 1);
    matrix.drawChar(xm, y, min1[0], HIGH, LOW, 1);
    matrix.drawChar(xm+6, y, min2[0], HIGH, LOW, 1);
//    matrix.drawChar(xs, y, sec1[0], HIGH, LOW, 1);
//    matrix.drawChar(xs+6, y, sec2[0], HIGH, LOW, 1);  


  
    matrix.write(); // Вывод на дисплей
}

// =======================================================================
void DisplayText(String text){
    matrix.fillScreen(LOW);
    for (int i=0; i<text.length(); i++){
    
    int letter =(matrix.width())- i * (width-1);
    int x = (matrix.width() +1) -letter;
    int y = (matrix.height() - 8) / 2; // Центрируем текст по Вертикали
    matrix.drawChar(x, y, text[i], HIGH, LOW, 1);
    matrix.write(); // Вывод на дисплей
    
    }

}
// =======================================================================
void ScrollText (String text){
    for ( int i = 0 ; i < width * text.length() + matrix.width() - 1 - spacer; i++ ) {
    if (refresh==1) i=0;
    refresh=0;
    matrix.fillScreen(LOW);
    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // Центрируем текст по Вертикали
 
    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < text.length() ) {
        matrix.drawChar(x, y, text[letter], HIGH, LOW, 1);
      }
      letter--;
      x -= width;
    }
    matrix.write(); // Вывод на дисплей
    delay(wait);
  }
}


// =======================================================================
// Берем погоду с сайта openweathermap.org
// =======================================================================



const char *weatherHost = "api.openweathermap.org";

void getWeatherData()
{
  Serial.print("connecting to "); Serial.println(weatherHost);
  if (client.connect(weatherHost, 80)) {
    client.println(String("GET /data/2.5/weather?id=") + cityID + "&units=metric&appid=" + weatherKey + weatherLang + "\r\n" +
                "Host: " + weatherHost + "\r\nUser-Agent: ArduinoWiFi/1.1\r\n" +
                "Connection: close\r\n\r\n");
  } else {
    Serial.println("connection failed");
    return;
  }
  String line;
  int repeatCounter = 0;
  while (!client.available() && repeatCounter < 10) {
    delay(500);
    Serial.println("w.");
    repeatCounter++;
  }
  while (client.connected() && client.available()) {
    char c = client.read(); 
    if (c == '[' || c == ']') c = ' ';
    line += c;
  }

  client.stop();

  DynamicJsonBuffer jsonBuf;
  JsonObject &root = jsonBuf.parseObject(line);
  if (!root.success())
  {
    Serial.println("parseObject() failed");
    return;
  }
  //weatherMain = root["weather"]["main"].as<String>();
  weatherDescription = root["weather"]["description"].as<String>();
  weatherDescription.toLowerCase();
  //  weatherLocation = root["name"].as<String>();
  //  country = root["sys"]["country"].as<String>();
  temp = root["main"]["temp"];
  humidity = root["main"]["humidity"];
  pressure = root["main"]["pressure"];
  tempMin = root["main"]["temp_min"];
  tempMax = root["main"]["temp_max"];
  windSpeed = root["wind"]["speed"];
  clouds = root["clouds"]["all"];
  String deg = String(char('~'+25));
  weatherString = "t: " + String(temp,1)+" ";
  weatherString += weatherDescription;
  weatherString += " Влажн.: " + String(humidity) + "% ";
  weatherString += "Давл.: " + String(pressure/1.3332239) + " мм ";
//  weatherString += "Облачность: " + String(clouds) + "% ";
  weatherString += "Ветер: " + String(windSpeed,1) + "М/с";
}

// =======================================================================
// Берем время у GOOGLE
// =======================================================================

float utcOffset = 5; //поправка часового пояса
long localEpoc = 0;
long localMillisAtUpdate = 0;

void getTime()
{
  WiFiClient client;
  if (!client.connect("www.google.com", 80)) {
    Serial.println("connection to google failed");
    return;
  }

  client.print(String("GET / HTTP/1.1\r\n") +
               String("Host: www.google.com\r\n") +
               String("Connection: close\r\n\r\n"));
  int repeatCounter = 0;
  while (!client.available() && repeatCounter < 10) {
    delay(500);
    //Serial.println(".");
    repeatCounter++;
  }

  String line;
  client.setNoDelay(false);
  while(client.connected() && client.available()) {
    line = client.readStringUntil('\n');
    line.toUpperCase();
    if (line.startsWith("DATE: ")) {
      date = "     "+line.substring(6, 22);
      h = line.substring(23, 25).toInt();
      m = line.substring(26, 28).toInt();
      s = line.substring(29, 31).toInt();
      localMillisAtUpdate = millis();
      localEpoc = (h * 60 * 60 + m * 60 + s);
    }
  }
  client.stop();
}

// =======================================================================r

void updateTime()
{
  long curEpoch = localEpoc + ((millis() - localMillisAtUpdate) / 1000);
  long epoch = round(curEpoch + 3600 * utcOffset + 86400L) % 86400L;
  h = ((epoch  % 86400L) / 3600) % 24;
  m = (epoch % 3600) / 60;
  s = epoch % 60;
}

// =======================================================================


String utf8rus(String source)
{
  int i,k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };

  k = source.length(); i = 0;

  while (i < k) {
    n = source[i]; i++;

    if (n >= 0xC0) {
      switch (n) {
        case 0xD0: {
          n = source[i]; i++;
          if (n == 0x81) { n = 0xA8; break; }
          if (n >= 0x90 && n <= 0xBF) n = n + 0x30-1;
          break;
        }
        case 0xD1: {
          n = source[i]; i++;
          if (n == 0x91) { n = 0xB8; break; }
          if (n >= 0x80 && n <= 0x8F) n = n + 0x70-1;
          break;
        }
      }
    }
    m[0] = n; target = target + String(m);
  }
return target;
}
