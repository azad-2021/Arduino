
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "emedwifi1.h"
#include <ESP8266Ping.h>

int wifiStatus = 12;
int internet = 13;
int sw = 14;
//const char* ssid = "SHUBHAM BOYS HOSTAL";
//const char* password = "Shubham@99";
const int deviceStatus = 5;
const IPAddress remote_ip(142, 250, 192, 46); // Remote host
unsigned long sendStatus;
void setup () {

  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(deviceStatus, INPUT);
  pinMode(wifiStatus, OUTPUT);
  pinMode(internet, OUTPUT);
  pinMode(sw, OUTPUT);
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }

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

  StaticJsonDocument<200> doc;
  String payload = "";
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    digitalWrite(wifiStatus, HIGH);

    // Ping
    if (Ping.ping(remote_ip)) {
      Serial.println("Success!!");
      digitalWrite(internet, HIGH);
    } else {
      Serial.println("Error :(");
      digitalWrite(internet, LOW);
    }


    HTTPClient http;  //Declare an object of class HTTPClient
    int Status  = digitalRead(deviceStatus);
    String url = "";
    if (Status == 1) {
      url = "http://api.suryavanshiservices.com/writeDevice1.php?Value=1";
      digitalWrite(sw, HIGH);
    }
    else {
      url = "http://api.suryavanshiservices.com/writeDevice1.php?Value=0";
      digitalWrite(sw, LOW);
    }
    http.begin(url);  //Specify request destination
    int httpCode = http.GET();                                  //Send the request
    Serial.println(httpCode);
    if (httpCode > 0) { //Check the returning code

      payload = http.getString();   //Get the request response payload
      Serial.println(payload);             //Print the response payload

    }

    if (millis() - sendStatus > 10000) {
      http.begin("http://api.suryavanshiservices.com/d1Status.php?Value=1");
      Serial.println("Sending device status to server");
      int httpCode = http.GET();                                  //Send the request
      Serial.println(httpCode);
      sendStatus = millis();

    }

    http.end();   //Close connection

  }
 
}
