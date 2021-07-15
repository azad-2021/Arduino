/* ESP8266 AWS IoT
    Device 1
   Simplest possible example (that I could come up with) of using an ESP8266 with AWS IoT.
   No messing with openssl or spiffs just regular pubsub and certificates in string constants

   This is working as at 3rd Aug 2019 with the current ESP8266 Arduino core release:
   SDK:2.2.1(cfd48f3)/Core:2.5.2-56-g403001e3=20502056/lwIP:STABLE-2_1_2_RELEASE/glue:1.1-7-g82abda3/BearSSL:6b9587f

   Author: Anant Singh Suryavanshi
   License: Apache License v2
   https://github.com/sborsay
   Add in EOF certificate delimiter
   Add in Char buffer utilizing sprintf to dispatch JSON data to AWS IoT Core
   First 9 chars of certs obfusicated, use your own, but you can share root CA / x.509 until revoked
*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
int power = 12;
int Data = 0;
int switchStatus = 13;
int lastSwstatus = 0;
int currState = 0;
extern "C" {
#include "libb64/cdecode.h"
}

const char* ssid = "Shubham boys hostel_4G";
const char* password = "987654321@";

// Find this awsEndpoint in the AWS Console: Manage - Things, choose your thing
// choose Interact, its the HTTPS Rest endpoint.  Endpoints ARE regional
const char* awsEndpoint = "a3irubq5su94at-ats.iot.ap-south-1.amazonaws.com";

// For the two certificate strings below paste in the text of your AWS
// device certificate and private key, comment out the BEGIN and END
// lines, add a quote character at the start of each line and a quote
// and backslash at the end of each line:

// xxxxxxxxxx-certificate.pem.crt
const String certificatePemCrt = \
                                 //-----BEGIN CERTIFICATE-----
                                 R"EOF(
MIIDWjCCAkKgAwIBAgIVANPx53DhAfaEGNOVNNQNw/0Bl9UQMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMTAzMTAwNjU4
NTlaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDdXbjZ5Nthoe3Ci0wn
VOtRkUL5kQWrhyFuY3ZMFxXcQIkXfItj4R61W5nwhfh9qboOimRzVarYPbSX1b/n
9yAaMmWlwd0G4sbs0oE6az2GhqWoyuDhKBf1N0OKsIG9mrK/WdBIyhwRPvMkP4vL
p05zeSLtqB4UgGw2gzSr1rxYnSKSaxT2LBG3m2JFG2iC6aS0fG2iBgMNLi7CAsV6
3zQQO6YzdpqqAv+/4YOSuUSEjyyIDj97BzIPFeHXpsR5P1pHvZhQ+1nUotaFpV+N
U/IJomgRdchEjlXS36WIJGahSB3z4NKUqEYHQnuO/lhGpPV5ZXqKi26m2WuQK880
9BP7AgMBAAGjYDBeMB8GA1UdIwQYMBaAFIpcowrswbmlzezggu2oikOFUq/6MB0G
A1UdDgQWBBQwwzNE9e1NHKqHkTjMvDmdABuaYjAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEACOyqa7lVaUu48EeXjbaTFNhu
rEGP7XrmgdjK6TEVGCYstnL9PEvA+BbF30dUB6LllaWogz1J7Pqm4JgcweZCJ/Ul
btHb6inK9PZ4y9sz21iO7TvJWXYlLgaGA81ISkwglixEgHatXGOmiwOM/zqyac+Y
cR0HqUVJZQcm9F9v9PT1IRiorj/76pmXeNgwZZBqyO354cWqKZfzh9bc6knn9R02
+INFbC+AHe/V9A9BKV9HDGBl67NByXkdsX3uyQCAuwG7Jw9jgmonboikQUkVY265
cso/h3yM5/a8M3rLRRm3rMzKx+8z4m8vCOf86qmesJHaGgjWR9twUXtq05MsJQ==

)EOF"; 
//-----END CERTIFICATE-----

// xxxxxxxxxx-private.pem.key
const String privatePemKey = \
//-----BEGIN RSA PRIVATE KEY-----
R"EOF(

MIIEpgIBAAKCAQEA3V242eTbYaHtwotMJ1TrUZFC+ZEFq4chbmN2TBcV3ECJF3yL
Y+EetVuZ8IX4fam6Dopkc1Wq2D20l9W/5/cgGjJlpcHdBuLG7NKBOms9hoalqMrg
4SgX9TdDirCBvZqyv1nQSMocET7zJD+Ly6dOc3ki7ageFIBsNoM0q9a8WJ0ikmsU
9iwRt5tiRRtogumktHxtogYDDS4uwgLFet80EDumM3aaqgL/v+GDkrlEhI8siA4/
ewcyDxXh16bEeT9aR72YUPtZ1KLWhaVfjVPyCaJoEXXIRI5V0t+liCRmoUgd8+DS
lKhGB0J7jv5YRqT1eWV6iotuptlrkCvPNPQT+wIDAQABAoIBAQDWQYhRHblY3ioK
SlkDGKZK5JY2h689PiWVep9dNlrqO2x1Vw5zNclLL8Xnj0PcHRMAT+K53wjlE5Rg
AXslQYPI106bCPCtYYG4Zma97F44fsqIfRT72JqXtU2iRwTMR5cKrhty2GwiJROg
2gCstNqx2WV9MWMr5MDYYlXwnH7gJCohxeaB0uTvvCexSL+y4uAPUvLxA2Nq+62Q
risu4gkn6UDrfCcBDQZ0T8+NhatylssmtijEG4htQxnmAbYVvhPJr/QGEn36jHpZ
5tNLioSxXnrVosTK+1eHNS9Hxw00h+wFmmjLytiv8denAMSApRC3MWwpWPgaqiK3
pHS4i8/BAoGBAPBnZeJcUfVTIzB0WxekUpN69mlGedIaX9oHdvkiW5ZicH/sE4O6
3Ru7n6Sc2GnKt/H2m2nlZJH0RnaI+LkZnjEg2uW6Enqlq0TYlj4mde1A5bhCNKAR
dW3dpSq36oV+zOaJtN6x7zc8WneCQbuT9Ih8+Ksr9a2/+9yr3qluortfAoGBAOu6
JXuqoqXLGqFodsNgDHtG8UvKr0TPro8NiisWHKdr61X6r6GE8PmcZgQqUqkZuQSH
0meTOowuQ8iDnX9I0YUSmDCD1ZSUyfXuqR3Gs9n9IBR+pQqy4wrApZzSyJDbG3wQ
35n5kSpzbo4zUUcJPjkFEbdWTIWTp2yYqEZdKIjlAoGBAJupJ8RZKMxjp55hSPZW
9Qz/inwCITEU2MmxGa0ZPRpz5Qo/jyc1oKOraiRJVquxkkd++YSo/uSuuc1YxUv9
NXv0HSiwwfRePzaV0zqs5qDX66bSX4bhpxqbcK3nZlhIEV6Y/d9QcWdfkeheFDfk
Bms2GEGldi+p5ugs9UDaHU6bAoGBAK2tgrxw5lE9KavOqeIdPFT/2fy7oxMR6Uzg
7vH74rS/4K6CezCIIHvobYmYHTsWBql94NoLMdHirHJp9nq3jhd7scHdAFnunjai
1ttncU+lHiO7g6I0Vu+vLNY0vy1rIf7w964coIYkcko3KSUrfDrKr1jjLpIkTfDH
MQRW0uy5AoGBAJUrWmhYSkL6pixpo72rLW1epwINS/MQ9x128fbVVq7QRW97/ZwJ
I1iDpTt3fgG5Nu5sV3sQl4hloirVfSitfDMIVP+TRk618lcdpf6MH+gasnjfBVHL
7BpMn2s2PCAmn3kePdIzYNtUu8XohzOTj4PKdwkqux30XONFQv9iAaTh

)EOF";
//-----END RSA PRIVATE KEY-----

// This is the AWS IoT CA Certificate from: 
// https://docs.aws.amazon.com/iot/latest/developerguide/managing-device-certs.html#server-authentication
// This one in here is the 'RSA 2048 bit key: Amazon Root CA 1' which is valid 
// until January 16, 2038 so unless it gets revoked you can leave this as is:
const String caPemCrt = \
//-----BEGIN CERTIFICATE-----

"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF"\
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6"\
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL"\
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv"\
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj"\
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM"\
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw"\
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6"\
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L"\
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm"\
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC"\
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA"\
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI"
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs"\
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv"\
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU"\
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy"\
"rqXRfboQnoZsG4q5WTP468SQvvG5";
//-----END CERTIFICATE-----

WiFiClientSecure wiFiClient;
void msgReceived(char* topic, byte* payload, unsigned int len);
PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, wiFiClient); 

void setup() {
   pinMode(power, OUTPUT);
   pinMode(LED_BUILTIN, OUTPUT);
   //pinMode(power, OUTPUT);
   pinMode(switchStatus, INPUT);

  Serial.begin(115200); Serial.println();
  Serial.println("ESP8266 AWS IoT Example");

  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());

  // get current time, otherwise certificates are flagged as expired
  setCurrentTime();

  uint8_t binaryCert[certificatePemCrt.length() * 3 / 4];
  int len = b64decode(certificatePemCrt, binaryCert);
  wiFiClient.setCertificate(binaryCert, len);
  
  uint8_t binaryPrivate[privatePemKey.length() * 3 / 4];
  len = b64decode(privatePemKey, binaryPrivate);
  wiFiClient.setPrivateKey(binaryPrivate, len);

  uint8_t binaryCA[caPemCrt.length() * 3 / 4];
  len = b64decode(caPemCrt, binaryCA);
  wiFiClient.setCACert(binaryCA, len);
}

unsigned long lastPublish;
int msgCount;

void loop() {
  
  pubSubCheckConnect();
  delay(100);
  //Add a JSON package of fake data to deliver to AWS IoT
  //Uses snprintf but other viable options are: sprintf, strcpy, strncpy, or
  //Use the ArduinoJson library for Efficient JSON serialization
  //If you need to increase buffer size, then you need to change MQTT_MAX_PACKET_SIZE in PubSubClient.h
  char pubData[10];

  //float var1 =  random(55,77); //fake number range, adjust as you like
  //float var2 =  random(77,99);
  

 // String msg = String("Hello from ESP8266: ") + ++msgCount;
  currState = digitalRead(switchStatus);
  if (lastSwstatus != currState) {

   
   if (currState == HIGH) {
      Serial.println("on");
      Data = 1;
    } else {
      Serial.println("off");
      Data = 0;
    }
    delay(50);

    sprintf(pubData,  "{\"uptime\":%lu,\"id\":1,\"Switch\":%d}", millis() / 1000, Data);
    pubSubClient.publish("outTopic", pubData);
    delay(100);
    Serial.print("Published: "); Serial.println(pubData);
  }

   

  

   //char Status[10];
   //sprintf(Status,  "{\"uptime\":%lu,\" Device Ststus\":1}", millis() / 1000);


  if (millis() - lastPublish > 10000) {
    sprintf(pubData,  "{\"uptime\":%lu,\"id\":1,\"Switch\":%d}", millis() / 1000, Data);
    pubSubClient.publish("outTopic", pubData);
    //delay(100);
    Serial.print("Published: "); Serial.println(pubData);
    lastPublish = millis();
   
  }
  lastSwstatus = currState; 
  // yield();
  //delay(5000);
}



void msgReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on "); Serial.print(topic); Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void pubSubCheckConnect() {
  if ( ! pubSubClient.connected()) {
    Serial.print("PubSubClient connecting to: "); Serial.print(awsEndpoint);
    while ( ! pubSubClient.connected()) {
      yield();
      Serial.print(".");
      pubSubClient.connect("ESPthing");
    }
    Serial.println(" connected");
    digitalWrite(LED_BUILTIN, LOW);
   Serial.println(pubSubClient.subscribe("inTopic"));

  }
  pubSubClient.loop();
}

int b64decode(String b64Text, uint8_t* output) {
  base64_decodestate s;
  base64_init_decodestate(&s);
  int cnt = base64_decode_block(b64Text.c_str(), b64Text.length(), (char*)output, &s);
  return cnt;
}

void setCurrentTime() {
  configTime(3 * 3600, 0,"pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: "); Serial.print(asctime(&timeinfo));
}
