#ifdef LOG_MODULE

void setupLog()
{
  wifiServer.begin();
}

void loopLog()
{
  
}

void logServerLn(const __FlashStringHelper* message)
{
  WiFiClient client = wifiServer.available();
  if (!client) {
    return;
  }

  client.println(message);
}

void logServerLn(uint16_t message)
{
  WiFiClient client = wifiServer.available();
  if (!client) {
    return;
  }

  client.println(message);
}

void logServerLn(const char* message)
{
  WiFiClient client = wifiServer.available();
  if (!client) {
    return;
  }

  client.println(message);
}

void logServerLn(char* message)
{
  WiFiClient client = wifiServer.available();
  if (!client) {
    return;
  }

  client.println(message);
  client.flush();
}

void logServer(const __FlashStringHelper* message)
{
  WiFiClient client = wifiServer.available();
  if (!client) {
    return;
  }

  client.print(message);
  client.flush();
}

void logServer(uint16_t message)
{
  WiFiClient client = wifiServer.available();
  if (!client) {
    return;
  }

  client.print(message);
  client.flush();
}

void logServer(const char* message)
{
  WiFiClient client = wifiServer.available();
  if (!client) {
    return;
  }

  client.print(message);
  client.flush();
}

void logServer(char* message)
{
  WiFiClient client = wifiServer.available();
  if (!client) {
    return;
  }

  client.print(message);
  client.flush();
}

#endif
