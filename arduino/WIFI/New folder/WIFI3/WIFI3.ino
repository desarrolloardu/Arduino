#include "ESP8266.h"
#include <SoftwareSerial.h>
SoftwareSerial BT1(10,11); // RX | TX

#define SSID        "Cisco16027"
#define PASSWORD    "lospericos8"
#define HOST_NAME   "192.168.1.10"
#define HOST_PORT   (80)
ESP8266 wifi(BT1);

String prueba = "";
int cont = 0;
bool actualizar = false;



void setup(void)
{

  
    Serial.begin(115200);
    BT1.begin(19200);
    Serial.print("setup begin\r\n");

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
    uint8_t buffer[550] = {0};
    char result[100];   // array to hold the result.

   if(cont==5)
   {

      if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
       // Serial.print("create tcp ok\r\n");
      } else {
       // Serial.print("create tcp err\r\n");
      }
    
   // char *hello = "GET http://192.168.1.10/admin/modulos/consultar.php?idModulo=0\r\n";

      char *one= "GET http://192.168.1.10/admin/modulos/consultar.php?idModulo=";
      char *two= "1";
      char *three= "\r\n";
  
      strcpy(result,one); // copy string one into the result.
      strcat(result,two); // append string two to the result.
      strcat(result,three); // append string three to the result.
     
      wifi.send((const uint8_t*)result, strlen(result));
      Serial.print("enviando...");
      cont=0;
   }

  if(actualizar) 
  {
      if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
       // Serial.print("create tcp ok\r\n");
      } else {
       // Serial.print("create tcp err\r\n");
      }
      char *four = "GET http://192.168.1.10/admin/modulos/actualizar.php?id=3\r\n";
      strcpy(result,four);
      wifi.send((const uint8_t*)result, strlen(result));
      actualizar=false;
  }

  uint32_t len = wifi.recv(buffer, sizeof(buffer),500);

  if (len > 0) 
  {
    actualizar = true;
    cont=0;
      
    for(uint32_t i = 0; i < len; i++) 
    {
        if((char)buffer[i]=='[' && (char)buffer[i+1]=='[' && (char)buffer[i+2]=='[' )
        {
            i+=3;
            prueba = "";

            while((char)buffer[i]!=']')
            {
                prueba = prueba + (char)buffer[i];                   
                i++;
            }
            
          /*
            for(int j=0;j<58;j++){
              
                prueba = prueba + (char)buffer[i];   
                i++;
              }*/
            /*
            for(int j=0;(char)buffer[i]!=']';j++){
              
                prueba = prueba + (char)buffer[i];   
                i++;
              }
*/
           //prueba+="\r\n";
           Serial.println(prueba);  
        }

    }
      
  }
    
  cont++;
  
}
