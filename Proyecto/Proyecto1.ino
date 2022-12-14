/*
  Arduino Starter Kit example
  Project 5 - Servo Mood Indicator

  This sketch is written to accompany Project 5 in the Arduino Starter Kit

  Parts required:
  - servo motor
  - 10 kilohm potentiometer
  - two 100 uF electrolytic capacitors

  created 13 Sep 2012
  by Scott Fitzgerald

  https://store.arduino.cc/genuino-starter-kit

  This example code is part of the public domain.
*/

// include the Servo library
#include <Servo.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>


Servo servoLata;  // create a servo object
Servo servoGancho;  // create a servo object
const int btnI = 2;
const int btnD = 3;
int const potPin = A0;  // analog pin used to connect the potentiometer
int potVal;             // variable to read the value from the analog pin
int angle;              // variable to hold the angle for the servo motor
    

#define RST_PIN         5         // Pin para el RESET del RC522
#define SS_PIN          53          // Pin para el SS (SDA) del RC522

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Crea objeto mfrc522 enviando pines de slave select y reset
void mostrarByteArray(byte* buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

 boolean comparaUID(byte lectura[], byte usuario[]) // funcion comparaUID
  {
      for (byte i = 0; i < mfrc522.uid.size; i++) { // bucle recorre de a un byte por vez el UID
        if (lectura[i] != usuario[i])       // si byte de UID leido es distinto a usuario
          return (false);         // retorna falso
      }
    return (true);          // si los 4 bytes coinciden retorna verdadero
  }
byte LecturaUID[4];         // crea array para almacenar el UID leido
byte Usuario1[4] = {0x04, 0xF2, 0xF7, 0x91} ;   // UID de tarjeta leido en programa 1
byte Usuario2[4] = {0x07, 0x29, 0xB7, 0x60} ;   // UID de llavero leido en programa 1

void setup() {
  pinMode(btnI, INPUT);   // pin del pulsador A como entrada
   Serial.begin(9600);  // open a serial connection to your computer
  	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  pinMode(btnD, INPUT);   // pin del pulsador B como entrada
  servoLata.attach(9);   //  // Iniciamos el servo para que empiece a trabajar con el pin 9
  servoGancho.attach(10);//  // Iniciamos el servo para que empiece a trabajar con el pin 10
  SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Escanea PICC para ver el UID, SAK, type, and y los bloques de datos..."));
}

void loop() {

   String content= "";
 // Detecta una tarjeta
  if ( ! mfrc522.PICC_IsNewCardPresent())       // Si no hay una tarjeta presente
  {
    return;    
  }                                     // Retorna al loop esperando por una tarjeta
  if ( ! mfrc522.PICC_ReadCardSerial())         // Si no puede obtener datos de la tarjeta
  {
      Serial.println("No se pueden obtener los datos de la tarjeta");    
       return;                                     // Vuelve al loop esperando por otra tarjeta
  } 
  for (byte i = 0; i < mfrc522.uid.size; i++) { // bucle recorre de a un byte por vez el UID
      if (mfrc522.uid.uidByte[i] < 0x10) {  // si el byte leido es menor a 0x10
      }
      LecturaUID[i] = mfrc522.uid.uidByte[i];   // almacena en array el byte del UID leido
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
   if (comparaUID(LecturaUID, Usuario1)) {  // llama a funcion comparaUID con Usuario1
        Serial.println("Bienvenido Mario, pulsa un botón para elegir el producto"); // si retorna verdadero muestra texto bienvenida
        Serial.println("content");
        Serial.println(content);
  }
Serial.println("Contenido sub");
Serial.println(content.substring(1));
      if (content.substring(1) == "04 F2 F7 91") //change here the UID of card/cards or tag/tags that you want to give access
      {
          if (digitalRead(btnI) == HIGH) // Si el botón A está presionado endiendo el Led
        {
            // Desplazamos a la posición 0º
            servoLata.write(180); // move MG996R's shaft to angle 0°
            delay(1000); // wait for one second
            // Desplazamos a la posición 0º
            servoLata.write(0); // move MG996R's shaft to angle 0°
            delay(1000); // wait for one second
        }
        if (digitalRead(btnD) == HIGH) // Si el botón B está presionado apago el Led
        {
          // Desplazamos a la posición 90º
            servoGancho.write(0);
            delay(1000);  // Esperamos 1 segundo
            // Desplazamos a la posición 180º
            servoGancho.write(360);
            delay(1000);  // Esperamos 1 segundo
      
        }
      }
 
}
