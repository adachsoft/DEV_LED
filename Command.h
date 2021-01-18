#ifndef Command_h
#define Command_h

#include<Arduino.h>

typedef void (*TypeOfCommands)(String param);

class Command
{
  public:
    Command(const TypeOfCommands* arrayOfCommands, const char* commands[], int numberOfCommands);
    void runCommand();
    void init();
    void parseString(String inputString);
    String getCommand();
    String getParam();
    void executeCmd();
    int getCmdFromString(String cmd);
    String getResponse();
    bool isError();
    void getIntValueFromString(String param, byte arrayOfValue[], byte sizeOfArray);
    void getIntValueFromString(String param, int arrayOfValue[], int sizeOfArray);
  private:
    int _numberOfCommands;
    const TypeOfCommands* _arrayOfCommands;
    const char** _commands;
    String cmd;
    String param;
    String _response;
    bool _is_error;
};

#endif
