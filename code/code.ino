#include <SPI.h> //Inclui a biblioteca SPI.h
#include <Ethernet.h> //Inclui a biblioteca Ethernet.h

// Configurações para o Ethernet Shield
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x83, 0xEA }; // Entre com o valor do MAC

// network config
IPAddress ip(192,168,1,30);            // config the arduino IP
byte gateway[] = { 192 , 168, 1, 1 };  // config the default gateway
byte subnet[] = { 255, 255, 255, 0 };  // config the subnet mask
EthernetServer server(80);             // init the server in the IP passed

const int LM35 = A0;
float temperatura;

const int ldr    = A1;

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
}

void loop() {
  temperatura = (float(analogRead(LM35))*5/(1023))/0.01;

  EthernetClient client = server.available(); // check if has a client connected

  if ( client ) {
    boolean currentLineIsBlank = true; // A requisição HTTP termina com uma linha em branco Indica o fim da linha
    String valPag;

    while ( client.connected() ) {
      if ( client.available() ) {
        char c = client.read(); //Variável para armazenar os caracteres que forem recebidos
        valPag.concat(c);       // Pega os valor após o IP do navegador ex: 192.168.1.2/0001        
        
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
          client.print(temperatura);
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

        } //Fecha if (c == '\n' && currentLineIsBlank)
        
      } //Fecha if (client.available())
      
    } //Fecha While (client.connected())
    
    delay(3);// Espera um tempo para o navegador receber os dados
    client.stop(); // Fecha a conexão
    
  } //Fecha if(client)
  
} //Fecha loop

const char * getDoorStatus() {
   return "aberto"; 
}

const char * getLightStatus() {   
   if ( analogRead(ldr) < 1000 ) return "Acesa";
   else return "Apagada";
   
   return "";
}

