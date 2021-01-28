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
    LOGLN(error.c_str());
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
    LOGLN_STR("ON");
    lightOn();
    return;
  }
  if (command == "OFF") {
    LOGLN_STR("OFF");
    lightOff();
    return;
  }
  if (command == "TOGGLE") {
    LOGLN_STR("TOGGLE");
    lightToggle();
    return;
  }
  if (command == "ON_TIME") {
    LOGLN_STR("ON_TIME");
    lightOnWithTimeOff(config.timeOff);
    return;
  }
  if (command == "FADE_IN") {
    LOGLN_STR("FADE_IN");
    lightFadeInStart(config.fadeTime);
    return;
  }
  if (command == "FADE_OUT") {
    LOGLN_STR("FADE_OUT");
    lightFadeOutStart(config.fadeTime);
    return;
  }
  if (command == "TOGGLE_FADE") {
    LOGLN_STR("TOGGLE_FADE");
    lightToggleFade(config.fadeTime);
    return;
  }
}
