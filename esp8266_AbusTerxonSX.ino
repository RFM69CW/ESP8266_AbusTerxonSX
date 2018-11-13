#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson
/*
 * Settings
 */
//initial wifi app
const char* config_ssid = "esp8266TerxonSX";
const char* config_password = "terxonAbus";
//HTTP basic auth
const char* WWW_USERNAME = "terxonAlarm";
const char* WWW_PASSWORD = "esp8266";

const int RESET_PIN = 14; //D5 on nodeMCU
#define TOGGLE_MS 500
const int ARM_DISARM_PIN = 4; //D2 on nodeMCU (LOW ACTIVE RELAIS BOARD)
const int ALARM_STATUS_PIN = 5; //D1 on nodeMCU
const int ALARM_ACTIVE_PIN = 12; //D6 on nodeMCU

/*
 * 
 */

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
  setupAlarmGpios(ARM_DISARM_PIN, ALARM_STATUS_PIN);

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
  printAvailableRam();
}

void loop() {
  server->handleClient();
}
