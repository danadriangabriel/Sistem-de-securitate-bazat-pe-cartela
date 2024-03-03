#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <LiquidCrystal.h>


#define SS_PIN 10
#define RST_PIN 9
#define LED_G 5 //define green led pin
#define LED_R 4 //define red led pin
#define BUZZER 2 //buzzer pin
MFRC522 mfrc522(SS_PIN,RST_PIN); //create MFRC522 instance
Servo myServo; //define servo name
LiquidCrystal lcd(A0,A1,A2,A3,A4,A5); //define lcd

void setup()
{
  Serial.begin(9600); //Initiate a serial communication
  SPI.begin(); //Initiate SPI bus
  lcd.begin(16,2);
  mfrc522.PCD_Init(); //initiate MFRC522
  myServo.attach(3); //servo pin
  myServo.write(0); //servo start position
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  Serial.println("Put your card to the reader...");
  Serial.println();
}

void loop()
{
  lcd.setCursor(0, 0);
  lcd.print("     Buna Ziua");
  lcd.setCursor(0, 1);
  lcd.print("Scanati cartela");

  //Look for new cards
  if( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }

  //Select one of the cards
  if( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  //Show UID on Serial Monitor
  Serial.print("UID tag: ");
  String content= "";
  byte letter;
  for(byte i=0; i<mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if(content.substring(1) == "33 D1 C0 AA") //Change here the UID of the card/cards that you want to give access
  {
    Serial.println("Acces permis");
    Serial.println();
    delay(500);
    digitalWrite(LED_G, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Card acceptat!");
    lcd.setCursor(0, 1);
    lcd.print("Bine ati venit!");
    tone(BUZZER, 500);
    delay(300);
    noTone(BUZZER);
    myServo.write(180);
    delay(5000);
    myServo.write(0);
    digitalWrite(LED_G, LOW);
    lcd.clear();
  }
  else
  {
    Serial.println("Acces respins");
    digitalWrite(LED_R, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Card invalid!");
    tone(BUZZER, 300);
    delay(1000);
    digitalWrite(LED_R, LOW);
    lcd.clear();
    noTone(BUZZER);
  }
}