#include <SoftwareSerial.h>
#include <TimerOne.h>


#define RxD 11
#define TxD 10

//PARA EL ONOFF
#define ONOFF 5
String mensaje = "";
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

  command = BTSerial.read();
  while (command != -1)
  { 
    // La funcion read() devuelve un caracter 

     if(command == ';')
    {
      //Serial.println(mensaje);
      Rele(mensaje);
      mensaje = "";
    }
    else
    {
      mensaje = mensaje + String(command);
      //Serial.println(String(command));
    }
    command = BTSerial.read();
    BTSerial.flush();
  }

  delay(3000);
}


void Rele(String value)
{
  if (value == "ON")
  {
     digitalWrite(ONOFF, HIGH);
  }
  if (value == "OFF")
     digitalWrite(ONOFF, LOW);
}

