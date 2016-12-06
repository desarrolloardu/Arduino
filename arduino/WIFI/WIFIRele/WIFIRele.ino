#include "ESP8266.h"
#include <SoftwareSerial.h>
SoftwareSerial BT1(8,9); // RX | TX --> WIFI

SoftwareSerial BTBluetooth(10,11); // RX | TX

//#define SSID        "Cisco16027"
//#define PASSWORD    "lospericos8"

#define SSID        "NotebookNetwork"
#define PASSWORD    "dvo3E7Wxxne4kybJEouyPK"
#define HOST_NAME   "192.168.1.107"
#define HOST_PORT   (80)
ESP8266 wifi(BT1);

//PARA EL ONOFF
#define ONOFF 5
char command;
String mensaje = "";
//PARA EL ONOFF

String prueba = "";
int cont = 0;
int cconnect = 0;

bool actualizar = false;
bool bconnect = false;
int bluetimeout = 1500; // referencia a tiempo de enfoque en bluethooth. cuando supera este valor empieza a chequear wifi tambien.

char result[100];

int dato = 0;
String selec_id = ""; 
String selec_idModulo = "";  
String selec_entrada = "";    
String selec_accion = ""; 
String selec_valor = "";
String selec_ejecutado = ""; 
uint8_t buffer[50];


void setup(void)
{
    //PARA EL ONOFF  
    pinMode(ONOFF, OUTPUT);
    //PARA EL ONOFF

    BTBluetooth.begin(9600);
    BTBluetooth.flush();
    //delay(500);
  
    Serial.begin(115200);
    BT1.begin(19200);
    Serial.print("setup begin\r\n");

    BT1.listen();
    Serial.print("FW Version: ");
    Serial.println(wifi.getVersion().c_str());
    
    
    if (wifi.setOprToStation()) {
        Serial.print("to station ok\r\n");
    } else {
        Serial.print("to station err\r\n");
    }

    if (wifi.joinAP(SSID, PASSWORD)) {
        Serial.print("Join AP success\r\n");
        Serial.print("IP: ");       
        Serial.println(wifi.getLocalIP().c_str());
    } else {
        Serial.print("Join AP failure\r\n");
    }

     
    
    Serial.print("setup end\r\n");

   


}

void loop(void)
{
   //result[100] = {'\0'};   // array to hold the result.


   if(cconnect > bluetimeout){

  //  Serial.println("contador en 10000");
    cconnect = 0;
    bconnect = false;
    }

  BTBluetooth.listen();
  delay(15);
  //Serial.println("esperando!!!");
  command = BTBluetooth.read();
  //BTBluetooth.flush();
  while (command != -1)
  { 

    // La funcion read() devuelve un caracter 
   /* Rele("ON");
    delay(1000);
    Rele("OFF");
    */
     if(command == ';')
    {
     // Serial.println(mensaje);
      if(mensaje == "CN"){

        bconnect = true;
        cconnect = 0;
        
        }else{
            Rele(mensaje);
      }
      mensaje = "";
    }
    else
    {
      mensaje = mensaje + String(command);
      //Serial.println(String(command));
    }
    command = BTBluetooth.read();
    BTBluetooth.flush();
  }
   

   if(cont==50)
   {
     BT1.listen();
     wifi.releaseTCP();
   //  Serial.print(wifi.releaseTCP());
     wifi.createTCP(HOST_NAME, HOST_PORT);
   /*
     if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
        Serial.print("create tcp ok\r\n");
      } else {
        Serial.print("create tcp err\r\n");
      }
    */
   // char *hello = "GET http://192.168.1.107/admin/modulos/consultar.php?idModulo=0\r\n";

      char *one= "GET http://192.168.1.107/admin/modulos/consultar.php?idModulo=";
      char *two= "1";
      char *three= "\r\n";
  
      strcpy(result,one); // copy string one into the result.
      strcat(result,two); // append string two to the result.
      strcat(result,three); // append string three to the result.
     
      wifi.send((const uint8_t*)result, strlen(result));
      Serial.print("consulto...");
      cont=0;

      BTBluetooth.listen();
   }

  if(actualizar) 
  {
     BT1.listen();
  //  Serial.print("actualizar");
    //Serial.print("actualizar - selec_id: " + selec_id);
      if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
       // Serial.print("create tcp ok\r\n");
      } else {
       // Serial.print("create tcp err\r\n");
      }
      
      char *four = "GET http://192.168.1.107/admin/modulos/actualizar.php?id=";
    
      // Length (with one extra character for the null terminator)
      int str_len = selec_id.length() + 1; 
      
      // Prepare the character array (the buffer) 
      char char_array[str_len];
      
      // Copy it over 
      selec_id.toCharArray(char_array, str_len);
      
      char *five = char_array;
      char *six = "\r\n";
      strcpy(result,four);
      strcat(result,five); // append string two to the result.
      strcat(result,six); // append string two to the result.
      
      wifi.send((const uint8_t*)result, strlen(result));
      actualizar=false;
      Serial.print("actualizo");
   //   Serial.print(result);
      
      wifi.releaseTCP();
     
      BTBluetooth.listen();
  }
  
  if(BTBluetooth.available()>0 || bconnect)
  {
  }
  else
  {
    BT1.listen();
    uint32_t len = wifi.recv(buffer, sizeof(buffer),60);
  
    if (len > 0) 
    {
      
      cont=3;
  
      for(uint32_t i = 0; i < len; i++) 
      {
          if((char)buffer[i]=='[' && (char)buffer[i+1]=='[' && (char)buffer[i+2]=='[' )
          {    
              i+=3;
              prueba = "";
              dato = 0;
  
              while((char)buffer[i]!=']')
              {
                  if ((char)buffer[i] == ';')
                  {
                    dato++;
  
                    switch ( dato )
                    {  
                      case 1 : selec_id = prueba; break;
                      case 2 : selec_idModulo = prueba; break;
                      case 3 : selec_entrada = prueba; break;
                      case 4 : selec_accion = prueba; break;
                      case 5 : selec_valor = prueba; break;
                      case 6 : selec_ejecutado = prueba; break;
                    }
                
                    prueba = "";
                  }
                  else
                    prueba = prueba + (char)buffer[i];                   
                  i++;
              }
              
             //prueba+="\r\n";
            // Serial.println(" imprimio");  

            /*
             Serial.println(" selec_id:" + selec_id); 
             Serial.println(" selec_idModulo:" + selec_idModulo); 
             Serial.println(" selec_entrada:" + selec_entrada); 
             Serial.println(" selec_accion:" + selec_accion); 
             Serial.println(" selec_valor:" + selec_valor); 
             Serial.println(" selec_idModulo:" + selec_ejecutado); 
            */
             if(selec_id != "")
                actualizar = true;
             
          }
  
      }
  
      
  
      Rele(selec_accion);
  
    }
  
    cont++;
  } 

   cconnect++;
  
}


void Rele(String value)
{
  if (value == "ON")
  {
     digitalWrite(ONOFF, HIGH);
  }
  if (value == "OFF")
  {
     digitalWrite(ONOFF, LOW);
  }
}

