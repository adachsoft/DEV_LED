#ifndef CONFIG_h
#define CONFIG_h

#include "config_default.h";

struct Config
{
  unsigned int configVersion;
  char mqttServer[32];
  char mqttTopic[32];
  char mqttGroupTopic[32];
  char ssid[32];
  char password[32];
  char ntpServer[32];
  uint16_t light;
  bool pwmReverse;
  uint16_t timeOff;
  uint16_t fadeTime;
  uint8_t ledPin;
  #ifdef DS18B20_MODULE
  char friendlyNameOfTemp[TEMPERATURE_SENSORS_MAX][16];
  uint8_t oneWireBUS;
  #endif
  #ifdef MOTION_PIR
  uint8_t motionPirPin;
  char mqttTopicMotionPirH[32];
  char mqttMessageMotionPirH[64];
  char mqttTopicMotionPirL[32];
  char mqttMessageMotionPirL[64];
  char motionPirCmndStateL[64];
  char motionPirCmndStateH[64];
  #endif
};
Config config;

#endif
