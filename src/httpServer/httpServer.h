#include "Arduino.h"


#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>

void serverBegin(void);
void serverHandle(void);
void getSettingsJSON(void);
void postSettingsJSON(void);
