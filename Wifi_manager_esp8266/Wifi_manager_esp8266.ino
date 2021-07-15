#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#define trigger 5
#define LED 13
#define power 14

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(trigger, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(power, OUTPUT);
  //trigger = HIGH;
  //staticIp();
  dynamicIp();
}
void loop() {
  if (WiFi.status() == WL_CONNECTED)
  {
    digitalWrite(power, LOW);
    while (WiFi.status() == WL_CONNECTED) {
      digitalWrite(LED, HIGH);
      delay(500);
      digitalWrite(LED, LOW);
      delay(200);
    }
  }
  else {
    digitalWrite(LED, LOW);
  }
}

void staticIp() {

  char static_ip[16] = "192.168.1.184";
  char static_gw[16] = "192.168.1.1";
  char static_sn[16] = "255.255.255.0";

  WiFiManager wifiManager;
  wifiManager.resetSettings();
  IPAddress _ip, _gw, _sn;
  _ip.fromString(static_ip);
  _gw.fromString(static_gw);
  _sn.fromString(static_sn);


  wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn);

  wifiManager.autoConnect("WiFi Manager");
  Serial.println("connected :)");



}


void dynamicIp() {


  WiFiManager wifiManager;
  wifiManager.resetSettings();
  wifiManager.autoConnect(" WiFi Manager");
  Serial.println("connected :)");



}
