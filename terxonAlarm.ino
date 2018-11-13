enum StatesTemp {
  armed = 1,
  disarmed = 3,
  alarmActive = 4
};
enum StatesTemp actualState = disarmed;

void setupAlarmGpios(const int ARM_DISARM_PIN, const int ALARM_STATUS_PIN) {
  pinMode(ARM_DISARM_PIN, OUTPUT);
  digitalWrite(ARM_DISARM_PIN, HIGH);
  pinMode(ALARM_STATUS_PIN, INPUT);
  pinMode(ALARM_ACTIVE_PIN, INPUT);
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
    changeAlarmstate();
    actualState = armed;
  } else {
    Serial.println(F("==>Alarm was already armed"));
  }
  sendStateToServer();
}

void disarmAlarm() {
  Serial.println(F("Disarm alarm system"));
  authenticateUser();
  if(actualState == armed) {
    changeAlarmstate();
    actualState = disarmed;
  } else {
    Serial.println(F("==>Alarm was already disarmed"));
  }
  sendStateToServer();
}

void checkAlarmStatus() {
  Serial.println(F("Check status"));
  authenticateUser();
  if(digitalRead(ALARM_STATUS_PIN)) {
    actualState = armed;
  } else {
    actualState = disarmed;
  }

  if(digitalRead(ALARM_ACTIVE_PIN)) {
    Serial.println(F("ALARM!!!"));
    actualState = alarmActive;
  }
  
  sendStateToServer();
}

void sendStateToServer() {
  String message;
  switch(actualState){
    case armed: message = "1"; break;
    case disarmed: message = "3"; break;
    case alarmActive: message = "4"; break;
    default: message = "error"; break;
  }
  server->send(200, F("text/html"), message);
}

void changeAlarmstate() {
  digitalWrite(ARM_DISARM_PIN, LOW);
  delay(TOGGLE_MS);
  digitalWrite(ARM_DISARM_PIN, HIGH);
}

