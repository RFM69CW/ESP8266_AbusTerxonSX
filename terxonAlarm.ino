enum StatesTemp {
  armed = 1,
  disarmed = 3,
  alarmActive = 4
};
enum StatesTemp actualState = disarmed;

void setupAlarmGpios(const int ARM_DISARM_PIN, const int ALARM_STATUS_PIN) {
  pinMode(ARM_DISARM_PIN, OUTPUT);
  digitalWrite(ARM_DISARM_PIN, LOW);
  pinMode(ALARM_STATUS_PIN, INPUT);
}

void setupAlarmEndpoints() {
  server->on("/alarm/arm", armAlarm);
  server->on("/alarm/disarm", disarmAlarm);
  server->on("/alarm/check", checkAlarmStatus);
}

void armAlarm() {
  Serial.println(F("Arm alarm system"));
  authenticateUser();
  if(actualState == disarmed) {
    actualState = armed;
  }
  sendStateToServer();
}

void disarmAlarm() {
  Serial.println(F("Disarm alarm system"));
  authenticateUser();
  if(actualState == armed) {
    actualState = disarmed;
  }
  sendStateToServer();
}

void checkAlarmStatus() {
  Serial.println(F("Check status"));
  authenticateUser();
  
  sendStateToServer();
}

void sendStateToServer() {
  server->send(200, F("text/html"), actualState + "");
}

