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

char result[100];

int dato = 0;
String selec_id = ""; 
String selec_idModulo = "";  
String selec_entrada = "";    
String selec_accion = ""; 
String selec_valor = "";
String selec_ejecutado = ""; 
uint8_t buffer[50] = {0};


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
   //result[100] = {'\0'};   // array to hold the result.

   if(cont==5)
   {
    
     Serial.print(wifi.releaseTCP());
     if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
        Serial.print("create tcp ok\r\n");
      } else {
        Serial.print("create tcp err\r\n");
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
  //  Serial.print("actualizar");
    //Serial.print("actualizar - selec_id: " + selec_id);
      if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
       // Serial.print("create tcp ok\r\n");
      } else {
       // Serial.print("create tcp err\r\n");
      }
      
      char *four = "GET http://192.168.1.10/admin/modulos/actualizar.php?id=";

      // Define 
      // String str = "This is my string"; 
    
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

      Serial.print(result);
      
      wifi.releaseTCP();
     
      
  }
  

  uint32_t len = wifi.recv(buffer, sizeof(buffer),600);

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
           Serial.println(" selec_id:" + selec_id); 
           Serial.println(" selec_idModulo:" + selec_idModulo); 
           Serial.println(" selec_entrada:" + selec_entrada); 
           Serial.println(" selec_accion:" + selec_accion); 
           Serial.println(" selec_valor:" + selec_valor); 
           Serial.println(" selec_idModulo:" + selec_ejecutado); 
           
        }

    }

    actualizar = true;
      
  }
    
  cont++;
  
}
