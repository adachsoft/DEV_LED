#ifndef IR_MODULE_h
#define IR_MODULE_h

#ifdef IR_MODULE
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

int RECV_PIN = D2; //an IR detector connected to D4
IRrecv irrecv(RECV_PIN);
decode_results results;
#endif

#endif
