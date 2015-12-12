#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "HX711.h"

/////////////////////
// Pin Definitions //
/////////////////////
const int LED_PIN = 5; // Thing's onboard, green LED
const int LOADCELL_DATA = 12;
const int LOADCELL_CLK = 13;

ESP8266WebServer server(80);
HX711 scale(LOADCELL_DATA, LOADCELL_CLK);

const char pageFirstPart[]="<html>\
  <head>\
    <title>447 Project</title>\
  </head>\
  <body>\
";

const char pageEnding[]="</body>\
</html>";


void handleRoot()
{
 
  String response = pageFirstPart;

  response += pageEnding;
  
  server.send(200, "text/html", response);
}

void loop()
{
  server.handleClient();
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  WiFi.mode(WIFI_AP);
  WiFi.softAP("FLL447");  // no password

  server.on ( "/", handleRoot );
 
  server.begin();
}
