//SLAVE

//определяем пины SPI
#define MOSI_PIN 11 //data,q8
#define MISO_PIN 12 
#define SCK_PIN  13//clock
#define SS_PIN 10//latch,ps
//определяем переменную для получаемого байта
byte recievedByte;

void setup() {
  //обнуляем регистр управления SPI
  SPCR = B00000000;
  //разрешаем работу SPI
  SPCR = (1<<SPE);
  //определяем пины со светодиодами как выходы
  for(int i=2;i<10;i++){
    pinMode(i, OUTPUT);
  }
  //инициализируем последовательное соединение
  Serial.begin(9600);
  //определяем пины для работы с SPI
  pinMode(MOSI_PIN, INPUT);
  pinMode(MISO_PIN, OUTPUT);
  pinMode(SCK_PIN, INPUT);
  pinMode(SS_PIN, INPUT);
}

void loop() {
  //пока пин slave select опущен
  while (digitalRead(SS_PIN)==LOW){
    //принимаем байт и записываем его в переменную
    recievedByte=spi_receive();
    //смотрим в мониторе полученный байт
    Serial.println(recievedByte,BIN);
    //зажигаем светодиоды, которые соответствуют единицам в полученном байте
    /*
    digitalWrite(2,recievedByte & (1 << 0));
    digitalWrite(3,recievedByte & (1 << 1));
    digitalWrite(4,recievedByte & (1 << 2));
    digitalWrite(5,recievedByte & (1 << 3));
    digitalWrite(6,recievedByte & (1 << 4));
    digitalWrite(7,recievedByte & (1 << 5));
    digitalWrite(8,recievedByte & (1 << 6));
    digitalWrite(9,recievedByte & (1 << 7));
    */
  }
}

//функция для приема байта
byte spi_receive()
{
  //пока не выставлен флаг окончания передачи, принимаем биты
  while (!(SPSR & (1<<SPIF))){};
  //позвращяем содержимое регистра данных SPI
  return SPDR;                    
}
