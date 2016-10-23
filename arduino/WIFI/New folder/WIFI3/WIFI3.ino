#include "ESP8266.h"
#include <SoftwareSerial.h>
SoftwareSerial BT1(10,11); // RX | TX

#define SSID        "Cisco16027"
#define PASSWORD    "lospericos8"
#define HOST_NAME   "www.domtec.hol.es"
#define HOST_PORT   (80)
ESP8266 wifi(BT1);

String prueba = "";
int cont = 0;

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



    if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
       // Serial.print("create tcp ok\r\n");
    } else {
       // Serial.print("create tcp err\r\n");
    }


    if(cont==5){
    char *hello = "GET http://www.domtec.hol.es/admin/modulos/consultar.php?idModulo=3\r\nHTTP/1.1\r\nHost: www.domtec.hol.es\r\nAccept: text/plain\r\n";
    wifi.send((const uint8_t*)hello, strlen(hello));
    cont=0;
    };
    

    uint32_t len = wifi.recv(buffer, sizeof(buffer),500);
    
    if (len > 0) {
        
        for(uint32_t i = 0; i < len; i++) {
           if((char)buffer[i]=='[' && (char)buffer[i+1]=='[' && (char)buffer[i+2]=='[' ){
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
