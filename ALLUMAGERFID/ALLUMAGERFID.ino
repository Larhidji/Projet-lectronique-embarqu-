#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN  10
#define RST_PIN  9
#define LED_PIN  7

MFRC522 rfid(SS_PIN, RST_PIN);

// ⬇️ Remplace par ton UID (en décimal)
byte uidAutorise[] = {0xB9, 0x72, 0x3D, 0x98};

bool lampeAllumee = false;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("Système prêt. Approche ta carte...");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  // Vérifier si l'UID correspond
  bool acces = true;
  if (rfid.uid.size != sizeof(uidAutorise)) {
    acces = false;
  } else {
    for (byte i = 0; i < rfid.uid.size; i++) {
      if (rfid.uid.uidByte[i] != uidAutorise[i]) {
        acces = false;
        break;
      }
    }
  }


  if (acces) {
    lampeAllumee = !lampeAllumee;          // Bascule ON/OFF
    digitalWrite(LED_PIN, lampeAllumee ? HIGH : LOW);
    Serial.println(lampeAllumee ? "✅ Lampe ALLUMÉE" : "⭕ Lampe ÉTEINTE");
  } else {
    Serial.println("❌ Carte non autorisée !");
  }
NN
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  delay(1000); // Anti-rebond
}


