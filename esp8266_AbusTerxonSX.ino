#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson
#include <DHT.h>
#include <PubSubClient.h>
#include <Ticker.h>
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
#define TOGGLE_MS 3000
const int ARM_DISARM_PIN = 4; //D2 on nodeMCU (LOW ACTIVE RELAIS BOARD)
const int ALARM_STATUS_PIN = 5; //D1 on nodeMCU
const int ALARM_ACTIVE_PIN = 12; //D6 on nodeMCU

//MQTT DHT22
#define SENSORLOCATION "hall"
#define REPORT_DELAY_SECS 30
#define MQTT_HOST "192.168.178.179"
#define MQTT_PORT 1883
#define MQTT_CLIENT "ESP8266_dht22_" SENSORLOCATION
#define MQTT_TOPIC_H1 SENSORLOCATION "/hum" 
#define MQTT_TOPIC_T1 SENSORLOCATION "/temp"
#define MQTT_TOPIC_ACTIVE SENSORLOCATION "/sensor/active"

/*
 * 
 */

WiFiClient espClient;
PubSubClient client(espClient); 
DHT dht22(D3, DHT22); //DHT22 wired to D3
Ticker blinker;
int timerTick = 0;

std::unique_ptr<ESP8266WebServer> server;

void ICACHE_RAM_ATTR onTimerISR(){
  timerTick++;
  if((2 * timerTick) > (REPORT_DELAY_SECS * 1000)) {
    timerTick = 0;
    if (!client.connected()) {
      mqttConnect();
    }
    client.loop(); 
    readAndPublishDHT22();
  }
 
  timer1_write(600000);//12us
}

void initTimer() {
  Serial.println(F("init timer"));
  //Initialize Ticker every 0.5s
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  timer1_write(600000); //120000 us
}

void setup() {
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  //needs to be in the beginning for accessing settings reset on reset pin
  WiFiManager wifiManager;
  //wifiManager.resetSettings(); //to change accesspoint
  Serial.begin(115200);
  Serial.println();
  //setup gpios
  Serial.println(F("GPIO setup..."));
  pinMode(RESET_PIN, INPUT_PULLUP);
  setupAlarmGpios(ARM_DISARM_PIN, ALARM_STATUS_PIN);

  setupResetHandling(wifiManager, RESET_PIN);
 
  connectToWifi(wifiManager);

  client.setServer(MQTT_HOST, MQTT_PORT); 


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
  Serial.println(F("Start DHT22"));
  dht22.begin();
  initTimer();
  printAvailableRam();
}

void loop() {
  server->handleClient();
}


