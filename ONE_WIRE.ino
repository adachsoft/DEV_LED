#ifdef DS18B20_MODULE

void setupOneWire()
{
  oneWire = new OneWire(ONE_WIRE_BUS);
  sensors = new DallasTemperature(oneWire);
  
  deviceCount = 0;
  sensors->begin();
  sensors->setWaitForConversion(false);
  deviceCount = sensors->getDeviceCount();
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(deviceCount, DEC);
  Serial.println(" devices.");

  for (int i = 0;  i < deviceCount;  ++i) {
    Serial.print("Sensor ");
    Serial.print(i + 1);
    Serial.print(" : ");
    sensors->getAddress(deviceAddresses[i], i);
    printAddress(deviceAddresses[i]);
    Serial.println("");

    int id = sensors->getUserData(deviceAddresses[i]);
    Serial.print("ID: ");
    Serial.println(id);

    Serial.print("Friendly name: ");
    Serial.println(config.friendlyNameOfTemp[i]);
  }
  Serial.println("");
  
  sensors->requestTemperatures();
}

void loopOneWire()
{
  float temp;
  
  for (uint8_t i = 0;  i < deviceCount;  ++i) {
    temp = sensors->getTempCByIndex(i);
    temperatures[i] = temp;
  }
  
  sensors->requestTemperatures();
}

void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; ++i) {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) {
      Serial.print("0");
    }
    
    Serial.print(deviceAddress[i], HEX);
  }
}

String deviceAddressToString(DeviceAddress deviceAddress)
{
  String devAddress = "";
  for (uint8_t i = 0; i < 8; ++i) {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) {
      devAddress += "0";
    }
    devAddress += String(deviceAddress[i], HEX);
  }

  return devAddress;
}

#endif
