#include "httpServer/httpServer.h"

#include <Arduino.h>

void setup(void) {
	Serial.begin(115200);
	serverBegin();
	pinMode(14, INPUT);
}

void loop(void) { serverHandle(); }