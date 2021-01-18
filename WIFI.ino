long millisWIFI;
bool isConnectedWIFI;

void setupWIFI()
{
  isConnectedWIFI = false;
  if (strlen(config.ssid) > 0) {
    setModeSTA();
  } else {
    setModeSetup();
  }
}

void loopCheckWIFI()
{
  if (currentMillis - millisWIFI > 500) {
    millisWIFI = currentMillis;
    checkWIFI();
  }
}

void setModeSTA()
{
  LOGLN_STR("WIFI mode: WIFI_STA");
  LOG_STR("WIFI connecting to: ");
  LOGLN(config.ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  WiFi.begin(config.ssid, config.password);
  isWIFIOn = true;
}

void setModeSetup()
{
  LOGLN_STR("WIFI mode: WIFI_AP");
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.mode(WIFI_AP);
  isWIFIOn = false;
  String ssid = String("ESP_") + getDevId() + String("_setup");
  WiFi.softAP(ssid.c_str(), "123456789");

  LOG_STR("AP IP: ");
  LOGLN(WiFi.softAPIP());
}

/*void setModeSTA()
{
  
}*/

void checkWIFI()
{
  if (!isWIFIOn) {
    return;
  }
  
  if (!isConnectedWIFI && WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    isConnectedWIFI = true;
  } else if (isConnectedWIFI && WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected");
    setupWIFI();
  }
}

bool isWIFIConnected()
{
  return isConnectedWIFI;
}
