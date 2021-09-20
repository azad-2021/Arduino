
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

WiFiClient wifiClient;

int wifiStatus = 12;
int internet = 13;
int sw = 14;
const char* ssid = "ASUS";
const char* password = "leo greator";
const int deviceStatus = 5;
const IPAddress remote_ip(185, 27, 134, 113); // Remote host
unsigned long sendStatus;
void setup () {

  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(deviceStatus, INPUT);
  pinMode(wifiStatus, OUTPUT);
  pinMode(internet, OUTPUT);
  pinMode(sw, OUTPUT);


  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  String payload = "";
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    digitalWrite(wifiStatus, HIGH);


    HTTPClient http;  //Declare an object of class HTTPClient

    String url = "";
    url = "http://officialas.byethost24.com/aqms/write.php?pm2=10&pm10=30&temp=40&hum=80&p=100&co2=82&btlevel=96&btemp=45&so2=10";

    http.begin(wifiClient, url); //Specify request destination
    int httpCode = http.GET();                                  //Send the request
    Serial.println(httpCode);
    if (httpCode > 0) { //Check the returning code

     // payload = http.getString();   //Get the request response payload
      Serial.println("Temp: 28*C\n");             //Print the response payload
      Serial.println("Hum: 81%\n");
      Serial.println("P: 1012hPa\n");
      Serial.println("PM2.5: 50 ug/m3\n");
      Serial.println("PM10: 39ug/m3\n");
      Serial.println("CO2: 200ug/m3\n");

    }

    if (millis() - sendStatus > 10000) {
      http.begin(wifiClient, "http://officialas.byethost24.com/aqms/write.php?pm2=10&pm10=30&temp=40&hum=80&p=100&co2=82&btlevel=96&btemp=45&so2=10");
      Serial.println("Sending device status to server");
      int httpCode = http.GET();                                  //Send the request
      Serial.println(httpCode);
      sendStatus = millis();

    }

    http.end();   //Close connection

  }
delay(3600000);
}
