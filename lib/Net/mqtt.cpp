#include <WiFi.h>
#include <PubSubClient.h>
#include "Def.h"
#include "Net.h"
#include "CoffeeSelect.h"

WiFiClient   espClient;
PubSubClient mqtt(espClient);

void connectWifi() {
    Serial.printf("Connecting to %s\n", WIFI_SSID);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PWD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.printf("Connected to %s, IP: %s\n", WIFI_SSID, WiFi.localIP().toString().c_str());
}

void connectMQTT() {
    while(!mqtt.connected()) {
        Serial.print("Connecting to MQTT broker... ");
        if (mqtt.connect(MQTT_CLIENT)) {
            Serial.println("connected.");
            if (MODE == COFFEE_SELECT) {
                mqtt.subscribe(MQTT_SUB_TOPIC);
                mqtt.subscribe(MQTT_SUB_COORD_TOPIC);
            }
        } else {
            Serial.printf("failed to connect (%d). Retrying in 3s...\n", mqtt.state());
            delay(3000);
        }
    }
}

// Temporary
void publishCoords(int x, int y) {
    char buf[32];
    snprintf(buf, sizeof(buf), "x:%d,y:%d", x, y);

    mqtt.publish(MQTT_PUB_TOPIC, buf);
}

void onMessage(char* topic, byte* payload, uint length) {
    if (strcmp(topic, MQTT_SUB_TOPIC) == 0) {
        char message[length + 1];
        for (uint i = 0; i < length; i++) message[i] = (char)payload[i];
        message[length] = '\0';
        Serial.printf("Message: %s\n", message);
        handleCoffeeRequest(message);
    }

    if (strcmp(topic, MQTT_SUB_COORD_TOPIC) == 0) {
        char message[length + 1];
        for (uint i = 0; i < length; i++) message[i] = (char)payload[i];
        message[length] = '\0';
        Serial.printf("Message: %s\n", message);

        char* x = strtok(message, ",");
        char* y = strtok(NULL, ",");
        char* c = strtok(NULL, ",");
        char* d = strtok(NULL, ",");
        int xVal = atoi(x);
        int yVal = atoi(y);
        bool mouseClicked = atoi(c);
        int delay = atoi(d);

        handleCoordinatesRequest(xVal, yVal, mouseClicked, delay);
        publishCoords(xVal, yVal);
    }
}

int publishUID(uint8_t* uid, uint8_t uidLength) {
    char payload[PAYLOAD_SIZE] = "{\"uid\":\"";

    char hexStr[uidLength * 2 + 1];
    hexStr[0] = '\0';
    for (uint8_t i = 0; i < uidLength; i++) {
        sprintf(hexStr + strlen(hexStr), "%02X", uid[i]);
    }
    strcat(payload, hexStr);
    strcat(payload, "\"}");

    if (!mqtt.publish(MQTT_PUB_TOPIC, payload)) {
        Serial.println("Failed to publish payload");
        return 0;
    }

    return 1;
}

void processAndKeepAlive() {
    mqtt.loop();
}

void initMQTT() {
    connectWifi();

    mqtt.setServer(MQTT_BROKER, MQTT_PORT);
    mqtt.setCallback(onMessage);
    connectMQTT();
}
