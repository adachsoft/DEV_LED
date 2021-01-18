#include "Command.h"

Command::Command(const TypeOfCommands* arrayOfCommands, const char* commands[], int numberOfCommands)
{
  _arrayOfCommands = arrayOfCommands;
  _commands = commands;
  _numberOfCommands = numberOfCommands;
}

void Command::runCommand()
{
  
}

void Command::init()
{
}

void Command::parseString(String inputString)
{
  _is_error = false;
  cmd = "";
  param = "";
  bool isComplete = false;
  int strLength = inputString.length();
  for(int i=0; i < strLength; i++){
    if (inputString[i] != ' ' && !isComplete){
      cmd += inputString[i];
    }else if (inputString[i] == ' ' && !isComplete){
      while(inputString[i]==' ' && i < strLength){
        i++;
      }
      if (i < strLength){
        param += inputString[i];
      }
      isComplete = true;
    }else if( isComplete ){
      param += inputString[i]; 
    }
  }
}

String Command::getCommand()
{
  return cmd;
}

String Command::getParam()
{
  return param;
}

String Command::getResponse()
{
  return _response;
}

bool Command::isError()
{
  return _is_error;
}

void Command::executeCmd()
{
  int cmdIndex = getCmdFromString(cmd);
  if (cmdIndex == -1) {
    _is_error = true;
    _response = "ERROR: Command not found '" + cmd + "'";
    return;
  }

  _arrayOfCommands[cmdIndex](param);
}

int Command::getCmdFromString(String cmd)
{
  for(int i = 0; i < _numberOfCommands; ++i) {
    if (strcmp(cmd.c_str(), _commands[i]) == 0) {
      return i;
    }
  }
  
  return -1;
}

void Command::getIntValueFromString(String param, byte arrayOfValue[], byte sizeOfArray)
{
  byte idx=0;
  String value;
  byte strLength = param.length();
  for(byte i = 0; i < strLength && idx < sizeOfArray; i++){
    if (',' != param[i]) {
      value += param[i];
    }else{
      arrayOfValue[idx++] = value.toInt();
      value="";
    }
  }
  if(value.length() > 0 && idx < sizeOfArray) {
    arrayOfValue[idx++] = value.toInt();
  }
}

void Command::getIntValueFromString(String param, int arrayOfValue[], int sizeOfArray)
{
  byte idx=0;
  String value;
  int strLength = param.length();
  for(int i=0; i < strLength && idx < sizeOfArray; i++){
    if (',' != param[i]) {
      value += param[i];
    }else{
      arrayOfValue[idx++] = value.toInt();
      value="";
    }
  }
  if(value.length() > 0 && idx < sizeOfArray) {
    arrayOfValue[idx++] = value.toInt();
  }
}
