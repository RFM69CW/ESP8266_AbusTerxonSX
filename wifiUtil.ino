void authenticateUser() {
  if (!server->authenticate(WWW_USERNAME, WWW_PASSWORD)) {
      return server->requestAuthentication();
    }
}

void setupDefaultEndpoints() {
  server->on("/", []() {
    authenticateUser();
    server->send(200, "text/plain", "Login OK");
  });

  server->onNotFound(handleNotFound);
}


void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server->uri();
  message += "\nMethod: ";
  message += (server->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server->args();
  message += "\n";
  for (uint8_t i = 0; i < server->args(); i++) {
    message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
  }
  server->send(404, "text/plain", message);
}

void connectToWifi(WiFiManager &wifiManager) {
   //set static ip
  //wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect(config_ssid,config_password)) {
    Serial.println(F("failed to connect and hit timeout"));
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }
  //if you get here you have connected to the WiFi
  Serial.println(F("\nconnected...yeey :)"));
}

void setupResetHandling(WiFiManager &wifiManager, int resetPin) {
  //if reset is triggered
  if(digitalRead(resetPin) == LOW){
    Serial.println(F("RESET triggered..."));
    //we format the memory
    SPIFFS.format();
    //and reset the ESP settings
    wifiManager.resetSettings();
  }
}
