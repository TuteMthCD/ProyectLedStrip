#include "httpServer.h"
#include "../stripLib/striplib.h"

//#define SERVER "ServerStrip.local"
#define PORT 8266
#define URL  "/ledStrip/update"

#define ssid	 "Fibertel WiFi528"
#define password "Jm_23242526"

#define WiFiMode_AP 16
int brightness = 0;

ESP8266WebServer server(80);
DynamicJsonDocument settingsJSON(1024);

void check_updates(String);
void update_progress(int, int);
// public
/*void ICACHE_RAM_ATTR off(void) {
	strip.light(0);
	detachInterrupt(digitalPinToInterrupt(14));
}*/

void serverBegin(void) {
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	// Serial.println("");

	bool APMODE = 0;
	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");

		if (!digitalRead(WiFiMode_AP)) { // GPIO16 GND WIFIMODE_AP

			IPAddress local_IP(192, 168, 4, 1);
			IPAddress gateway(192, 168, 4, 1);
			IPAddress subnet(255, 255, 255, 0);

			WiFi.mode(WIFI_AP);

			WiFi.softAPConfig(local_IP, gateway, subnet);
			APMODE = WiFi.softAP("ESP_" + String(ESP.getChipId()));

			Serial.println("\nAPMODE SSID = ESP_" +
				       String(ESP.getChipId()));
			break;
		}
	}
	// incio mdns (servicio en red)
	// Activate mDNS this is used to be able to connect to the server
	// with local DNS hostmane esp8266.local
	String MDNSName = "ESP_" + String(ESP.getChipId());
	if (APMODE == 1) {

		if (MDNS.begin(MDNSName, WiFi.softAPIP())) // inicio en AP
			Serial.println("MDNS responder started in AP");

	} else if (MDNS.begin(MDNSName)) // incio en STA
		Serial.println("MDNS responder started");

	// Announce esp tcp service on port 8080
	// MDNS.addService("ip", "tcp", 8080);
	// Announce esp htpp service on port 80
	// aparece como _esp._tcp
	MDNS.addService("esp", "tcp", 80);
	// Server uris open
	server.on("/", HTTP_GET, []() {
		server.send(200, F("text/html"),
			    F("Welcome to the REST Web Server"));
	});
	server.onNotFound([]() {
		server.send(404, "text/plain", "404: Not found");
	});

	server.on("/settingsJSON", HTTP_GET, getSettingsJSON);
	server.on("/settingsJSON", HTTP_POST, postSettingsJSON);

	server.begin();
	Serial.println("HTTP server started");

	// inicio archivo json
	settingsJSON["ip"]     = WiFi.localIP().toString();
	settingsJSON["chipId"] = String(ESP.getChipId());

	settingsJSON["power"]	   = 0;
	settingsJSON["brightness"] = (unsigned int)(brightness);
	settingsJSON["effect"]	   = 0;
	settingsJSON["color"]	   = 0xFFFFFF;
	settingsJSON["velocity"]   = 10;

	// paso variables
	strip.light((bool)settingsJSON["power"]);
	strip.setBrightness((unsigned int)settingsJSON["brightness"]);
	strip.setEffect((unsigned char)settingsJSON["effect"]);
	strip.setcolor(settingsJSON["color"]);
	strip.setVelocity(settingsJSON["velocity"]);
	// inicio tira de led
	strip.init();
	// attachInterrupt(digitalPinToInterrupt(14), off,
	// FALLING); /// pruebaaaaaaaaaaaaaaaaaaaaaaaaaa
}

void getSettingsJSON(void) {
	settingsJSON["signalStrengh"] = String(WiFi.RSSI());
	settingsJSON["freeHeap"]      = String(ESP.getFreeHeap());

	String response;
	serializeJson(settingsJSON, response);
	server.send(200, "text/json", response);
}

void postSettingsJSON(void) {
	DynamicJsonDocument receivingJSONdoc(1024);

	deserializeJson(receivingJSONdoc, server.arg("plain"));
	JsonObject receivingJSON = receivingJSONdoc.as<JsonObject>();

	// checkea update
	if (receivingJSON.containsKey("update")) {
		if (receivingJSON.containsKey("update")) {
			String ipAddr = receivingJSON["ip"];
			// Serial.println(ipAddr);
			server.send(200, "text/plain");
			check_updates(ipAddr);
		}
		return;
	}

	// ingresa los valores
	for (auto kvp : receivingJSON) { // ingresa los valores
		settingsJSON[kvp.key()] = kvp.value();
	}

	String response;
	serializeJson(settingsJSON, response);
	// Serial.println(response);

	server.send(200, "text/json", response);

	// seteo variables en el objeto strip
	if (receivingJSON.containsKey("power"))
		strip.light((bool)settingsJSON["power"]);

	if (receivingJSON.containsKey("brightness"))
		strip.setBrightness((unsigned int)settingsJSON["brightness"]);

	if (receivingJSON.containsKey("effect"))
		strip.setEffect((unsigned char)settingsJSON["effect"]);

	if (receivingJSON.containsKey("color"))
		strip.setcolor(settingsJSON["color"]);

	if (receivingJSON.containsKey("velocity"))
		strip.setVelocity(settingsJSON["velocity"]);
}
void serverHandle(void) {
	MDNS.update();
	server.handleClient();
	strip.handle(); // handle strip
}

void check_updates(String SERVER) {
	MDNS.close();
	Serial.println("updating...");

	WiFiClient client;
	ESPhttpUpdate.onProgress(update_progress);
	ESPhttpUpdate.setLedPin(LED_BUILTIN, HIGH);
	ESPhttpUpdate.update(client, SERVER, PORT, URL);
}

void update_progress(int cur, int total) { showProgres(cur, total); }