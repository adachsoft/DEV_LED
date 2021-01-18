#ifdef IR_MODULE

#define MQTT_TOPIC_IR_SENSOR "tele/%topic%/SENSOR/IR"
#define PERIOD_IR_LOOP 1200

long millisIR;
String mqttTopicIRSensor;

void setupIR()
{
  mqttTopicIRSensor = MQTT_TOPIC_IR_SENSOR;
  mqttTopicIRSensor.replace("%topic%", config.mqttTopic);
  
  irrecv.enableIRIn();
  millisIR = currentMillis;
}

void loopIR()
{
  if (currentMillis - millisIR <= PERIOD_IR_LOOP) {
    return;
  }
  
  if (!irrecv.decode(&results)) {
    return;
  }

  #ifdef DEBUG
  Serial.print("IR: ");
  serialPrintUint64(results.value, HEX);
  Serial.println("");
  #endif

  publishIRSensor(uint64ToString(results.value, HEX));
  irrecv.resume();  // Receive the next value
  millisIR = currentMillis;
}

void publishIRSensor(String irCode)
{
  StaticJsonDocument<128> jsonMessage;
  jsonMessage["TIME"] = formattedDateTime;
  jsonMessage["IR"]["CODE"] = irCode;
  
  char buffer[128];
  serializeJson(jsonMessage, buffer);
  mqttClient.publish((char*) mqttTopicIRSensor.c_str(), buffer);
}


#endif
