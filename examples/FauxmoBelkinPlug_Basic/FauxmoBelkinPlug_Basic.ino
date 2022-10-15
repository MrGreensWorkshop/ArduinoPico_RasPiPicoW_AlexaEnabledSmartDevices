#include <Arduino.h>
#if defined(ESP8266)
    #include <ESP8266WiFi.h>
    #define MCU  ESP
#elif defined(ESP32)
    #include <WiFi.h>
    #define MCU  ESP
#elif defined(ARDUINO_RASPBERRY_PI_PICO_W)
    #include <pico/cyw43_arch.h>
    #define MCU  rp2040
#else
	#error Platform not supported
#endif

#include "src/fauxmoESP.h"

#define WIFI_SSID "..."
#define WIFI_PASS "..."

#define SERIAL_BAUDRATE                 115200
#define LED                             2 //GP2

fauxmoESP fauxmo;

// This is for one device only. You need an array for multiple devices.
bool SwitchState = false;

// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------

void wifiSetup() {

    // Set WIFI module to STA mode
    WiFi.mode(WIFI_STA);

    // Connect
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // Wait
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println();

    // Connected!
#ifdef ARDUINO_RASPBERRY_PI_PICO_W
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID(), WiFi.localIP().toString().c_str());
#else
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
#endif
}

void setup() {

    // LED
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);
    
    // Init serial port and clean garbage
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println();
    Serial.println();

    // Wifi
    wifiSetup();

    // You have to call enable(true) once you have a WiFi connection
    // You can enable or disable the library at any moment
    // Disabling it will prevent the devices from being discovered and switched
    fauxmo.enable(true);

    // Add virtual devices
    fauxmo.addDevice("switch one");
    //fauxmo.addDevice("switch two"); // You can add more devices
    //fauxmo.addDevice("switch three");
    //fauxmo.addDevice("switch four");
    //fauxmo.addDevice("switch five");
    //fauxmo.addDevice("switch six");
    //fauxmo.addDevice("switch seven");
    //fauxmo.addDevice("switch eight");
    //fauxmo.addDevice("switch nine");
    //fauxmo.addDevice("switch ten");

    // fauxmoESP 2.0.0 has changed the callback signature to add the device_id,
    // this way it's easier to match devices to action without having to compare strings. (value is not functional)
    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        Serial.printf("[MAIN] Device #%d (%s) state: %s\n", device_id, device_name, state ? "ON" : "OFF");
        digitalWrite(LED, state);
        // This is for one device only. You need an array for multiple devices.
        SwitchState = state;
    });

    // Callback to retrieve current state (for GetBinaryState queries) (value is not functional)
    fauxmo.onGetState([](unsigned char device_id, const char * device_name, bool &state, unsigned char &value) {
        Serial.println("onGetStateCallback");
        // This is for one device only. You need an array for multiple devices.
        state = SwitchState;
    });

}

void loop() {

    // Since fauxmoESP 2.0 the library uses the "compatibility" mode by
    // default, this means that it uses WiFiUdp class instead of AsyncUDP.
    // The later requires the Arduino Core for ESP8266 staging version
    // whilst the former works fine with current stable 2.3.0 version.
    // But, since it's not "async" anymore we have to manually poll for UDP
    // packets
    fauxmo.handle();

    static unsigned long last = millis();
    if (millis() - last > 5000) {
        last = millis();
        Serial.printf("[MAIN] Free heap: %d bytes\n", MCU.getFreeHeap());
    }

}
