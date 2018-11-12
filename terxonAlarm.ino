// states for homebridge
// 1: away armed
// 3: disarmed
// 4: alarm has been triggered
String actualState = "3";

void setupAlarmEndpoints() {
  server->on("/alarm/arm", armAlarm);
  server->on("/alarm/disarm", disarmAlarm);
  server->on("/alarm/check", checkAlarmStatus);
}

void armAlarm() {
   Serial.println(F("Arm alarm system"));
  authenticateUser();
  actualState = "1";
  sendStateToServer(actualState);
}

void disarmAlarm() {
  Serial.println(F("Disarm alarm system"));
  authenticateUser();
  actualState = "3";
  sendStateToServer(actualState);
}

void checkAlarmStatus() {
  Serial.println(F("Check status"));
  authenticateUser();
  
  sendStateToServer(actualState);
}

void sendStateToServer(String actualState) {
  server->send(200, F("text/html"), actualState);
}

