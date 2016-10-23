#include <SoftwareSerial.h>
#include <TimerOne.h>


#define RxD 11
#define TxD 10

//PARA EL ONOFF
#define ONOFF 5
//PARA EL ONOFF

SoftwareSerial BTSerial(TxD, RxD);

void setup()
{
  //PARA EL ONOFF  
  pinMode(ONOFF, OUTPUT);
  //PARA EL ONOFF
  
  // Configuracion del puerto serie por software
  // para comunicar con el modulo HC-05
  BTSerial.begin(9600);
  BTSerial.flush();
  delay(500);
  
  // Configuramos el puerto serie de Arduino para Debug
  Serial.begin(115200);
  Serial.println("Readyyyy");

/*
  while(1)
  {
      digitalWrite(ONOFF, HIGH);
      delay(1000);
      digitalWrite(ONOFF, LOW);
      delay(1000);
  }
*/

}


void loop()
{
  // Esperamos ha recibir datos.

  char command;
  if (BTSerial.available())
  {
    
    // La funcion read() devuelve un caracter 
    command = BTSerial.read();
    BTSerial.flush();

    //Serial.println(mensaje);
    Rele(String(command));

  }

}


void Rele(String value)
{
  if (value == "1")
  {
     digitalWrite(ONOFF, HIGH);
  }
  else
     digitalWrite(ONOFF, LOW);
}

