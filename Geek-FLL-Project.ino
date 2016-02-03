#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
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
  <head>";
  
const char pageSecondPart[]="\
  </head>\
  <body>";
  
const char pageEnding[]="</body>\
</html>";

char result[20];

const float Filling_weight = 500.0f;  // to be changed
const float Pickup_weight = 1000.0f; // to be changed

void handle_weight()
{
  String filename;
  digitalWrite(LED_PIN, LOW);
  float weight = scale.get_units(5);
  if(weight < Filling_weight){
    filename = "/Empty.html";
  }
  else if(weight < Pickup_weight){
    filename = "/Filling.html";
  }
  else{
    filename = "/Pickup.html";
  }
 File file = SPIFFS.open(filename, "r");
 size_t sent = server.streamFile(file, "text/html");
 file.close();
 digitalWrite(LED_PIN, HIGH);
}

void handleCalib()
{
  String response = pageFirstPart;
  response += "<title>weight</title>";
  response += pageSecondPart;

  response += "<h1>Our Geeky Prototype</h1>";
  response += "<p><a href = '/tare'>Tare Here</a></p>";
   
  dtostrf(scale.get_value(), 1, 2, result);
  response += "raw result = ";
  response += result;
  
  dtostrf(scale.get_units(), 1, 2, result);
  
  response += "<p/>calib weight = ";
  response += result;
  
  response += pageEnding;
  
  server.send(200, "text/html", response);
}

void handleTare() 
{
  scale.tare();
  handleCalib();
}

void loop()
{
  server.handleClient();
}

//const float calibration = 2280.f; // replace once prototype is made
const float calibration = 154.f;  // came from measured known weight

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  SPIFFS.begin();

  WiFi.mode(WIFI_AP);
  WiFi.softAP("FLL447");  // no password
  // setup scale
  scale.tare();
  scale.set_scale(calibration);
  server.on ( "/", handle_weight );
  server.on ( "/calib", handleCalib );
  server.on ( "/tare", handleTare );
 
  server.begin();
}
