void cmdOk(String param)
{
  Serial.println(F("OK"));
}

void cmdIP(String param)
{
  Serial.println(WiFi.localIP());
}

void cmdPower(String param)
{
  powerCommand(param);
}
