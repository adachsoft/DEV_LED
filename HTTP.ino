#ifdef HTTP_MODULE

void initHTTP()
{

  /*server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/html", index_html, processor);
  });*/
  
  server.on("/", handleRoot);
  server.on("/config", handleConfig);
  server.on("/save", handleSave);
  server.on("/light", handleLight);
  server.on("/reset", handleReset);
  #ifdef DS18B20_MODULE
  server.on("/temp", handleTemp);
  server.on("/tempJson", handleTempJSON);
  server.on("/save_temp", handleSaveTemp);
  #endif
  server.begin();
}

void handleRoot()
{
  String html = INDEX_HTML;
  
  server.send(200, "text/html", html);
}

void handleConfig()
{
  String html = CONFIG_HTML;
  html.replace("{$mqtt_server}", config.mqttServer);
  html.replace("{$mqtt_topic}", config.mqttTopic);
  html.replace("{$mqtt_group_topic}", config.mqttGroupTopic);
  html.replace("{$ssid}", config.ssid);
  html.replace("{$password}", config.password);
  html.replace("{$light}", String(config.light));
  html.replace("{$time_off}", String(config.timeOff));
  html.replace("{$fade_time}", String(config.fadeTime));
  html.replace("{$led_pin}", String(config.ledPin));
  
  server.send(200, "text/html", html);
}

void handleSave()
{
  String formSSID = server.arg("ssid");
  String formPassword = server.arg("password");
  String formMqttServer = server.arg("mqtt_server");
  String formMqttTopic = server.arg("mqtt_topic");
  String formMqttGroupTopic = server.arg("mqtt_group_topic");
  String formLight = server.arg("light");
  String formTimeOff = server.arg("time_off");
  String formFadeTime = server.arg("fade_time");
  String formLedPin = server.arg("led_pin");

  formSSID.toCharArray(config.ssid, 32);
  formPassword.toCharArray(config.password, 32);
  formMqttServer.toCharArray(config.mqttServer, 32);
  formMqttTopic.toCharArray(config.mqttTopic, 32);
  formMqttGroupTopic.toCharArray(config.mqttGroupTopic, 32);
  config.light = formLight.toInt();
  config.timeOff = formTimeOff.toInt();
  config.fadeTime = formFadeTime.toInt();
  config.ledPin = formLedPin.toInt();
  
  writeEEPROM();
  resetMillis = currentMillis;
  
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plane", "");
}

#ifdef DS18B20_MODULE
void handleTemp()
{
  String html = TEMP_HTML;

  String htmlTempFrom = "";
  String htmlTemp = "";
  for (uint8_t i = 0;  i < deviceCount;  ++i) {
    String devAddr = deviceAddressToString(deviceAddresses[i]);
    int id = sensors->getUserData(deviceAddresses[i]);
    
    htmlTemp += "Temp[" + String(config.friendlyNameOfTemp[i]) + "]: <span id=\"temp_" + devAddr + "\">" + String(temperatures[i]) + "</span><br>";
    htmlTempFrom += "<div class=\"input\"><span>[" + String(id) + "] " +  devAddr + "</span><input type=\"text\" name=\"" + devAddr + "\" value=\"" + String(config.friendlyNameOfTemp[i]) + "\" /></div>";
  }

  html.replace("{$temp}", htmlTemp);
  html.replace("{$temp_form}", htmlTempFrom);
  
  server.send(200, "text/html", html);
}

void handleTempJSON()
{
  StaticJsonDocument<256> jsonMessage;
  jsonMessage["TIME"] = formattedDateTime;
  
  for (int i = 0;  i < deviceCount;  ++i) {
    jsonMessage["TEMP"][i]["ADDRESS"] = deviceAddressToString(deviceAddresses[i]);
    jsonMessage["TEMP"][i]["NAME"] = config.friendlyNameOfTemp[i];
    jsonMessage["TEMP"][i]["VALUE"] = temperatures[i];
  }
  
  char buffer[256];
  serializeJson(jsonMessage, buffer);
  server.send(200, "text/html", buffer);
}

void handleSaveTemp()
{
  for (uint8_t i = 0;  i < deviceCount;  ++i) {
    String devAddr = deviceAddressToString(deviceAddresses[i]);
    sensors->setUserData(deviceAddresses[i], i);
    
    //int id = sensors->getUserData(deviceAddresses[i]);
    String formDevName = server.arg(devAddr);
    
    Serial.print("devAddr: ");
    Serial.println(devAddr);
    //Serial.print("id: ");
    //Serial.println(id);
    Serial.print("formDevName: ");
    Serial.println(formDevName);

    formDevName.toCharArray(config.friendlyNameOfTemp[i], 16);
  }

  writeEEPROM();
  
  server.sendHeader("Location", "/temp", true);
  server.send(302, "text/plane", "");
}
#endif

void handleReset()
{
  resetMillis = currentMillis;
  
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plane", "");
}

void handleLight()
{
  String strLight = server.arg("light");
  
  config.light = strLight.toInt();
  lightOn();

  server.sendHeader("Location", "/", true);
  server.send(302, "text/plane", "");
}

#endif
