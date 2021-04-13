#include "Arduino.h"
#include <ESP8266WiFi.h>


#define Right_forward D3 
#define Left_forward D5
#define Right_back D4  
#define Left_back D6
#define IR_right D0
#define Echo_front D2
#define Echo_down D1
#define IR_left D7
#define Trigger D8
#define metal_pin 3

// Variabelen
const char* ssid = "autobots9000";
const char* passwd = "peepoopeepoo9000";

WiFiServer server(80);

String  data ="";
WiFiClient client;

bool mode = false;


void setup() {
    Serial.begin(9600);
    WiFi.softAP(ssid, passwd);
    server.begin();

    pinMode(Right_forward,OUTPUT);   //right forward
    pinMode(Left_forward,OUTPUT);   //right back
    pinMode(Right_back,OUTPUT);   //left forward
    pinMode(Left_back,OUTPUT);   //left back

    pinMode(Echo_down,INPUT); 
    pinMode(Echo_front,INPUT); 
    pinMode(IR_right,INPUT); 
    pinMode(IR_left,INPUT);

    pinMode(Trigger,OUTPUT);  

    pinMode(metal_pin,FUNCTION_3);
}



void loop(){
    client = server.available();
    if (!client) return; 
    data = checkClient ();

    if (data == "mode")
    {
        forward();
        check_ir();
        ultrasonic_down();
        check_metal();
        delay(100);
        ultrasonic_front();
    }

    else if (data == "forward") forward();
    
    else if (data == "reverse") backward();

    else if (data == "left") remote_left();

    else if (data == "right") remote_right();

    else if (data == "stop") stop_motor();
}

void stop_motor()
{
    digitalWrite(Right_forward,LOW);
    digitalWrite(Left_forward,LOW);
    digitalWrite(Right_back,LOW);
    digitalWrite(Left_back,LOW);
}

void forward()
{
    digitalWrite(Right_forward,HIGH);
    digitalWrite(Left_forward,HIGH);
    digitalWrite(Right_back,LOW);
    digitalWrite(Left_back,LOW);
}

void backward()
{
    digitalWrite(Right_back,HIGH);
    digitalWrite(Left_back,HIGH);
    digitalWrite(Right_forward,LOW);
    digitalWrite(Left_forward,LOW);
    delay(500);
}

void remote_right()
{
    digitalWrite(Right_back,HIGH);
    digitalWrite(Right_forward,LOW);
    digitalWrite(Left_forward,HIGH);
    digitalWrite(Left_back,LOW);
}

void remote_left()
{
    digitalWrite(Left_back,HIGH);
    digitalWrite(Left_forward,LOW);
    digitalWrite(Right_forward,HIGH);
    digitalWrite(Right_back,LOW);
}

void turn_right(int radius)
{
    // unsigned long current_time = millis();
    // stop_motor();
    
    // while ((millis() - current_time) < (radius*100))
    // {
    //     digitalWrite(Right_back,HIGH);
    //     digitalWrite(Right_forward,LOW);
    //     digitalWrite(Left_forward,HIGH);
    //     digitalWrite(Left_back,LOW);
    // } 

    digitalWrite(Right_back,HIGH);
    digitalWrite(Right_forward,LOW);
    digitalWrite(Left_forward,HIGH);
    digitalWrite(Left_back,LOW);
    delay(radius*100);
    // stop_motor();
    stop_motor();
}

void turn_left(int radius)
{
    // unsigned long current_time = millis();
    // stop_motor();

    digitalWrite(Left_back,HIGH);
    digitalWrite(Left_forward,LOW);
    digitalWrite(Right_forward,HIGH);
    digitalWrite(Right_back,LOW);
    delay(radius*100);
    // while ((millis() - current_time) < (radius*100))
    // {
        
    // } 
    // Serial.print("rotated");
    stop_motor();
}


void check_ir()
{
    pinMode(IR_right,INPUT); 
    pinMode(IR_left,INPUT);
    int right_ir = digitalRead(IR_right);
    int left_ir = digitalRead(IR_left);


    if (right_ir > 0)
    {

        turn_left(9);
    }

    if (left_ir > 0)
    {
        turn_right(9);
    }
}

void ultrasonic_down()
{
    digitalWrite(Trigger, LOW);
    delayMicroseconds(5);
    digitalWrite(Trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trigger, LOW);

    pinMode(Echo_down,INPUT);

    int duration = pulseIn(Echo_down, HIGH);

    int cm = (((duration/2) / 29.1)-4); 
    // Serial.print(cm);
    // Serial.print("cm down");
    // Serial.println();

    if (cm > 5 && cm < 200)
    {   
        backward();
        turn_left(40);
    }

}


void ultrasonic_front()
{
    digitalWrite(Trigger, LOW);
    delayMicroseconds(5);
    digitalWrite(Trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trigger, LOW);

    pinMode(Echo_front,INPUT);

    int duration = pulseIn(Echo_front, HIGH);

    int cm = (((duration/2) / 29.1)); 
    // Serial.print(cm);
    // Serial.print("cm front");
    // Serial.println();

    if (cm < 11 && cm < 200 && cm> 0)
    {   
        // turn_left(18);
        backward();
        backward();
        turn_right(18);
    }
}

void check_metal()
{
   int metal =  digitalRead(metal_pin);

   if (metal == 0)
   {
       backward();
       backward();
       stop_motor();
       delay(10000);
   }
}

String checkClient (void)
{
  while(!client.available()) delay(1); 
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length()-9,9);
  return request;
}
