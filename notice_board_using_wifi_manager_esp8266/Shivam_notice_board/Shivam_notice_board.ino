// Problem >> Reset IP after restart and can't open webpage

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include "html.h"

char static_ip[16] = "192.168.29.222";
char static_gw[16] = "192.168.1.1";
char static_sn[16] = "255.255.255.0";


/*
  IPAddress local_IP(192, 168, 1, 184);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 0, 0);
  IPAddress primaryDNS(8, 8, 8, 8);   //optional
  IPAddress secondaryDNS(8, 8, 4, 4); //optional

  const char* ssid     = "Shubham boys hostel_4G";
  const char* password = "9876544321@";
*/
ESP8266WebServer server(80);



void setup() {

  Serial.begin(115200);
  Serial.println();
  
  staticIp();

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();

}



void loop() {
  server.handleClient();//Checks for web server activity

}



void handleRoot() {
  if (server.hasArg("drum") && server.hasArg("O2") && server.hasArg("shs_temp") && server.hasArg("shs_prs") && server.hasArg("fr1_temp") && server.hasArg("fr2_temp")) { //If all form fields contain data call handelSubmit()
    handleSubmit();
  }
  else {//Redisplay the form
    server.send(200, "text/html", INDEX_HTML);
  }
}



void handleSubmit() { //dispaly values and write to memmory
  String response = "<p>Data 1 is";
  response += server.arg("drum");
  response += "<br>";
  response += "And Data 2 is ";
  response += server.arg("O2");
  response += "<br>";
  response += "And the Data 3 is ";
  response += server.arg("shs_temp");
  response += "<br>";
  response += "And the Data 4 is ";
  response += server.arg("shs_prs");
  response += "<br>";
  response += "And Data 5 is ";
  response += server.arg("fr1_temp");
  response += "<br>";
  response += "And the Data 6 is ";
  response += server.arg("fr2_temp");
  response += "</P><BR>";
  response += "<H2><a href=\"/\">go home</a></H2><br>";

  server.send(200, "text/html", response);
  //calling function that writes data to memory
  Print_to_serial(String(server.arg("drum")), String(server.arg("O2")), String(server.arg("shs_temp")), String(server.arg("shs_prs")), String(server.arg("fr1_temp")), String(server.arg("fr2_temp")));
}


void Print_to_serial(String d, String O, String st, String sp, String ft1, String ft2) {

  Serial.println(d);
  Serial.println(O);
  Serial.println(st);
  Serial.println(sp);

  Serial.println(ft2);
  Serial.println(ft1);
}



void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  message += "<H2><a href=\"/\">go home</a></H2><br>";
  server.send(404, "text/plain", message);
}

/*
  void wifi_setup()
  {

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");

  }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  return;

  }*/


void staticIp() {


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
