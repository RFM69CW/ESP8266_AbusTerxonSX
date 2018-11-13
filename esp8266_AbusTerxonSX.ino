#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson
#include <RFControl.h>

//flag for saving data
bool shouldSaveConfig = false;

WiFiClient espClient;

std::unique_ptr<ESP8266WebServer> server;

void setup() {
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  //needs to be in the beginning for accessing settings reset on reset pin
  WiFiManager wifiManager;
  
  Serial.begin(115200);
  Serial.println();
  //setup gpios
  Serial.println(F("GPIO setup..."));
  pinMode(RESET_PIN, INPUT_PULLUP);

  setupResetHandling(wifiManager, RESET_PIN);
 
  connectToWifi(wifiManager);

  //we start the interaction webserver
  server.reset(new ESP8266WebServer(WiFi.localIP(), 80));

  
  setupAlarmEndpoints();
  
  setupDefaultEndpoints();

  server->begin();
  Serial.println(F("HTTP server started"));

  //make sure softap is off
  WiFi.softAPdisconnect(true);
  
  Serial.println(F("local ip:"));
  Serial.println(WiFi.localIP());
}

void loop() {
  server->handleClient();
}
