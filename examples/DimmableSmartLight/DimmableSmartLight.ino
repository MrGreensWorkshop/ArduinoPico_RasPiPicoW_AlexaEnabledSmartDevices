/*
 * Dimmable Smart Light v1.0.0
 * 
 * Copyright (c) 2022 Mr. Green's Workshop https://www.MrGreensWorkshop.com
 *
 * SPDX-License-Identifier: MIT
 */

#include <Arduino.h>

#include "device.h"
#include "src/fauxmoESP.h"
#include "configManager.h"

// Pin definition             GPIO numbers
#define ButtonEnterSettings   13  //button to enter settings
#define ButtonOutputControl   0   //button to manually control the output
#define statusLed             14  //Don't use LED_BUILTIN cause some problems, since its connected to Wifi chip.
#define OutputDeviceLed       2

#define ButtonON(btn)         (digitalRead(btn) == 0) //Active low
#define ButtonSettings        ButtonON(ButtonEnterSettings)
#define ButtonOnOff           ButtonON(ButtonOutputControl)

//to prevent continuous on / off operation from network.
#define networkControlDelay   3000 //ms I recommend 10000 ms
#define buttonDebounceDelay   40 //ms

#define DEBUG_MAIN Serial

#ifdef DEBUG_MAIN
  #define DBG(x, ...) DEBUG_MAIN.printf(x , ##__VA_ARGS__)
#else
  #define DBG(x, ...)
#endif

// Enable DTIM sleep for ESP
//#define ESPSleep

fauxmoESP fauxmo;
configManager cfgMngr(setLedStatus);

bool SwitchState = false;
bool SendFlag = false;

unsigned char Device_id = 0;
unsigned char Device_value = 255; //set to max
int Device_led_value = 0; //OFF
int Device_set_value = 0; //OFF
const int Device_led_step = 10;


void ledFadeOutput(void) {
  //Led Brightness fade
  static unsigned long last = millis();
  if (millis() - last > 25) {
    last = millis();
    if (Device_led_value < Device_set_value) {
      Device_led_value += Device_led_step;
      if (Device_led_value > Device_set_value) Device_led_value = Device_set_value;
    }
    if (Device_led_value > Device_set_value) {
      Device_led_value -= Device_led_step;
      if (Device_led_value < Device_set_value) Device_led_value = Device_set_value;  
    }
    analogWrite(OutputDeviceLed, Device_led_value);
  }
}

void AllLed(bool stat){
  setLedStatus(stat);
}

void statusLedFlick(void){
  static bool t = 1;
  setLedStatus(t);
  t=!t;
}

void setLed(char ledpin, bool stat){
  if (stat == true){
    digitalWrite(ledpin, HIGH);//ON
  }else{
    digitalWrite(ledpin, LOW); //OFF
  }
}

void setLedStatus(bool stat) {
  setLed(statusLed, stat);
}

void setup() {
  pinMode(ButtonEnterSettings, INPUT_PULLUP);//Button (H:OFF, L:ON)
  pinMode(ButtonOutputControl, INPUT_PULLUP);//Button (H:OFF, L:ON)  

  pinMode(OutputDeviceLed, OUTPUT);       //Output (H:ON, L:OFF)
  pinMode(statusLed, OUTPUT);   //Status LED (H:ON, L:OFF)

  //set default states
  analogWrite(OutputDeviceLed, 0);  //power OFF
  AllLed(false);    //All LEDs OFF

  delay(200);
  //I'm alive
  AllLed(true);     //All LEDs ON
  delay(200);
  AllLed(false);    //All LEDs OFF
  delay(200);

#ifdef DEBUG_MAIN
  DEBUG_MAIN.begin(115200);
  delay(1);//after begin we need small time for print.
  DEBUG_MAIN.println("");
  delay(100);
#endif

  // to disable soft AP after restart
  WiFi.mode(WIFI_OFF);
  delay(100);

  cfgMngr.readWifiCredentials();
  if (configModeCheckAndWifiConn(cfgMngr.getWifiSsid(), cfgMngr.getWifiPass(), cfgMngr.getWifiHost()) == true) return;

//selects fauxmoESP version automatically
#ifdef FAUXMO_TCP_PORT
  cfgMngr.setConfigModeCallback([]{ fauxmo.enable(false); });

  // By default, fauxmoESP creates it's own webserver on the defined port
  // The TCP port must be 80 for gen3 devices (default is 1901)
  // This has to be done before the call to enable()
  fauxmo.createServer(true); // not needed, this is the default value
  fauxmo.setPort(80); // This is required for gen3 devices
#else
  #error This example only works with fauxmoESP v3.4
#endif

  // You have to call enable(true) once you have a WiFi connection
  // You can enable or disable the library at any moment
  // Disabling it will prevent the devices from being discovered and switched
  fauxmo.enable(true);

  // You can use different ways to invoke alexa to modify the devices state:
  // "Alexa, turn SmartDevice on"
  // "Alexa, turn on SmartDevice
  // "Alexa, set SmartDevice to fifty percent"

  // Add virtual devices
  fauxmo.addDevice(cfgMngr.getDeviceName());

  // Callback set state
  fauxmo.onSetState(onSetStateCallback);

  // Callback get state
  fauxmo.onGetState(onGetStateCallback);
}

// returns true if entered config mode
bool configModeCheckAndWifiConn(const char* ssid, const char* pass, const char* host) {
  if (ssid != NULL && pass != NULL) {
    //if user entered config mode return
    return wifiSetup(ssid, pass, host);
  } else {
    DBG("[WIFI] Error Wifi ssid, pass are not set.\n");
    for (int i = 0; i <= 6; i++) {
      statusLedFlick();
      delay(50);
    }
    setLedStatus(false);
    while (1) {
      if (configCheckAndInputOutputControl()) break;
      delay(100);// give time to do it's business, otherwise soft wdt reset occurs
    }
    return true;
  }
}

bool configCheckAndInputOutputControl() {
  //check config mode
  bool configMode = cfgMngr.handle(ButtonSettings);
  if (!configMode) {
    //check user input
    outputControl();
  }
  return configMode;
}

bool wifiSetup(const char* ssid, const char* pass, const char* host) {
  bool configMode = false;

  // Set WIFI module to STA mode
  WiFi.mode(WIFI_STA);

#if defined(ESP8266)
  #ifdef ESPSleep
    WiFi.setSleepMode(WIFI_LIGHT_SLEEP);
    delay(1);
  #endif
  if (host != NULL) WiFi.hostname(host);
#elif defined (ARDUINO_RASPBERRY_PI_PICO_W)
  if (host != NULL) WiFi.setHostname(host);
#endif

  // Connect
  DBG("[WIFI] Connecting to %s \n", ssid);
  WiFi.begin(ssid, pass);

  // Wait for wifi connection
  while (1) {
    //check wifi connection
    static unsigned long last = millis();
    if (millis() - last > 500) {
      last = millis();
      if (WiFi.status() == WL_CONNECTED) {
        // Connected!
        #ifndef ARDUINO_RASPBERRY_PI_PICO_W
          DBG("[WIFI] STATION Mode, SSID: %s, IP address: %s, Host: %s\n", \
                          WiFi.SSID().c_str(), WiFi.localIP().toString().c_str(), host);
        #else
          DBG("[WIFI] STATION Mode, SSID: %s, IP address: %s, Host: %s\n", \
                          WiFi.SSID(), WiFi.localIP().toString().c_str(), host);
        #endif
        break;
      }
      DBG(".");
      statusLedFlick();
    }

    configMode = configCheckAndInputOutputControl();
    if (configMode) break;

    delay(100);// give time to do it's business, otherwise soft wdt reset occurs.
  }

  if (!configMode) setLedStatus(false);//LED OFF
  return configMode;
}

void onSetStateCallback(unsigned char device_id, const char * device_name, bool state, unsigned char value) {
  DBG("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
  SwitchState = state;
  Device_id = device_id;
  Device_value = value;
  SendFlag = true;
}

void onGetStateCallback(unsigned char device_id, const char * device_name, bool &state, unsigned char &value) {
  DBG("onGetStateCallback\n");
  state = SwitchState;
  value = Device_value;
}

// Button push and release check with state machine for debouncing.
bool buttonCheck(bool button_stat) {
  static int level = 0;
  static unsigned long lastP;
  bool status = false;

  switch(level){
    case 0:
      if (button_stat == true) {
        lastP = millis();
        level++;
      }        
      break;
    case 1:
      // push check
      if (button_stat == true) {
        if (millis() - lastP > buttonDebounceDelay) {
          lastP = millis();
          level++;
          DBG("sw pushed\n");
        }
      }else{
        level = 0;
      }
      break;
    case 2:
      //release check
      if (button_stat == false) {
        if (millis() - lastP > buttonDebounceDelay) {
          SwitchState = !SwitchState;
          status = true;
          level = 0;
          DBG("sw release\n");
        }
      }
      break;
  }
  
  return status;
}

void outputControl(void) {
  static unsigned long tmr;
  static bool disableNetReq = false;
  bool localReq = buttonCheck(ButtonOnOff);
  bool networkReq = (SendFlag && !disableNetReq);
  if (networkReq || localReq) {
    switch (Device_id) {
      case 0:
        if (SwitchState == true){
          DBG("Plug ON\n");
          Device_set_value = Device_value;
        }else{
          DBG("Plug OFF\n");
          Device_set_value = 0;
        }
        break;
    }
  }
  if (networkReq) {
    SendFlag = false;
    tmr = millis();
    disableNetReq = true;
  }
  if (disableNetReq && (millis() - tmr > networkControlDelay)) {
    disableNetReq = false;
    DBG("Network request re-enabled\n");
  }
  ledFadeOutput();
}

void loop_fauxmo() {
  // fauxmoESP uses an async TCP server but a sync UDP server
  // Therefore, we have to manually poll for UDP packets
  fauxmo.handle();
  
  // Check Wifi connection and memory leaks
  static unsigned long last = millis();
  if (millis() - last > 5000) {
    last = millis();
    DBG("[MAIN] Free heap: %d bytes\n", MCU.getFreeHeap());
    if (WiFi.status() != WL_CONNECTED) {
      WiFi.disconnect();
      DBG("[MAIN] Wifi disconnected. Rebooting\n");
      delay(100);
      MCU.restart();
    }
  }

  outputControl();

#if defined(ESP8266) && defined (ESPSleep)
  //needed for sleep op
  delay(310);//more then 300ms (DTIM val 3 x 100ms))
#endif
}

void loop() {
  if (cfgMngr.handle(ButtonSettings) == false) loop_fauxmo();
}