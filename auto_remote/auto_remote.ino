#include "Arduino.h"
#include <ESP8266WiFi.h>



// Variabelen
const char* ssid = "autobots9000";
const char* passwd = "peepoopeepoo9000";
WiFiServer server(80);
String header;
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

bool autonoom = 1;

void setup() {
    Serial.begin(9600);
    WiFi.softAP(ssid, passwd);
    server.begin();
}




void loop(){
  WiFiClient client = server.available();
  if (client) {
    String currentLine = "";
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            if (header.indexOf("GET /Forward") >= 0) {
            //   Drive(1);
                Serial.println("");
            }
            if (header.indexOf("GET /Stop") >= 0) {
            //   Drive(0);
                Serial.println("");
            }
            if (header.indexOf("GET /Left") >= 0) {
            //   Drive(3);
                Serial.println("");
            }
            if (header.indexOf("GET /Right") >= 0) {
            //   Drive(4);
                Serial.println("");
            }
            if (header.indexOf("GET /Backwards") >= 0) {
            //   Drive(2);
                Serial.println("");
            }
            if (header.indexOf("GET /Changemode") >= 0) {
              if (autonoom){
                autonoom = 0;
                // Drive(1);
                Serial.println("");
              }
              else{
                autonoom = 1;
                // Drive(0);
                Serial.println("");
              }
            }
            
            // Web Page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<link rel=stylesheet href=\"css.css\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".knopGroen { background-color: #71e300; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".knopRed { background-color: #e30000; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".knopMiddle { background-color: #970000; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".knopLeft { background-color: #970000; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".knopRight { background-color: #970000; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}</style></head>");
            
            client.println("<body><h1> Autobot9000 </h1>");
            if (!autonoom){
              client.println("<h1>Auto Mode</h1>");
            }
            client.println("<b>niger</b>");
            if (autonoom){
              client.println("<p><a href=\"/Forward\"><button class=\"knopMiddle\">^</button></a></p>");
              client.println("<p><a href=\"/Left\"><button class=\"knopLeft\"><</button></a>  <a href=\"/Right\"><button class=\"knopRight\">></button></a></p>");
              client.println("<p><a href=\"/Backwards\"><button class=\"knopMiddle\">v</button></a></p>");

              client.println("<p><a href=\"/Stop\"><button class=\"knopMiddle\">STOP</button></a></p>");
            }
            client.println("<p><a href=\"/Changemode\"><button class=\"knopMiddle\">Change mode</button></a></p>");
            client.println("</body></html>");
            client.println();
            break;
            } 
            else {
              currentLine = "";
            }
        } 
        else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
  }
}
