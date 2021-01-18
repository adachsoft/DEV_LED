#ifdef SERIAL_MODULE

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
String param = "";

void serialListen() 
{
  while (!stringComplete && Serial.available()) {
    char inChar = (char)Serial.read();
    Serial.print(inChar);
    
    if (inChar != '\n' && inChar != '\r') { 
      inputString += inChar;
    }else if( inChar == '\n' ){
      stringComplete = true;
    }
  }
}

void commandFromSerial()
{
  if (stringComplete) {
    command.parseString(inputString);
    command.executeCmd();
    if (command.isError()) {
      Serial.println(command.getResponse());
    }
    //executeCommand(inputString);
    
    stringComplete = false;
    inputString = "";
  }
}

#endif
