#include <DHT.h>            // declare using library DHT
#include <ESP8266WiFi.h>    //declare using library ESP8266WiFi.h to set up HTTP client mode for board ESP8266
#define DHTPIN 5           // data pins of DHT11 connect to GPI005 of ESP8266
#define DHTTYPE DHT11       // Type of library DHT be used

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;          // create an variable client has datatype WiFiClient
const char* ssid = "yyy";      // the wireless network name (SSID)
const char* password = "VangY@0306";  // Password of Wifi network will be connected 
const char* server = "192.168.43.36";     // IP address of computer when connecting with the wifi network
const int port = 8000;                    // Port of opened server 
const int sendingInternval = 2 * 1000;    // variable update data after every 2s


void setup() {
  Serial.begin(115200);
  dht.begin();                            // Create DHT11 to send - recieve data
  Serial.println("Connecting");

  // set ESP8266 is Station and connest to Wifi. display the dots `.` on terminal if it is not connected
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
}
void loop() {
// Read value of Temperature(celcius),Humidity.If data is not a number, displaying error and exit.
  float temp = dht.readTemperature();
  float humi = dht.readHumidity();
  if (isnan(temp) || isnan(humi)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (client.connect(server, port)) {       // create an connect to server through opened IP and Port.
  //---------------------------------------------------------------------------------------
    String req_uri = "/update?temp=" + String(temp, 1) + "&humd=" + String(humi, 1);
    client.print("GET " + req_uri + " HTTP/1.1\n" + "Host: "+ server +"\n" +
                 "Connection: close\n" + "Content-Length: 0\n" +"\n\n");   
  //---------------------------------------------------------------------------------------

  // temp, humi change from "float" into "string" and display serial     // terminal on Arduino.
    Serial.printf("Nhiet do %s - Do am %s\r\n", String(temp, 1).c_str(), String(humi, 1).c_str());
  }
  client.stop();                          // disconnect to server

  delay(sendingInternval);
}
