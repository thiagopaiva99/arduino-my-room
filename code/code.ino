// including some libs
#include <SPI.h>
#include <Ethernet.h>

// network config
IPAddress ip(192,168,1,30);                              
byte mac[]     = { 0x90, 0xA2, 0xDA, 0x0D, 0x83, 0xEA }; 
byte gateway[] = { 192 , 168, 1, 1 };                    
byte subnet[] = { 255, 255, 255, 0 };                    
EthernetServer server(80);                               

const int LM35 = A0;
const int ldr  = A1;

void setup() {
  Ethernet.begin(mac, ip);
}

void loop() {
  EthernetClient client = server.available(); // check if has a client connected

  if ( client ) {
    boolean currentLineIsBlank = true;

    while ( client.connected() ) {
      if ( client.available() ) {
        char c = client.read();
        
        if ( c == '\n' && currentLineIsBlank ) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.print("<html><head><meta charset=\"utf-8\"><meta http-equiv=\"refresh\" content=\"5; url=http://192.168.1.30/\"><link href=\"https://fonts.googleapis.com/css?family=Ubuntu\" rel=\"stylesheet\"><title>@thiagopaiva99 - my room status</title></head><body>");
          
          // styles - minify all later
          // body
          client.print("<style>body{font-family:Ubuntu;}");
          
          // temp
          client.print(".temp{box-shadow:5px 5px 10px rgba(0,0,0,.3);cursor:pointer;position:fixed;top:0;right:0;margin:25px;background:#eee;padding:10px;border-radius:4px;text-transform:uppercase}");
          
          // widgets
          client.print(".widgets{display:flex;width:100%;height:100%;justify-content:center;align-items:center}");
          
          // door
          client.print(".door,.light{margin:15px;height:200px;width:200px;background:#eee;text-align:center;border-radius:4px;box-shadow:5px 5px 10px rgba(0,0,0,.3);display:flex;justify-content:center;align-items:center}");          
          
          client.print("</style>");
          
          client.print("<div class=\"temp\">");   
          client.print("Temperatura do quarto: ");
          client.print(getTemp());
          client.print("ºC");
          client.print("</div>");
          
          client.print("<div class=\"widgets\">");
          
          client.print("<div class=\"door\">");
          client.print("Porta:<br>");
          client.print(getDoorStatus());
          client.print("</div>");
          
          client.print("<div class=\"light\">");
          client.print("Luz:<br>");
          client.print(getLightStatus());
          client.print("</div>");
          
          client.print("</div>");
          
          client.print("</body></html>");

          break;
        }        
      }      
    }
    
    delay(3);
    client.stop();    
  }  
}

float getTemp() {
   return (float(analogRead(LM35))*5/(1023))/0.01; 
}

const char * getDoorStatus() {
   return "aberto"; 
}

const char * getLightStatus() {   
   if ( analogRead(ldr) < 1000 ) return "Acesa";
   else return "Apagada";
   
   return "";
}

