#include <ESP8266WiFi.h>    //declare using library ESP8266WiFi.h to set up HTTP client mode for board ESP8266

WiFiClient client;          // create an variable client has datatype WiFiClient
const char* ssid = "yyy";      // the wireless network name (SSID)
const char* password = "VangY@0306";  // Password of Wifi network will be connected  
const char* server = "192.168.43.36";     // IP address of computer when connecting with the wifi network
const int port = 8000;                    // Port of opened server 

const int buzzer = 12;                // connect data pin to GPI012 of ESP8266
const int LED = 10;                   // connect data pin to GPI010 of ESP8266
const int flame_sensor = 4;           // connect data pin to GPI004 of ESP8266
int flame_detected;             //variable to storing the digital value read out from flame sensor.
float warn;
void setup()
{
  Serial.begin(115200);            // connect UART, baud = 115200
  Serial.println("Connecting");
 
    // set ESP8266 is Station and connest to Wifi. display the dots `.` on terminal if it is not connected
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(100);
    }
  
  Serial.println("\r\nWiFi connected");
  
  pinMode(flame_sensor, INPUT);     //set input
  pinMode(LED, OUTPUT);             // set output
  pinMode(buzzer,OUTPUT);           // set output
}


void loop()
{
  //read the digital output from flame sensor and stores it in the variable “flame_detected”.
  flame_detected = digitalRead(flame_sensor); 

  if (flame_detected ==0)               //if flame has been detected, displaying warning messages and turning led on 
  {
    warn = 1;
    Serial.println("Flame detected...! take action immediately.");   
    Serial.println("LED on"); 
    digitalWrite(LED, HIGH);
    digitalWrite(buzzer,HIGH);
    delay(200);
    digitalWrite(LED, LOW);
    digitalWrite(buzzer,LOW);
    delay(200);
    
  }
  else                               //if flame has not been detected, displaying notification messages and turning led off
  {
    Serial.println("No flame detected. stay cool");
    warn = 0;
    digitalWrite(LED, LOW);
  }

  if (client.connect(server, port)) {       // create an connect to server through opened IP and Port.
  //---------------------------------------------------------------------------------------
    String req_uri = "/update?warn=" + String(warn);
    client.print("GET " + req_uri + " HTTP/1.1\n" + "Host: "+ server +"\n" +
                 "Connection: close\n" + "Content-Length: 0\n" +"\n\n");   
  //---------------------------------------------------------------------------------------
    
  client.stop();                          // disconnect to server
  delay(1000);

  }
}
