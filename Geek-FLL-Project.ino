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
float calibration_factor = 2000; // needs to come from calibration sketch

void setupScale() {
  scale.tare();  //Reset the scale to 0
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
}

float getScaleWeight(){
  return scale.get_units(3);  // average of 3 readings
}

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
  float weight = getScaleWeight();
  
  String response = pageFirstPart;
  response += "Weight: " + String(weight, 3);  // 3 is for number of decimal places
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
  setupScale();

  WiFi.mode(WIFI_AP);
  WiFi.softAP("FLL447");  // no password

  server.on ( "/", handleRoot );
 
  server.begin();
}
