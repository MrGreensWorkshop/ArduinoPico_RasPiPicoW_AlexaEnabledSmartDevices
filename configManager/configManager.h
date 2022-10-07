/*
 * Config Manager v1.0.0
 * 
 * Copyright (c) 2022 Mr. Green's Workshop https://www.MrGreensWorkshop.com
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef CONFIG_MANAGER_H_
#define CONFIG_MANAGER_H_

#include "configManagerSettings.h"

#include <Arduino.h>
#if defined(ESP8266)
  #include <ESP8266WebServer.h>
  #include <ESP8266HTTPUpdateServer.h>
#elif defined(ARDUINO_RASPBERRY_PI_PICO_W)
  #include <WebServer.h>
  #include <HTTPUpdateServer.h>
  #ifdef DNS_ON
    #include <DNSServer.h>
  #endif
#endif
#include <functional>

#ifdef USE_FILE_FS
  #if defined(ESP8266)
    #define FILE_SYS SPIFFS
    #include <FS.h>
  #elif defined(ARDUINO_RASPBERRY_PI_PICO_W)
    #define FILE_SYS LittleFS
    #include <LittleFS.h>
  #endif
#elif defined (USE_EEPROM)
  #include <EEPROM.h>
#endif

//#define DEBUG_CNFG_MNGR Serial

typedef std::function<void(bool)> TButtonSetStateCallback;
typedef std::function<void(bool)> TLedCallback;
typedef std::function<void(void)> TConfigModeCallback;
#if defined(ESP8266)
  typedef std::function<void(ESP8266WebServer*)> TCustomWebPathsCallback;
#elif defined(ARDUINO_RASPBERRY_PI_PICO_W)
  typedef std::function<void(WebServer*)> TCustomWebPathsCallback;
#endif

// Constants
#define ssid_len 32 + 1
#define pass_len 64 + 1
#define host_len 16 + 1
#ifdef add_device_name
  #define device_len 16 + 1
#endif

struct configCredentials_t {
  char ssid[ssid_len];
  char pass[pass_len];
  char host[host_len];
#ifdef add_device_name
  char device[device_len];
#endif
} __attribute__ ((packed));

#ifdef USE_EEPROM
  #define EEpromStartAddr (0)
  #define EEpromSize sizeof(configCredentials_t)
  #define STR_END (0xFF)
  static_assert(!(EEpromSize > 4096), "Max available EEprom Size is 4096");
#elif defined (USE_FILE_FS)
  #define STR_END (0x00)
#endif

class configManager {
  public:
    explicit configManager(TLedCallback fnLed = nullptr, TCustomWebPathsCallback fnWebPath = nullptr);
    bool handle(bool button_stat);
    bool getConfigMode(void);
    void readWifiCredentials(void);
    const char* getWifiSsid(void);
    const char* getWifiPass(void);
    const char* getWifiHost(void);
#ifdef add_device_name
    const char* getDeviceName(void);
#endif
    // mode: true (enter), false (exit) config mode
    void configModeSetup(bool mode);
    void setConfigModeCallback(TConfigModeCallback fnConfigMode = nullptr);

  private:
    //Default Wifi Host Name
    static constexpr const char* wifi_host_default = default_wifi_host_name;
    
    //Default Wifi Credentials
    static constexpr const char* wifi_ssid_default = default_wifi_ssid;
    static constexpr const char* wifi_pass_default = default_wifi_pass;

    //Default Device Name
#ifdef add_device_name
    static constexpr const char* device_name_default = default_device_name;
#endif

    //Firmware update HTTP Auth Credentials
    static constexpr const char* firmUpdateUsername = config_firm_update_http_user;
    static constexpr const char* firmUpdatePassword = config_firm_update_http_pass;
    
    //Config Wifi AP Credentials
    static constexpr const char* ap_ssid = config_ap_ssid;
    static constexpr const char* ap_pass = config_ap_pass;

    unsigned long configMode_timeout = config_mode_timeout;
  #ifdef DNS_ON
    const byte DNS_PORT = 53;
  #endif
    bool configMode;
    bool firmwareUpdateFlg;
    TLedCallback _ledStat = nullptr;
    TCustomWebPathsCallback _customWebPathsCallback = nullptr;
    TConfigModeCallback _configModeCallback = nullptr;

    //internal functions
    void resetDeinit(void);
    void redirect(String url);
    void web404(void);
    void webRoot(void);
    void webRestart(void);
    void webRebootMsg(void);
    void webConfigCredentials(void);
    void webConfigCredentialsSave(void);
#ifdef add_delete_credentials_button
    void webConfigCredentialsDelete(void);
#endif
    void webFirmUpdatePrepare(void);
    void webAddInput(const char* input, const char* value);

    //period in ms
    bool buttonCheck(bool button_stat, uint16_t period = 500, uint16_t check_count = 10, TButtonSetStateCallback fn = nullptr);
    void pushCheck(bool button_stat);
    void httpServerConfigSetup(void);
    void saveWifiCredentials(void);
    void deleteWifiCredentials(void);

#ifdef USE_FILE_FS
    static constexpr const char* configFilePath = "/config.txt";
    void fs_init(bool createNewSettingFile = false);
    bool fs_WriteSettings(void);
    void fs_ReadConfig(void);
    void fs_deleteConfig(void);
    //just in case needed
    void fs_format(void);
    void fs_writeWifiConfig(void);
#elif defined(USE_EEPROM)
    void clearEEpromSettings(void);
#endif

  protected:
#if defined(ESP8266)
    ESP8266WebServer* httpServer;
    ESP8266HTTPUpdateServer* httpUpdater;
#elif defined(ARDUINO_RASPBERRY_PI_PICO_W)
    WebServer* httpServer;
    HTTPUpdateServer* httpUpdater;
  #ifdef DNS_ON
    DNSServer* dnsServer;
  #endif
#endif
    configCredentials_t configCredentials;
};


#endif  // CONFIG_MANAGER_H_
