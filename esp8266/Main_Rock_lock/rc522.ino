/*
  SDA  - D2
  SCk  - D5
  MOSI - D7
  MISO - D6
  RST  - D0
*/




#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN   16     // D0 Configurable, see typical pin layout above
#define SS_PIN    4      // D2 Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

/* Set your new UID here! */
#define NEW_UID {0xDE, 0xAD, 0xBE, 0xEF}

MFRC522::MIFARE_Key key;

void setup_rc_522() {
  SPI.begin();         // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522 card
  Serial.println(F("Warning: this example overwrites the UID of your UID changeable card, use with care!"));

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

char * dump_byte_array(byte *buffer, byte bufferSize) {
  //byte header[] = {0x5A, 0xA5};
  char dest[bufferSize];
  sprintf(dest, "%s", "");


  for (byte i = 0; i < bufferSize; i++) {
    //Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    //Serial.print(buffer[i], HEX);
    char src[2];
    sprintf(src, "%02X", buffer[i]);
    //memcpy(dest + sizeof(src - 1) + 1, src, sizeof(src) + 1);
    strncat(dest, src, 2);
  }
  //dest[bufferSize] = '\0';
  //Serial.println("Copied:");
  return dest;
}


void loop_rc522() {

  // Look for new cards, and select one if present
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    delay(50);
    return;
  }
  String codeIR;
  Serial.print(F("Card UID:"));
  codeIR = String(dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size));
  Serial.print(codeIR);
  Serial.println();


  //  Serial.println(codeIR);
  if (!Page_IR_opened) {
    if (check_code_IR(codeIR)) {
      open_lock();
      delay(3000);
      //reboot
      ESP.reset();
    }
    // saveocde_to_file(codeIR);
  }
  else {
    server.send(200, "text/plain", codeIR);
  }
}
