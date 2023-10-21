#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN  5  // ESP32 pin GIOP5 
#define RST_PIN 27 // ESP32 pin GIOP27 

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::PICC_Type piccType;
bool cardPresent = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    cardPresent = true;
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      piccType = rfid.PICC_GetType(rfid.uid.sak);

      while(cardPresent){
        Serial.print("DATA-");
        for (int i = 0; i < rfid.uid.size; i++) {
          Serial.print(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
          Serial.print(rfid.uid.uidByte[i], HEX);
        }
        cardPresent = rfid.PICC_ReadCardSerial();
      }
      
      Serial.println();
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }else{
      Serial.println("READ-ERROR");      
    }
  }else{
    Serial.println("PLACE-CARD");
  }
  delay(200);
}
