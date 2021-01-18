#define DEBUG
#define HTTP_MODULE
#define IR_MODULE
#define SERIAL_MODULE
#define MOTION_PIR
#define DS18B20_MODULE
#define DS18B20_MODULE_MQTT_DEV_ADDR

#ifdef DEBUG
  #define LOG_STR(str) { Serial.print(F(str)); }
  #define LOGLN_STR(str) { Serial.println(F(str)); }
  #define LOGLN(value) { Serial.println(value); }
  #define LOG(value) { Serial.print(value); }
#else
  #define LOG_STR(str)
  #define LOGLN_STR(str)
  #define LOGLN(value)
  #define LOG(value)
#endif

#define SIZE_OF_EEPROM 768
//#define MQTT_MAX_TRANSFER_SIZE 1 * 1024
#define MQTT_MAX_PACKET_SIZE 2 * 1024

#ifdef DS18B20_MODULE
#define TEMPERATURE_SENSORS_MAX 3
#endif 

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <time.h>
#include "index.h";
#include "config_html.h";

#define PERIOD_MAIN_LOOP 1000

//EEPROM
#include "config.h";

//NTP Client
const long utcOffsetInSeconds = 3600;
String formattedDateTime;
WiFiUDP ntpUDP;
NTPClient* timeClient;

//DS18B20
#ifdef DS18B20_MODULE
#include <OneWire.h>
#include <DallasTemperature.h>
#include "temp_html.h"

#define ONE_WIRE_BUS (uint8_t)config.oneWireBUS
int deviceCount;
DeviceAddress deviceAddresses[TEMPERATURE_SENSORS_MAX];
float temperatures[TEMPERATURE_SENSORS_MAX];

OneWire* oneWire;
DallasTemperature* sensors;

#define MQTT_TOPIC_CONFIG_TEMP_STAT "stat/%topic%/CONFIG/TEMP"
#define MQTT_TOPIC_CONFIG_TEMP_CMND "cmnd/%topic%/CONFIG/TEMP"
String mqttTopicConfigTempStat;
String mqttTopicConfigTempCmnd;
#endif

#ifdef MOTION_PIR
#define MQTT_TOPIC_CONFIG_MOTION_PIR_STAT "stat/%topic%/CONFIG/MOTION_PIR"
#define MQTT_TOPIC_CONFIG_MOTION_PIR_CMND "cmnd/%topic%/CONFIG/MOTION_PIR"
String mqttTopicConfigMotionPirStat;
String mqttTopicConfigMotionPirCmnd;
#endif

//Ligth
#define LED_PIN (int)config.ledPin
#define LIGHT_STATE_OFF 0
#define LIGHT_STATE_ON 1
#define LIGHT_STATE_FADE_IN 2
#define LIGHT_STATE_FADE_OUT 3
bool powerOn = false;
int lightState;
uint16_t currentLight;
long lightTimeOff;
long lightMillis;
float fadeValue;
float fadeStep;
uint16_t fadeTime;
long fadeMillis;
bool isFadeMode;

//WiFi Connection configuration
bool isWIFIOn;

//MQTT
WiFiClient espClient;
PubSubClient mqttClient(espClient);
#define MQTT_TOPIC_STATE "tele/%topic%/STATE"
#define MQTT_TOPIC_POWER "cmnd/%topic%/POWER"
#define MQTT_TOPIC_RESULT "stat/%topic%/RESULT"
#define MQTT_TOPIC_SENSOR "tele/%topic%/SENSOR"
#define MQTT_TOPIC_CONFIG_STAT "stat/%topic%/CONFIG"
#define MQTT_TOPIC_CONFIG_CMND "cmnd/%topic%/CONFIG"
String mqttTopicPower;
String mqttTopicSensor;
String mqttTopicConfigStat;
String mqttTopicConfigCmnd;

//HTTP Server
#ifdef HTTP_MODULE
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
#endif

#include "ir_module.h";

//------------------------------------
#ifdef SERIAL_MODULE
#include "Command.h"

#define CMD_COUNT 3

void cmdOk(String param);
void cmdIP(String param);
void cmdPower(String param);

const TypeOfCommands arrayOfCommands[CMD_COUNT] = {
  &cmdOk, &cmdIP, &cmdPower
};

const char *commands[CMD_COUNT] = {
  "OK", "IP", "POWER"
};

Command command(arrayOfCommands, commands, CMD_COUNT);
#endif
//------------------------------------


long mainLoop;
long currentMillis;
long statMillis;
long mqttMillis;
long resetMillis;

//================================================

void setup() 
{
  Serial.begin(115200);
  Serial.println();
  
  initLight();

  Serial.print("Dev ID: ");
  Serial.println(getDevId());

  initEEPROM();
  readEEPROM();
  #ifdef DS18B20_MODULE
  setupOneWire();
  #endif

  setupWIFI();

  #ifdef HTTP_MODULE
  //Initialize Webserver
  initHTTP();
  #endif

  #ifdef IR_MODULE
  setupIR();
  #endif

  #ifdef MOTION_PIR
  setupMotionPir();
  #endif

  setupMQTT();
  setupTime();
  
  currentMillis = millis();
  statMillis = currentMillis;
  mainLoop = currentMillis;
  //mqttMillis = currentMillis;
  mqttMillis = 0;
  resetMillis == 0;
}

void loop() 
{
  currentMillis = millis();
  if (currentMillis - mainLoop > PERIOD_MAIN_LOOP) {
    mainLoop = currentMillis;
    timeClient->update();
    formattedDateTime = getDateTime();
    lightOffAfterTime();
    #ifdef DS18B20_MODULE
    loopOneWire();
    #endif
  }

  if (lightState == LIGHT_STATE_FADE_IN && currentMillis - fadeMillis > 100) {
    fadeMillis = currentMillis;
    lightFadeInLoop();
  }

  if (lightState == LIGHT_STATE_FADE_OUT && currentMillis - fadeMillis > 100) {
    fadeMillis = currentMillis;
    lightFadeOutLoop();
  }

  if (currentMillis - statMillis > 10000) {
    statMillis = currentMillis;
    
    publishState("STATE");
    publishSensor();
  }

  loopCheckWIFI();
  loopMQTT();

  #ifdef IR_MODULE
  loopIR();
  #endif
  
  #ifdef HTTP_MODULE
  server.handleClient();
  #endif

  #ifdef SERIAL_MODULE
  serialListen();
  commandFromSerial();
  #endif

  #ifdef MOTION_PIR
  loopMotionPir();
  #endif

  resetDelay();
}

//================================================

String getDevId()
{
  return String(ESP.getChipId()) + "-" + String(ESP.getFlashChipId());
}

String getMacAddress()
{
  byte mac[6];
  String macStr = "";
  WiFi.macAddress(mac);

  for (uint8_t i = 0; i < 6; ++i){
    macStr += String(mac[i], HEX);
    if (i < 5) {
      macStr += ":";
    }
  }
  macStr.toUpperCase();
  
  return macStr;
}

void resetDev()
{
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(0, HIGH);
  digitalWrite(2, HIGH);

  ESP.restart();
}

void resetDelay()
{
  if (resetMillis == 0) {
    return;
  }

  if (currentMillis - resetMillis > 3000) {
    resetMillis = 0;
    resetDev();
  }
}
