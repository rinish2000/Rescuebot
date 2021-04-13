// IP ESP: 192.168.4.1

// Define componenten
#define reed 2
#define echoOnder 16
#define triggerOnder 12
#define echoVoor 5
#define triggerVoor 1
#define irLeft 14
#define irRight 3
#define motorLeft1 4
#define motorLeft2 0
#define motorRight1 15
#define motorRight2 13

// Externe Functions
extern void Drive(int direction);

// Externe Variabelen
extern bool autonoom;
extern bool autoStop;
extern bool rijBlock;

// Variabelen
const char* ssid = "autobot";
const char* passwd = "peepoo";
WiFiServer server(80);
String header;
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void webPagina(){
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
              Drive(1);
            }
            if (header.indexOf("GET /Stop") >= 0) {
              Drive(0);
            }
            if (header.indexOf("GET /Left") >= 0) {
              Drive(3);
            }
            if (header.indexOf("GET /Right") >= 0) {
              Drive(4);
            }
            if (header.indexOf("GET /Backwards") >= 0) {
              Drive(2);
            }
            if (header.indexOf("GET /Changemode") >= 0) {
              if (autonoom){
                autonoom = 0;
                Drive(1);
              }
              else{
                autonoom = 1;
                Drive(0);
              }
            }
            
            // Web Pagina
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
            
            client.println("<body><h1>RescueBot Web Control</h1>");
            if (!autonoom){
              client.println("<h1>Auto Mode</h1>");
            }
            client.println("<b>niger</b>");
            if (autonoom){
              client.println("<p><a href=\"/Forward\"><button class=\"knopMiddle\">^</button></a></p>");
              client.println("<p><a href=\"/Left\"><button class=\"knopLeft\"><</button></a> <a href=\"/Stop\"><button class=\"knopMiddle\">STOP</button></a> <a href=\"/Right\"><button class=\"knopRight\">></button></a></p>");
              client.println("<p><a href=\"/Backwards\"><button class=\"knopMiddle\">v</button></a></p>");
              if (!autoStop) {
                client.println("<p><a href=\"/autoStopAan\"><button class=\"knopRed\">AutoStop disabled!</button></a></p>");
              }
              else{
                client.println("<p><a href=\"/autoStopUit\"><button class=\"knopGroen\">AutoStop enabled</button></a></p>");
              }
              if (!rijBlock){
                client.println("<p><a href=\"/\"><button class=\"knopMiddle\">Fall Block Disabled</button></a></p>");
              }
              else{
                client.println("<p><a href=\"/rijBlockUit\"><button class=\"knopRed\">Remove Fall Block!</button></a></p>");
              }
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
