
String configToJson()
{
  StaticJsonDocument<256> jsonMessage;
  char jsonStr[256];
  
  jsonMessage["config_version"] = config.configVersion;
  jsonMessage["mqtt_server"] = config.mqttServer;
  jsonMessage["mqtt_topic"] = config.mqttTopic;
  jsonMessage["mqtt_group_topic"] = config.mqttGroupTopic;
  jsonMessage["ssid"] = config.ssid;
  jsonMessage["password"] = config.password;
  jsonMessage["ntp_server"] = config.ntpServer;
  jsonMessage["light"] = config.light;
  jsonMessage["pwm_reverse"] = config.pwmReverse;
  jsonMessage["time_off"] = config.timeOff;
  jsonMessage["fade_time"] = config.fadeTime;
  jsonMessage["led_pin"] = config.ledPin;

  serializeJson(jsonMessage, jsonStr);

  return String(jsonStr);
}

void configFromJson(String configJson)
{
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, configJson);
  if (error) {
    Serial.println(error.c_str());
    return;
  }

  if (doc.containsKey("mqtt_server")) {
    strcpy(config.mqttServer, doc["mqtt_server"]);
  }
  
  if (doc.containsKey("mqtt_topic")) {
    strcpy(config.mqttTopic, doc["mqtt_topic"]);
  }

  if (doc.containsKey("mqtt_group_topic")) {
    strcpy(config.mqttTopic, doc["mqtt_group_topic"]);
  }

  if (doc.containsKey("ssid")) {
    strcpy(config.mqttTopic, doc["ssid"]);
  }

  if (doc.containsKey("password")) {
    strcpy(config.mqttTopic, doc["password"]);
  }

  if (doc.containsKey("ntpServer")) {
    strcpy(config.mqttTopic, doc["ntp_server"]);
  }

  if (doc.containsKey("light")) {
    config.light = doc["light"];
  }

  if (doc.containsKey("time_off")) {
    config.timeOff = doc["time_off"];
  }

  if (doc.containsKey("pwm_reverse")) {
    config.pwmReverse = doc["pwm_reverse"].as<bool>();
  }

  if (doc.containsKey("fade_time")) {
    config.fadeTime = doc["fade_time"];
  }

  if (doc.containsKey("led_pin")) {
    config.ledPin = doc["led_pin"];
  }
}

String configModuleToJson()
{
  StaticJsonDocument<256> jsonMessage;
  char jsonStr[256];
  uint16_t idx = 0; 
  
  jsonMessage["config_version"] = config.configVersion;
  
  #ifdef HTTP_MODULE
  jsonMessage["MODULES"][idx++] = "HTTP_MODULE";
  #endif
  
  #ifdef IR_MODULE
  jsonMessage["MODULES"][idx++] = "IR_MODULE";
  #endif

  #ifdef SERIAL_MODULE
  jsonMessage["MODULES"][idx++] = "SERIAL_MODULE";
  #endif

  #ifdef MOTION_PIR
  jsonMessage["MODULES"][idx++] = "MOTION_PIR";
  #endif

  #ifdef DS18B20_MODULE
  jsonMessage["MODULES"][idx++] = "DS18B20_MODULE";
  #endif

  #ifdef DS18B20_MODULE_MQTT_DEV_ADDR
  jsonMessage["MODULES"][idx++] = "DS18B20_MODULE_MQTT_DEV_ADDR";
  #endif

  serializeJson(jsonMessage, jsonStr);

  return String(jsonStr);
}

#ifdef DS18B20_MODULE

String configTempToJson()
{
  StaticJsonDocument<256> jsonMessage;
  char jsonStr[256];
  
  jsonMessage["config_version"] = config.configVersion;
  jsonMessage["one_wire_bus"] = config.oneWireBUS;
  
  serializeJson(jsonMessage, jsonStr);
  
  return String(jsonStr);
}

void configTempFromJson(String configJson)
{
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, configJson);
  if (error) {
    Serial.println(error.c_str());
    return;
  }

  if (doc.containsKey("one_wire_bus")) {
    config.oneWireBUS = doc["one_wire_bus"];
  }
}

#endif 

#ifdef MOTION_PIR

String configMotionPirToJson()
{
  StaticJsonDocument<1024> jsonMessage;
  char jsonStr[512];
  
  jsonMessage["config_version"] = config.configVersion;
  jsonMessage["motion_pir_pin"] = config.motionPirPin;
  jsonMessage["mqtt_topic_motion_pir_h"] = config.mqttTopicMotionPirH;
  jsonMessage["mqtt_topic_motion_pir_l"] = config.mqttTopicMotionPirL;
  jsonMessage["mqtt_message_motion_pir_h"] = config.mqttMessageMotionPirH;
  jsonMessage["mqtt_message_motion_pir_l"] = config.mqttMessageMotionPirL;
  jsonMessage["motion_pir_cmnd_state_l"] = config.motionPirCmndStateL;
  jsonMessage["motion_pir_cmnd_state_h"] = config.motionPirCmndStateH;
  
  serializeJson(jsonMessage, jsonStr);
  
  return String(jsonStr);
}

void configMotionPirFromJson(String configJson)
{
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, configJson);
  if (error) {
    Serial.println(error.c_str());
    return;
  }

  if (doc.containsKey("motion_pir_pin")) {
    config.motionPirPin = doc["motion_pir_pin"];
  }

  if (doc.containsKey("mqtt_topic_motion_pir_l")) {
    strcpy(config.mqttTopicMotionPirL, doc["mqtt_topic_motion_pir_l"]);
  }

  if (doc.containsKey("mqtt_topic_motion_pir_h")) {
    strcpy(config.mqttTopicMotionPirH, doc["mqtt_topic_motion_pir_h"]);
  }

  if (doc.containsKey("mqtt_message_motion_pir_l")) {
    strcpy(config.mqttMessageMotionPirL, doc["mqtt_message_motion_pir_l"]);
  }

  if (doc.containsKey("mqtt_message_motion_pir_h")) {
    strcpy(config.mqttMessageMotionPirH, doc["mqtt_message_motion_pir_h"]);
  }

  if (doc.containsKey("motion_pir_cmnd_state_l")) {
    strcpy(config.motionPirCmndStateL, doc["motion_pir_cmnd_state_l"]);
  }

  if (doc.containsKey("motion_pir_cmnd_state_h")) {
    strcpy(config.motionPirCmndStateH, doc["motion_pir_cmnd_state_h"]);
  }
}

#endif 
