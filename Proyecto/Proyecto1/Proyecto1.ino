/*
  Arduino Starter Kit example
  Proyecto DSE - EHU/UPV

  Este proyecto es creado por Mario Ernesto Calixto Perea

  Componentes utilizados:
  - servo motor
  - 2 Resistencia
  - LCD a I2c
  - Fuente de energía externa
  - Placa Elegoo Mega 2560 
  - 2 Botones
  - Lector RFID-RC522
  

  Creado el 15 Dic 2022
  Por Mario Ernesto Calixto Perea

  https://github.com/mcalix/Vendlegoo

  Repositorio almacenado en Github con licencia MIT.
*/

// Includes de librerías
#include <Servo.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

String val = "1234abcdef";

Servo servoLata;  // Crear un objeto servoLata
Servo servoGancho;  // Crear un objeto servoGancho
const int btnI = 2;
const int btnD = 3;
//Crear el objeto lcd  dirección  0x27 y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27,16,2);  //
#define RST_PIN         5         // Pin para el RESET del RC522
#define SS_PIN          53          // Pin para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Crea objeto mfrc522 enviando pines de slave select y reset
//Funcion que muestra los datos de la tarjeta leída
byte LecturaUID[4];         // crea array para almacenar el UID leido
byte Usuario1[4] = {0x04, 0xF2, 0xF7, 0x91} ;   // UID de tarjeta leido en programa 1
byte Usuario2[4] = {0x07, 0x29, 0xB7, 0x60} ;   // UID de llavero leido en programa 1

void mostrarByteArray(byte* buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
//Función que muestra el texto en la pantalla LCD
void textoLCD(){
  lcd.clear();
  // Hay que escribir el Mensaje en el LCD en una posición  central en la fila 1ª
                    lcd.setCursor(0,0);
                    lcd.print("Bienvenidx ");
                    lcd.print("a ");
                    lcd.print("Vendlegoo");
                    // Cursor en la 1° posición de la 2° fila
                    lcd.setCursor(0,1);
                    lcd.print("Acerque ");
                    lcd.print("su");
                    lcd.print(" tarjeta.");
                    // Cursor en la primera posición de la 3° fila
                    lcd.setCursor(0,2);
                    lcd.print("Elija ");
                    lcd.print("una "); 
                    lcd.print("opcion y"); 
                    // Cursor en la 1° posición de la 4° fila
                    lcd.setCursor(0,3);
                    lcd.print("Pulse ");
                    lcd.print("un");
                    lcd.print(" boton  ");
                    delay(200);
}

boolean comparaUID(byte lectura[], byte usuario[]) // funcion comparaUID
  {
      for (byte i = 0; i < mfrc522.uid.size; i++) { // bucle recorre de a un byte por vez el UID
        if (lectura[i] != usuario[i])       // si byte de UID leido es distinto a usuario
          return (false);         // retorna falso
      }
    return (true);          // si los 4 bytes coinciden retorna verdadero
  }

void setup() {
            pinMode(btnI, INPUT);   // pin del pulsador A como entrada
            Serial.begin(9600); 
            while (!Serial);	 
            pinMode(btnD, INPUT);   // pin del pulsador B como entrada
            servoLata.attach(9);   //  // Iniciamos el servo para que empiece a trabajar con el pin 9
            servoGancho.attach(10);//  // Iniciamos el servo para que empiece a trabajar con el pin 10
            SPI.begin();			// Init SPI bus
            mfrc522.PCD_Init();		// Init MFRC522
            delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
            mfrc522.PCD_DumpVersionToSerial();	// Muestra los detalles del MFRC522
            Serial.println(F("Escanea PICC para ver el UID, SAK, type, and y los bloques de datos..."));
            // Inicializar el LCD
            lcd.init();
            //Encender la luz de fondo.
            lcd.backlight();
            // Inicializar el LCD con el número de  columnas y filas del LCD
            lcd.begin(20,4);
            textoLCD();
            int address = 0;
            EEPROM.put(address, val);
            Serial.println("Esto almaceno =" + val);
            String str = "";
            EEPROM.get(address, str);
            Serial.println("Esto he leido =" + str);
}

void loop() {
          String content= "";
        // Detecta una tarjeta
          if ( ! mfrc522.PICC_IsNewCardPresent())       // Si no hay una tarjeta presente
            {
              // Retorna al loop esperando por una tarjeta
              return;    
            }                                     
          else
          {
              //Si hay tarjeta presente
            for (byte i = 0; i < mfrc522.uid.size; i++) { // bucle recorre de a un byte por vez el UID
            if (mfrc522.uid.uidByte[i] < 0x10) 
            {
                // si el byte leido es menor a 0x10
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
                    // Escribimos el Mensaje en el LCD en una posición  central.
                    lcd.setCursor(0,0);
                    lcd.print("Seleccion: ");
                    lcd.print("op. ");
                    lcd.print("izq.");
                    // Cursor en la 1° posición de la 2° fila
                    lcd.setCursor(0,1);
                    lcd.print("Recoja  ");
                    lcd.print("su ");
                    lcd.print(" lata.  ");
                    // Cursor en la primera posición de la 3° fila
                    lcd.setCursor(0,2);
                    lcd.print("Muchas");
                    lcd.print("gracias"); 
                    lcd.print(".  "); 
                    // Cursor en la 1° posición de la 4° fila
                    lcd.setCursor(0,3);
                    lcd.print("Vuelva ");
                    lcd.print("pronto");
                    lcd.print(".  ");
                    
                    // Desplazamos a la posición 0º
                    servoLata.write(360); // Mueve MG996R's 360º
                    delay(1000); // Espera 1 seg
                    // Desplazamos a la posición 0º
                    servoLata.write(0); // Mueve MG996R's 0º
                    delay(1000); // Espera 1 seg
                    lcd.clear();
                    textoLCD();
                }
                if (digitalRead(btnD) == HIGH) // Si el botón B está presionado apago el Led
                {

                  // Escribimos el Mensaje en el LCD en una posición  central.
                    lcd.setCursor(0,0);
                    lcd.print("Seleccion: ");
                    lcd.print("op. ");
                    lcd.print("derecha.  ");
                    // Cursor en la 1° posición de la 2° fila
                    lcd.setCursor(0,1);
                    lcd.print("Recoja  ");
                    lcd.print("su ");
                    lcd.print(" producto.  ");
                    // Cursor en la primera posición de la 3° fila
                    lcd.setCursor(0,2);
                    lcd.print("Muchas");
                    lcd.print("gracias"); //1 decimal
                    lcd.print(".  "); // 
                    // Cursor en la 1° posición de la 4° fila
                    lcd.setCursor(0,3);
                    lcd.print("Vuelva ");
                    lcd.print("pronto");
                    lcd.print(".  ");
                    // Desplazamos a la posición 90º
                    servoGancho.write(0);
                    delay(1000);  // Esperamos 1 segundo
                    // Desplazamos a la posición 180º
                    servoGancho.write(360);
                    delay(1000);  // Esperamos 1 segundo
                    lcd.clear();
                    textoLCD();         
                }
            }  
          }
          if ( ! mfrc522.PICC_ReadCardSerial())         // Si no puede obtener datos de la tarjeta
          {
              Serial.println("No se pueden obtener los datos de la tarjeta");    
              return;                                     // Vuelve al loop esperando por otra tarjeta
          } 
          
}
