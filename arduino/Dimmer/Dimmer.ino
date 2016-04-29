/*

 Pin    |  Interrrupt # | Arduino Platform
 ---------------------------------------
 2      |  0            |  All
 3      |  1            |  All
 18     |  5            |  Arduino Mega Only
 19     |  4            |  Arduino Mega Only
 20     |  3            |  Arduino Mega Only
 21     |  2            |  Arduino Mega Only
 
*/

#include <SoftwareSerial.h>
#include <TimerOne.h>

//VCC va conectado a 5V
#define RxD 11
#define TxD 10
#define RST 5 // Encendido del Modulo
#define KEY 4

#define LED 13

//GENERAL
#define GND3 12
#define VCC3 3
//GENERAL

//PARA EL ONOFF
#define GND2 8
#define ONOFF 13
//PARA EL ONOFF

SoftwareSerial BTSerial(TxD, RxD);
byte pinEstado = 0;

/***********dimmer***********/
volatile int intensidad = 0;
int Salida2 = 6;    // Output to Opto Triac pin
int i = 1;
String mensaje = "";
/***********\dimmer***********/

void setup()
{
  
  pinMode(LED, OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode(KEY, OUTPUT);

  //PARA EL ONOFF  
  pinMode(GND2, OUTPUT);
  pinMode(ONOFF, OUTPUT);
  digitalWrite(GND2, LOW);
  digitalWrite(ONOFF, HIGH);
  //PARA EL ONOFF
  
  //GENERAL  
  pinMode(GND3, OUTPUT);
  pinMode(VCC3, OUTPUT);
  digitalWrite(GND3, LOW);
  digitalWrite(VCC3, HIGH);
  //GENERAL
  
  // Estado inicial
  digitalWrite(LED, LOW);
  digitalWrite(RST, LOW);
  // Modo Comunicacion
  digitalWrite(KEY, LOW); 
   
  // Encendemos el modulo.
  digitalWrite(RST, HIGH);
  
  
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
  // Esperamos ha recibir datos.
  //String mensaje = "";
  char command;
  if (BTSerial.available())
  {
    
    // La funcion read() devuelve un caracter 
    command = BTSerial.read();
    BTSerial.flush();
    // Serial.println(command);
    if(command == ';')
    {
      //Serial.println(mensaje);
      DimmerOn(mensaje);
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
