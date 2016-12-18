/*

 Pin    |  Interrrupt # | Arduino Platform
 ---------------------------------------
 2      |  0            |  All
 3      |  1            |  All
 18     |  5            |  Arduino Mega Only
 19     |  4            |  Arduino Mega Only
 20     |  3            |  Arduino Mega Only
 21     |  2            |  Arduino Mega Only


 hola
*/

#include <SoftwareSerial.h>
#include <TimerOne.h>

//VCC va conectado a 5V
#define RxD 11
#define TxD 10


//GENERAL
#define GND3 12
#define VCC3 3
String mensaje = "";
//GENERAL

//PARA EL ONOFF
#define ONOFF 5
//PARA EL ONOFF

SoftwareSerial BTSerial(TxD, RxD);
byte pinEstado = 0;

char command;

/***********dimmer***********/
volatile int intensidad = 0;
int Salida2 = 6;    // Output to Opto Triac pin
int i = 1;
/***********\dimmer***********/

void setup()
{
  //PARA EL ONOFF  
  pinMode(ONOFF, OUTPUT);
  //PARA EL ONOFF
  
  //GENERAL  
  pinMode(GND3, OUTPUT);
  pinMode(VCC3, OUTPUT);
  digitalWrite(GND3, LOW);
  digitalWrite(VCC3, HIGH);
  //GENERAL
  
  
  /***********dimmer***********/
  Timer1.initialize(10000);
    pinMode(Salida2, OUTPUT);	      // Set the AC Load as output
  attachInterrupt(0, zero_crosss_int, FALLING);  // Choose the zero cross interrupt # from the table above
  Timer1.attachInterrupt(pulse);
  
  /***********\dimmer***********/
  
  
  // Configuracion del puerto serie por software
  // para comunicar con el modulo HC-05
  BTSerial.begin(9600);
  BTSerial.flush();
  delay(500);
  
  // Configuramos el puerto serie de Arduino para Debug
  //Serial.begin(115200);
  //Serial.println("Readyyyy");
  
}


/***********dimmer***********/
void pulse()
{
  digitalWrite(Salida2, HIGH);   // triac firing
  delayMicroseconds(100);         // triac On propogation delay
  digitalWrite(Salida2, LOW);
  
  Timer1.stop();
 //Timer1.setPeriod(9980);
}

void zero_crosss_int()  // function to be fired at the zero crossing to dim the light
{  
 // Serial.println("zero");
  if (intensidad != 0)
  {
    Timer1.setPeriod(500*(20-intensidad));
  }
  else 
  {
    Timer1.stop();   
  } 
}
/***********\dimmer***********/


void loop()
{
  // Esperamos a recibir datos.
  //String mensaje = "";
  
  //if (BTSerial.available())

  command = BTSerial.read();
  if (command != -1)
  {
    if(command == ';')
    {
      //Serial.println(mensaje);
     if((mensaje == "0") || (mensaje == "2") || (mensaje == "3") || (mensaje == "4") || (mensaje == "5") || (mensaje == "6") || (mensaje == "7") || (mensaje == "8") || (mensaje == "9") || (mensaje == "10") || (mensaje == "11") || (mensaje == "12") || (mensaje == "13") || (mensaje == "14") || (mensaje == "15") || (mensaje == "16") || (mensaje == "17") || (mensaje == "18") || (mensaje == "19")|| (mensaje == "20")) 
        DimmerOn(mensaje);
      if((mensaje == "ON") || (mensaje == "OFF"))
        Rele(mensaje);
        
      mensaje = "";
    }
    else
    {
      mensaje = mensaje + String(command);
      //Serial.println(String(command));
    }
  }

}


/***********\Dimmer***********/
void DimmerOn(String value)
{
        intensidad = value.toInt();
}
/***********\Dimmer***********/

//PARA EL ONOFF  
void Rele(String value)
{
  if (value == "ON")
  {
     digitalWrite(ONOFF, HIGH);
  }
  if (value == "OFF")
     digitalWrite(ONOFF, LOW);
}
//PARA EL ONOFF  
