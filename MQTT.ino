#define MQTT_RECONNECT_TIME 30000

void setupMQTT()
{
  mqttTopicPower = MQTT_TOPIC_POWER;
  mqttTopicPower.replace("%topic%", config.mqttTopic);

  mqttTopicSensor = MQTT_TOPIC_SENSOR;
  mqttTopicSensor.replace("%topic%", config.mqttTopic);

  mqttTopicConfigStat = MQTT_TOPIC_CONFIG_STAT;
  mqttTopicConfigStat.replace("%topic%", config.mqttTopic);

  mqttTopicConfigCmnd = MQTT_TOPIC_CONFIG_CMND;
  mqttTopicConfigCmnd.replace("%topic%", config.mqttTopic);

  mqttTopicConfigModuleStat = MQTT_TOPIC_CONFIG_MODULE_STAT;
  mqttTopicConfigModuleStat.replace("%topic%", config.mqttTopic);

  mqttTopicConfigModuleCmnd = MQTT_TOPIC_CONFIG_MODULE_CMND;
  mqttTopicConfigModuleCmnd.replace("%topic%", config.mqttTopic);

  #ifdef DS18B20_MODULE
  mqttTopicConfigTempStat = MQTT_TOPIC_CONFIG_TEMP_STAT;
  mqttTopicConfigTempStat.replace("%topic%", config.mqttTopic);

  mqttTopicConfigTempCmnd = MQTT_TOPIC_CONFIG_TEMP_CMND;
  mqttTopicConfigTempCmnd.replace("%topic%", config.mqttTopic);
  #endif

  #ifdef MOTION_PIR
  mqttTopicConfigMotionPirStat = MQTT_TOPIC_CONFIG_MOTION_PIR_STAT;
  mqttTopicConfigMotionPirStat.replace("%topic%", config.mqttTopic);

  mqttTopicConfigMotionPirCmnd = MQTT_TOPIC_CONFIG_MOTION_PIR_CMND;
  mqttTopicConfigMotionPirCmnd.replace("%topic%", config.mqttTopic);
  #endif

  if (mqttClient.setBufferSize(1024)) {
    LOGLN("setBufferSize Y");
  } else {
    LOGLN("setBufferSize N");
  }
  mqttClient.setServer(config.mqttServer, 1883);
  mqttClient.setCallback(mqttCallback);
}

void loopMQTT()
{
  if (currentMillis - mqttMillis > MQTT_RECONNECT_TIME && !mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();
}

void reconnectMQTT()
{
  if (!isWIFIOn) {
    return;
  }
  
  String clientId = "ESP8266Client_";
  clientId += getDevId();

  #ifdef DEBUG
  Serial.println(formattedDateTime);
  Serial.print("Attempting MQTT connection to: ");
  Serial.print(config.mqttServer);
  Serial.print(" ...");
  #endif
  
  // Attempt to connect
  if (mqttClient.connect(clientId.c_str())) {
    LOGLN_STR("connected");
    publishState("START");

    mqttSubscribe();
    return;
  } 

  #ifdef DEBUG
  Serial.print("failed, rc=");
  Serial.print(mqttClient.state());
  Serial.print(" try again in ");
  Serial.print(MQTT_RECONNECT_TIME);
  Serial.println(" milliseconds");
  #endif
  
  mqttMillis = currentMillis;
}

void mqttSubscribe()
{
  mqttSubscribe((char*)mqttTopicPower.c_str());
  mqttSubscribe((char*)mqttTopicConfigCmnd.c_str());
  mqttSubscribe((char*)mqttTopicConfigModuleCmnd.c_str());

  #ifdef DS18B20_MODULE
  mqttSubscribe((char*)mqttTopicConfigTempCmnd.c_str());
  #endif

  #ifdef MOTION_PIR
  mqttSubscribe((char*)mqttTopicConfigMotionPirCmnd.c_str());
  #endif
  
  if (strlen(config.mqttGroupTopic) > 0) {
    mqttSubscribe(config.mqttGroupTopic);
  }
}

/*void mqttSubscribeWithReplace(char* topic)
{
  String mqttTopic = String(topic);
  mqttTopic.replace("%topic%", config.mqttTopic);
  
  mqttSubscribe((char*)mqttTopic);
}*/

void mqttSubscribe(char* topic)
{
  LOG_STR("Subscribe: ");
  LOGLN(topic);
  mqttClient.subscribe(topic);
}

void mqttCallback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  payload[length] = 0;
  if (mqttTopicPower == topic) {
    mqttHandlePower(payload);
    return;
  }

  if (mqttTopicConfigCmnd == topic) {
    mqttHandleConfig(payload);
    return;
  }

  if (mqttTopicConfigModuleCmnd == topic) {
    mqttHandleConfigModule(payload);
    return;
  }

  if (strlen(config.mqttGroupTopic) > 0 && strcmp(config.mqttGroupTopic, topic) == 0) {
    mqttHandlePower(payload);
    return;
  }

  #ifdef DS18B20_MODULE
  if (mqttTopicConfigTempCmnd == topic) {
    mqttHandleConfigTemp(payload);
    return;
  }
  #endif

  #ifdef MOTION_PIR
  if (mqttTopicConfigMotionPirCmnd == topic) {
    mqttHandleConfigMotionPir(payload);
    return;
  }
  #endif
}

void mqttHandleConfig(String message)
{
  if (0 != message.length()) {
    configFromJson(message);
    writeEEPROM();
  }
  String configJson = configToJson();
  mqttClient.publish((char*)mqttTopicConfigStat.c_str(), (char*)configJson.c_str());
}

void mqttHandleConfig(byte* payload)
{
  String message = String((char *)payload);
  mqttHandleConfig(message);
}

void mqttHandleConfigModule(String message)
{
  String configJson = configModuleToJson();
  mqttClient.publish((char*)mqttTopicConfigModuleStat.c_str(), (char*)configJson.c_str());
}

void mqttHandleConfigModule(byte* payload)
{
  mqttHandleConfigModule(String((char *)payload));
}

#ifdef MOTION_PIR
void mqttHandleConfigMotionPir(String message)
{
  if (0 != message.length()) {
    configMotionPirFromJson(message);
    writeEEPROM();
  }
  String configJson = configMotionPirToJson();
  mqttClient.publish((char*)mqttTopicConfigMotionPirStat.c_str(), (char*)configJson.c_str());
}

void mqttHandleConfigMotionPir(byte* payload)
{
  String message = String((char *)payload);
  mqttHandleConfigMotionPir(message);
}
#endif

#ifdef DS18B20_MODULE
void mqttHandleConfigTemp(String message)
{
  if (0 != message.length()) {
    configTempFromJson(message);
    writeEEPROM();
  }
  String configJson = configTempToJson();
  mqttClient.publish((char*)mqttTopicConfigStat.c_str(), (char*)configJson.c_str());
}

void mqttHandleConfigTemp(byte* payload)
{
  String message = String((char *)payload);
  mqttHandleConfigTemp(message);
}
#endif

void mqttHandlePower(byte* payload)
{
  String message = String((char *)payload);
  powerCommand(message);
}

void publishSensor()
{
  StaticJsonDocument<256> jsonMessage;
  jsonMessage["TIME"] = formattedDateTime;
  #ifdef DS18B20_MODULE
  for (int i = 0;  i < deviceCount;  ++i) {
    #ifdef DS18B20_MODULE_MQTT_DEV_ADDR
    jsonMessage["TEMP"][i]["ADDRESS"] = deviceAddressToString(deviceAddresses[i]);
    #endif
    jsonMessage["TEMP"][i]["NAME"] = config.friendlyNameOfTemp[i];
    jsonMessage["TEMP"][i]["VALUE"] = temperatures[i];
  }
  #endif
  
  char buffer[256];
  serializeJson(jsonMessage, buffer);
  mqttClient.publish((char*) mqttTopicSensor.c_str(), buffer);
}

void publishState(char *eventName)
{
  String topic = MQTT_TOPIC_STATE;
  topic.replace("%topic%", config.mqttTopic);
  String strRSSI = String((long)WiFi.RSSI());

  StaticJsonDocument<256> jsonMessage;
  jsonMessage["TIME"] = formattedDateTime;
  jsonMessage["POWER"] = getPowerState();
  jsonMessage["LIGHT"] = config.light;
  jsonMessage["CURRENT_LIGHT"] = currentLight;
  jsonMessage["TIME_OFF"] = config.timeOff;
  jsonMessage["FADE_TIME"] = config.fadeTime;
  jsonMessage["EVENT_NAME"] = eventName;
  jsonMessage["WIFI"]["IP"] = WiFi.localIP().toString();
  jsonMessage["WIFI"]["RSSI"] = strRSSI;
  jsonMessage["WIFI"]["SSID"] = String(config.ssid);
  jsonMessage["WIFI"]["MAC"] = getMacAddress();

  char buffer[256];
  serializeJson(jsonMessage, buffer);
  mqttClient.publish((char*) topic.c_str(), buffer);
}

void publishResult()
{
  String topic = MQTT_TOPIC_RESULT;
  topic.replace("%topic%", config.mqttTopic);

  StaticJsonDocument<64> jsonMessage;
  jsonMessage["TIME"] = formattedDateTime;
  jsonMessage["POWER"] = getPowerState();
  jsonMessage["LIGHT"] = config.light;
  jsonMessage["CURRENT_LIGHT"] = currentLight;
  jsonMessage["TIME_OFF"] = config.timeOff;
  jsonMessage["FADE_TIME"] = config.fadeTime;

  char buffer[64];
  serializeJson(jsonMessage, buffer);
  
  mqttClient.publish((char*) topic.c_str(), buffer);
}

char* getPowerState()
{
  switch(lightState)
  {
    case LIGHT_STATE_OFF: return "OFF";
    case LIGHT_STATE_ON: return "ON";
    case LIGHT_STATE_FADE_IN: return "FADE_IN";
    case LIGHT_STATE_FADE_OUT: return "FADE_OUT";
  }
  
  return "ERROR";
}
