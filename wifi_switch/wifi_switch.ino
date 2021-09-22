/*Smart Plug - Captive Portal Home Automation
   Requires no Wi-Fi to operate
   Date:19-8-2019
   Author: B.Aswinth Raj
   Website: shivay.com
*/

//Download all required header filers from
#include <ESP8266WiFi.h>
#include "DNSServer.h"
#include <ESP8266WebServer.h>

const byte        DNS_PORT = 53;          // 53 is set as DNS port
IPAddress         apIP(10, 10, 10, 1);    // Network Server
DNSServer         dnsServer;              // DNS server object
ESP8266WebServer  webServer(80);          // Webserver object

int GPIO_2 = 12; //Pin defanition - Relay will be connected to GPIO-0
int GPIO_3 = 14;
/*START OF HMTL CODE*/
String style_detials =  //This String defines the style attributes for webpage
  "<style type=\"text/css\">"
  " body{"
  "  background-color: #830C7E;"
  "}"
  "#buttons{"
  " text-align: center;"
  " display: inline-block;"
  "}"

  ".controllButtons{"
  " margin: 10px;"
  "background-color: white;"
  "padding: 10px;"
  "border:1px solid black;"
  "border-radius: 10px;"
  "cursor: pointer;"
  "font-size: 16px;"
  "}"

  ".controllButtonsActive{"
  " margin: 10px;"
  "background-color: #63D50A;"
  "padding: 10px;"
  "border:1px solid black;"
  "border-radius: 10px;"
  "cursor: pointer;"
  "font-size: 16px;"
  "}"

  
  ".controllButtons:hover{"
  " background-color: orange;"
  "padding: 10px;"
  "border:1px solid black;"
  "border-radius: 10px;"
  "cursor: pointer;"
  "font-size: 16px;"
  "}"



  "@media only screen and (max-width: 1080px) {"
  " button{"
  "  display: inline-block;"
  "}"

  "}"

  "</style>";

String Home_Screen1 = "" //Page 1 - Home Screen HTML code
                      "<!DOCTYPE html><html>"
                      "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                      + style_detials +
                      "<body>"
                      "<div id=\"buttons\"><h3>Welcome to Suryavanshi's World</h3>"
                      "<a style=\"text-decoration:none;\" href=\"relay1_ON\"> <button id=\"switchLight1\" class=\"controllButtons\">Swith 1 ON</button> </a>"
                      "<a style=\"text-decoration:none;\" href=\"relay1_OFF\"><button id=\"switchLight2\" class=\"controllButtons\">Switch 1 OFF</button>  </a>"
                      "<a style=\"text-decoration:none;\" href=\"relay2_ON\"> <button id=\"switchLight3\" class=\"controllButtons\">Swith 2 ON </button> </a>"
                      "<a style=\"text-decoration:none;\" href=\"relay2_OFF\"><button id=\"switchLight4\" class=\"controllButtons\">Switch 2 OFF</button>  </a>"
                      "</div>"
                      "</body>"
                      "</html>";

String ON_Screen1 = "" //Page 2 - If device is turned ON
                    "<!DOCTYPE html><html>"
                    "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                    + style_detials +
                    "<body>"
                    "<div id=\"buttons\"><h3>Welcome to Suryavanshi's World</h3>"
                    "<a style=\"text-decoration:none;\" href=\"relay1_ON\"> <button id=\"switchLight1\" class=\"controllButtonsActive\">Swith 1 ON</button> </a>"
                    "<a style=\"text-decoration:none;\" href=\"relay1_OFF\"><button id=\"switchLight2\" class=\"controllButtons\">Switch 1 OFF</button>  </a>"
                    "<a style=\"text-decoration:none;\" href=\"relay2_ON\"> <button id=\"switchLight3\" class=\"controllButtons\">Swith 2 ON </button> </a>"
                    "<a style=\"text-decoration:none;\" href=\"relay2_OFF\"><button id=\"switchLight4\" class=\"controllButtons\">Switch 2 OFF</button>  </a>"
                    "</div>"
                    "</body>"
                    "</html>";

String OFF_Screen1 = "" //Page 3 - If device is turned OFF
                     "<!DOCTYPE html><html>"
                     "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                     + style_detials +
                     "<body>"
                     "<div id=\"buttons\"><h3>Welcome to Suryavanshi's World</h3>"
                     "<a style=\"text-decoration:none;\" href=\"relay1_ON\"> <button id=\"switchLight1\" class=\"controllButtons\">Swith 1 ON</button> </a>"
                     "<a style=\"text-decoration:none;\" href=\"relay1_OFF\"><button id=\"switchLight2\" class=\"controllButtons\">Switch 1 OFF</button>  </a>"
                     "<a style=\"text-decoration:none;\" href=\"relay2_ON\"> <button id=\"switchLight3\" class=\"controllButtons\">Swith 2 ON </button> </a>"
                     "<a style=\"text-decoration:none;\" href=\"relay2_OFF\"><button id=\"switchLight4\" class=\"controllButtons\">Switch 2 OFF</button>  </a>"
                     "</div>"
                     "</body>"
                     "</html>";

/*END OF HMTL CODE*/


void setup() {
  pinMode(LED_BUILTIN, OUTPUT); //LED pin as output for indication
  pinMode(GPIO_2, OUTPUT); //GPIO pin as output for Relay control
  pinMode(GPIO_3, OUTPUT); //GPIO pin as output for Relay control

  WiFi.mode(WIFI_AP); //Set ESP in AP mode
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Suryavanshi_Smart_Switch"); // Name your AP network

  dnsServer.start(DNS_PORT, "*", apIP);

  webServer.onNotFound([]() {
    webServer.sendHeader("Location", String("http://www.suryavanshi.com/home.html"), true); //Open Home screen by default
    webServer.send ( 302, "text/plain", "");
  });

  webServer.on("/home.html", []() {
    webServer.send(200, "text/html", Home_Screen1);
  });

  //ON_Screen
  webServer.on("/relay1_ON", []() { //If turn on Button is pressed
    digitalWrite(LED_BUILTIN, LOW); //Turn off LED
    digitalWrite(GPIO_2, HIGH);  //Turn off Relay
    webServer.send(200, "text/html", ON_Screen1); //Display this screen
  });
  webServer.on("/relay2_ON", []() { //If turn on Button is pressed
    digitalWrite(LED_BUILTIN, LOW); //Turn off LED
    digitalWrite(GPIO_3, HIGH);  //Turn off Relay
    webServer.send(200, "text/html", ON_Screen1); //Display this screen
  });
  //OFF_Screen
  webServer.on("/relay1_OFF", []() { //If turn off Button is pressed
    digitalWrite(LED_BUILTIN, HIGH); //Turn on LED
    digitalWrite(GPIO_2, LOW); //Turn on Relay
    webServer.send(200, "text/html", OFF_Screen1); //Display this screen
  });
  webServer.on("/relay2_OFF", []() { //If turn off Button is pressed
    digitalWrite(LED_BUILTIN, HIGH); //Turn on LED
    digitalWrite(GPIO_3, LOW); //Turn on Relay
    webServer.send(200, "text/html", OFF_Screen1); //Display this screen
  });



  webServer.begin();
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}
