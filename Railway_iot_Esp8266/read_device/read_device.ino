
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP8266Ping.h>
#include "emedwifi.h"
//const char* ssid = "SHUBHAM BOYS HOSTAL";
//const char* password = "Shubham@99";
unsigned long sendStatus;
int sw1 = 12;
int sw2 = 13;
int sw3 = 14;

int deviceStatus1 = 5;
int deviceStatus2 = 4;
int deviceStatus3 = 2;

int wifiStatus = 15;
int internet = 3;
const IPAddress remote_ip(142, 250, 192, 46); // Remote host

void setup () {

  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(deviceStatus1, OUTPUT);
  pinMode(deviceStatus2, OUTPUT);
  pinMode(deviceStatus3, OUTPUT);
  pinMode(sw1, OUTPUT);
  pinMode(sw2, OUTPUT);
  pinMode(sw3, OUTPUT);
  pinMode(wifiStatus, OUTPUT);
  pinMode(wifiStatus, OUTPUT);
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

  StaticJsonDocument<200> doc1;
  String payload1 = "";
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    digitalWrite(wifiStatus, HIGH);
    HTTPClient http;  //Declare an object of class HTTPClient

    http.begin("http://api.suryavanshiservices.com/read.php");  //Specify request destination
    int httpCode = http.GET();                                  //Send the request
    Serial.println(httpCode);
    if (httpCode > 0) { //Check the returning code

      payload1 = http.getString();   //Get the request response payload
      Serial.println(payload1);
    }
    //Print the response payload
    if (millis() - sendStatus > 10000) {
      dStatus();

    }


    Serial.print("Pinging ip ");
    Serial.println(remote_ip);

    // Ping
    if (Ping.ping(remote_ip)) {
      Serial.println("Success!!");
      digitalWrite(internet, HIGH);
    } else {
      Serial.println("Error :(");
      digitalWrite(internet, LOW);
    }

    http.end();
  }
  //Close connection

  DeserializationError error = deserializeJson(doc1, payload1);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  //parse data
  int Device_1 = doc1["feeds"][0]["Device1"];
  int Device_2 = doc1["feeds"][1]["Device2"];
  int Device_3 = doc1["feeds"][2]["Device3"];
  //print data

  Serial.print("Device 1 :");
  Serial.println( Device_1);
  Serial.print("Device 2 :");
  Serial.println( Device_2);
  Serial.print("Device 3 :");
  Serial.println(Device_3);

  if (Device_1 == 1) {
    digitalWrite(sw1, HIGH);
  }
  else {
    digitalWrite(sw1, LOW);
  }

  if (Device_2 == 1) {
    digitalWrite(sw2, HIGH);
  }
  else {
    digitalWrite(sw2, LOW);
  }

  if (Device_3 == 1) {
    digitalWrite(sw3, HIGH);
  }
  else {
    digitalWrite(sw3, LOW);
  }



  delay(1000);
}


void dStatus() {

  StaticJsonDocument<200> doc2;
  String payload2 = "";

  HTTPClient http;
  http.begin("http://api.suryavanshiservices.com/dStatus.php");
  Serial.print("Sending device status to server");
  int httpCode = http.GET();                                  //Send the request
  Serial.println(httpCode);
  sendStatus = millis();

  if (httpCode > 0) { //Check the returning code

    payload2 = http.getString();   //Get the request response payload
    Serial.println(payload2);
  }


  DeserializationError error = deserializeJson(doc2, payload2);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  int Status_1 = doc2["feeds"][0]["Status1"];
  int Status_2 = doc2["feeds"][1]["Status2"];
  int Status_3 = doc2["feeds"][2]["Status3"];

  Serial.print("Status 1 :");
  Serial.println(Status_1);
  Serial.print("Status 2 :");
  Serial.println(Status_2);
  Serial.print("Status 3 :");
  Serial.println(Status_3);


  if (Status_1 == 1) {
    digitalWrite(deviceStatus1, HIGH);

  }
  else {
    digitalWrite(deviceStatus1, LOW);
  }


  if (Status_2 == 1) {
    digitalWrite(deviceStatus2, HIGH);

  }
  else {
    digitalWrite(deviceStatus2, LOW);
  }


  if (Status_3 == 1) {
    digitalWrite(deviceStatus3, HIGH);
  }
  else {
    digitalWrite(deviceStatus3, LOW);
  }
}
