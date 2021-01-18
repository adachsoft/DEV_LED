#ifdef MOTION_PIR

#define MOTION_PIR_PERIOD_LOOP 300
#define MOTION_PIR_PIN config.motionPirPin
#define MQTT_TOPIC_MOTION_PIR "tele/%topic%/SENSOR/MOTION_PIR"

long millisMotionPir;
bool lastStateMotionPir;
String mqttTopicMotionPir;

void setupMotionPir()
{
  pinMode(MOTION_PIR_PIN, INPUT);
  millisMotionPir = 0;
  lastStateMotionPir = digitalRead(MOTION_PIR_PIN);

  mqttTopicMotionPir = MQTT_TOPIC_MOTION_PIR;
  mqttTopicMotionPir.replace("%topic%", config.mqttTopic);

  //strcpy(config.motionPirCmndStateH, "{\"current_light\":500,\"time_off\":30}");
  //strcpy(config.mqttTopicMotionPirH, "cmnd/dev_led2/POWER");
  //strcpy(config.mqttMessageMotionPirH, "{\"current_light\":500,\"time_off\":3}");
}

void loopMotionPir()
{
  if (currentMillis - millisMotionPir <= MOTION_PIR_PERIOD_LOOP) {
    return;
  }
  millisMotionPir = currentMillis;

  bool state = digitalRead(MOTION_PIR_PIN);
  if (lastStateMotionPir != state) {
    lastStateMotionPir = state;
    handelMotionPirStateChange(state);
    
    LOG_STR("PIR change state: ");
    LOGLN(state); 
  }
}

void handelMotionPirStateChange(bool state)
{
  StaticJsonDocument<128> jsonMessage;
  jsonMessage["TIME"] = formattedDateTime;
  jsonMessage["PIR"]["STATE"] = state;
  
  char buffer[128];
  serializeJson(jsonMessage, buffer);
  mqttClient.publish((char*) mqttTopicMotionPir.c_str(), buffer);

  if (state == HIGH) {
    handelStateH();
  } else {
    handelStateL();
  }
}

void handelStateH()
{
  if (strlen(config.motionPirCmndStateH) > 0) {
    powerCommand(String(config.motionPirCmndStateH));
  }

  if (strlen(config.mqttTopicMotionPirH) > 0) {
    mqttClient.publish(config.mqttTopicMotionPirH, config.mqttMessageMotionPirH);
  }
}

void handelStateL()
{
  if (strlen(config.motionPirCmndStateL) > 0) {
    powerCommand(String(config.motionPirCmndStateL));
  }

  if(strlen(config.mqttTopicMotionPirL) > 0) {
    mqttClient.publish(config.mqttTopicMotionPirL, config.mqttMessageMotionPirL);
  }
}

#endif
