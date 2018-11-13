
void printAvailableRam() {
  Serial.print("Available Ram: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println();
}

