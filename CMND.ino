void powerCommand(String message)
{
  if (message.length() == 0) {
    return;
  }
  if (message[0] == '{') {
    powerCommandJSON(message);
    return;
  }

  powerCommandSimple(message);
}

void powerCommandJSON(String message)
{
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, message);
  if (error) {
    Serial.println(error.c_str());
    return;
  }

  if (doc.containsKey("time_off")) {
    config.timeOff = doc["time_off"];
    lightTimeOff = 1000 * config.timeOff;
    lightMillis = currentMillis;
  }

  if (doc.containsKey("light")) {
    config.light = doc["light"];
  }

  if (doc.containsKey("current_light")) {
     lightPWM(doc["current_light"]);
  }

  if (doc.containsKey("fade_time")) {
    config.fadeTime = doc["fade_time"];
  }

  if (doc.containsKey("power")) {
    powerCommandSimple(doc["power"]);
  }
}

void powerCommandSimple(String command)
{
  if (command == "ON") {
    Serial.println("ON");
    lightOn();
    return;
  }
  if (command == "OFF") {
    Serial.println("OFF");
    lightOff();
    return;
  }
  if (command == "TOGGLE") {
    Serial.println("TOGGLE");
    lightToggle();
    return;
  }
  if (command == "ON_TIME") {
    Serial.println("ON_TIME");
    lightOnWithTimeOff(config.timeOff);
    return;
  }
  if (command == "FADE_IN") {
    Serial.println("FADE_IN");
    lightFadeInStart(config.fadeTime);
    return;
  }
  if (command == "FADE_OUT") {
    Serial.println("FADE_OUT");
    lightFadeOutStart(config.fadeTime);
    return;
  }
  if (command == "TOGGLE_FADE") {
    Serial.println("TOGGLE_FADE");
    lightToggleFade(config.fadeTime);
    return;
  }
}
