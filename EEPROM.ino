#define CONFIG_START 32
#define CONFIG_VERSION 11

void writeEEPROM()
{
  config.configVersion = CONFIG_VERSION;
  for (unsigned int i = 0; i < sizeof(config); i++) {
    EEPROM.write(CONFIG_START + i, *((char*)&config + i));
  }
  EEPROM.commit();
}

void readEEPROM()
{
  unsigned int sizeOfVersion = sizeof(config.configVersion);
  for (unsigned int i = 0; i < sizeof(config); i++){
    *((char*)&config + i) = EEPROM.read(CONFIG_START + i);
    if (sizeOfVersion == i && config.configVersion != CONFIG_VERSION) {
      LOGLN_STR("CONFIG VERSION");
      setDefaultConfig();
      return;
    }
  }
}

void initEEPROM()
{
  EEPROM.begin(SIZE_OF_EEPROM);  //Initialize EEPROM
  
  LOG_STR("Size of config: ");
  LOGLN(sizeof(config));
}

void setDefaultConfig()
{
  LOGLN_STR("Set default config");
  
  strcpy(config.mqttServer, CONFIG_MQTT_SERVER);
  strcpy(config.mqttTopic, CONFIG_MQTT_TOPIC);
  
  strcpy(config.ssid, CONFIG_SSID);
  strcpy(config.password, CONFIG_PASSWORD);
  strcpy(config.ntpServer, CONFIG_NTP_SERVER);

  config.light = CONFIG_LIGHT;
  config.timeOff = CONFIG_TIME_OFF;
  config.fadeTime = CONFIG_FADE_TIME;
  config.ledPin = CONFIG_LED_PIN;
  config.pwmReverse = CONFIG_PWM_REVERSE;
  
  #ifdef DS18B20_MODULE
  config.oneWireBUS = D1;
  for(uint8_t i; i < TEMPERATURE_SENSORS_MAX; ++i) {
    config.friendlyNameOfTemp[i][0] = 0;
  }
  #endif
  
  #ifdef MOTION_PIR
  config.motionPirPin = D6;
  strcpy(config.mqttTopicMotionPirH, "");
  strcpy(config.mqttMessageMotionPirH, "");
  strcpy(config.mqttTopicMotionPirL, "");
  strcpy(config.mqttMessageMotionPirL, "");
  #endif
}
