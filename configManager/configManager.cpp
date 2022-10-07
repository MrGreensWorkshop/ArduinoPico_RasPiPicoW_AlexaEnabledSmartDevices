/*
 * Config Manager v1.0.0
 * 
 * Copyright (c) 2022 Mr. Green's Workshop https://www.MrGreensWorkshop.com
 *
 * SPDX-License-Identifier: MIT
 */

#include "configManager.h"
#include "configManager_html.h"
#include "device.h"

#ifdef DEBUG_CNFG_MNGR
  #define DBG(x, ...) DEBUG_CNFG_MNGR.printf("[CNFG DBG] " x "\n", ##__VA_ARGS__)
#else
  #define DBG(x, ...)
#endif

configManager::configManager(TLedCallback fnLed, TCustomWebPathsCallback fnWebPath):
firmwareUpdateFlg(false)
,configMode(false)
,httpServer(nullptr)
,httpUpdater(nullptr)
#ifdef DNS_ON
,dnsServer(nullptr)
#endif
{
  _ledStat = fnLed;
  _customWebPathsCallback = fnWebPath;
}

void configManager::resetDeinit(void) {
  httpServer->close();
  DBG("ServerClose");
  WiFi.softAPdisconnect();
  DBG("AP disconnect");
  WiFi.mode(WIFI_OFF);
  DBG("Wifi Off");
  delay(100);

  MCU.restart();
}

void configManager::redirect(String url) {
  httpServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  httpServer->sendHeader("Location", url);
  httpServer->sendHeader("Cache-Control", "no-cache");
  httpServer->send(301);
}

void configManager::web404(void) {
  redirect("/");
}

void configManager::webRoot(void) {
  httpServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  httpServer->send_P(200, PSTR("text/html"), htmlStart);
  httpServer->sendContent_P(htmlIndexPageStart);
  if (_customWebPathsCallback != nullptr) httpServer->sendContent_P(htmlIndexPageCustomButton);
  httpServer->sendContent_P(htmlIndexPageEnd);
  httpServer->sendContent_P(htmlEnd);
  firmwareUpdateFlg = false;
}

void configManager::webRebootMsg(void) {
  httpServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  httpServer->send_P(200, PSTR("text/html"), htmlRebootMsg);
  delay(1000);
}

void configManager::webRestart(void) {
  webRebootMsg();
  resetDeinit();
}

void configManager::webAddInput(const char* input ,const char* value) {
  httpServer->sendContent_P(input);
  if (value != NULL) {
    httpServer->sendContent_P(PSTR(" value='"));
    httpServer->sendContent(value);
    httpServer->sendContent_P(PSTR("'"));
  }
  httpServer->sendContent_P(PSTR("><br><br>"));
}

void configManager::webConfigCredentials(void) {
  httpServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  httpServer->send_P(200, PSTR("text/html"), htmlStart);
  httpServer->sendContent_P(htmlCredentialsPage);

  webAddInput(htmlSsidInput, getWifiSsid());
  webAddInput(htmlPassInput, getWifiPass());
  webAddInput(htmlHostInput, getWifiHost());
#ifdef add_device_name
  webAddInput(htmlDeviceInput, getDeviceName());
#endif
  httpServer->sendContent_P(htmlIndexPageCredentialsSaveButton);
#ifdef add_delete_credentials_button
  httpServer->sendContent_P(htmlIndexPageDeleteCredentialsButton);
#endif
  httpServer->sendContent_P(htmlIndexPageCredentialsBackButton);
  httpServer->sendContent_P(htmlCredentialsPageEnd);
  httpServer->sendContent_P(htmlEnd);
  firmwareUpdateFlg = false;
}

void configManager::webConfigCredentialsSave(void) {
  bool save_set = false;
  if (httpServer->hasArg("ssid")) strcpy(configCredentials.ssid, httpServer->arg("ssid").c_str());
  if (httpServer->hasArg("pass")) strcpy(configCredentials.pass, httpServer->arg("pass").c_str());
  if (httpServer->hasArg("host")) strcpy(configCredentials.host, httpServer->arg("host").c_str());
#ifdef add_device_name
  if (httpServer->hasArg("dev")) strcpy(configCredentials.device, httpServer->arg("dev").c_str());
#endif
  if (httpServer->hasArg("ssid") && httpServer->hasArg("pass")) {
    save_set = true;
  }else{
    DBG("ssid / pass not entered");
  }
  
  if (save_set == true) {
    webRebootMsg();
    saveWifiCredentials();
    resetDeinit();
  }
}

void configManager::webConfigCredentialsDelete(void) {
  webRebootMsg();
  deleteWifiCredentials();
  resetDeinit();
}

void configManager::webFirmUpdatePrepare(void) {
  firmwareUpdateFlg = true;
  // just in case if we need to do deinit job.
  redirect("/update");
}

void configManager::httpServerConfigSetup(void) {
  WiFi.disconnect();
  delay(100);
#ifdef DNS_ON
  IPAddress apIP(192, 168, 42, 1); //currently ip cannot be changed because there is a problem on softAPConfig.
  WiFi.softAPConfig(apIP, apIP, IPAddress (255, 255, 255, 0));
#endif
  WiFi.softAP(ap_ssid, ap_pass);
  delay(500);

#if defined(ESP8266)
  httpServer = new ESP8266WebServer(80);
  httpUpdater = new ESP8266HTTPUpdateServer;  
#elif defined(ARDUINO_RASPBERRY_PI_PICO_W)
  httpServer = new WebServer(80);
  httpUpdater = new HTTPUpdateServer;
#ifdef DNS_ON
  dnsServer = new DNSServer;
#endif
#endif
#ifdef DNS_ON
  dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer->start(DNS_PORT, "*", apIP);
#endif

  httpServer->on("/", std::bind(&configManager::webRoot, this));
  httpServer->on("/generate_204", std::bind(&configManager::webRoot, this));  // Android captive portal. Leave it just in case. 
  httpServer->on("/fwlink", std::bind(&configManager::webRoot, this));        // Microsoft captive portal. Leave it just in case. 
  httpServer->on("/credentials", std::bind(&configManager::webConfigCredentials, this));
  httpServer->on("/save", std::bind(&configManager::webConfigCredentialsSave, this));
#ifdef add_delete_credentials_button
  httpServer->on("/deletecredentials", std::bind(&configManager::webConfigCredentialsDelete, this));
#endif
  httpServer->on("/firmupdate", std::bind(&configManager::webFirmUpdatePrepare, this));
  httpServer->on("/restart", std::bind(&configManager::webRestart, this));
  httpServer->onNotFound(std::bind(&configManager::web404, this));
#if defined(ESP8266)
  httpUpdater->setup(httpServer, firmUpdateUsername, firmUpdatePassword);
#elif defined(ARDUINO_RASPBERRY_PI_PICO_W)
  httpUpdater->setup(httpServer, "/update", firmUpdateUsername, firmUpdatePassword);
#endif

  if (_customWebPathsCallback != nullptr) _customWebPathsCallback(httpServer);
  httpServer->begin();

  DBG("for Config Connect to SSID name: %s", ap_ssid);
  DBG("HTTPConfigServer ready! Open http://%s in your browser", WiFi.softAPIP().toString().c_str());
}

void configManager::configModeSetup(bool mode) {
  if (mode == true) {
    if (_ledStat != nullptr) {
      for (int i = 0; i <= 3; i++) {
        _ledStat(true);
        delay(50);
        _ledStat(false);
        delay(50);
      }
      _ledStat(true);
    }
    
    if (_configModeCallback != nullptr) _configModeCallback();
    httpServerConfigSetup();
    configMode = true;
  }else{
    if (_ledStat != nullptr) _ledStat(false);
    resetDeinit();
  }
}

void configManager::setConfigModeCallback(TConfigModeCallback fnConfigMode) {
  _configModeCallback = fnConfigMode;
}

/*default  check 10 times 500ms period */
bool configManager::buttonCheck(bool button_stat, uint16_t period, uint16_t check_count, TButtonSetStateCallback fn) {
  static bool pressed_flag = false;
  static unsigned long button_timestamp = millis();
  static unsigned long button_chk_cnt = 0;
  static bool button_stat_last = false;
  bool pressed_shot = false;
  
  if (millis() - button_timestamp > period) {
    button_timestamp = millis();
    
    if (button_stat != button_stat_last) {
      button_chk_cnt = 0;
    } else {

      button_chk_cnt++;
  
      if (button_chk_cnt >= check_count) {
        if (button_stat && !pressed_flag) {
          pressed_flag = true;
          DBG("button pressed");
          if(fn != nullptr) fn(button_stat);
          pressed_shot = true;
        } else if (!button_stat && pressed_flag) {
          pressed_flag = false;
          DBG("button released");
          if(fn != nullptr) fn(button_stat);
        }
        
        button_chk_cnt = 0;
      }
    }

    button_stat_last = button_stat;
  }

  return pressed_flag;
}

void configManager::pushCheck(bool button_stat) {
  static bool button_stat_last = false;

  //release check
  if (button_stat != button_stat_last) {
    if (button_stat){
      DBG("pushed");
      configModeSetup(!configMode);
    }
  }
  
  button_stat_last = button_stat;
}

bool configManager::handle(bool button_stat) {
  static bool configMode_last = false;
  
  bool button_pushed = buttonCheck(button_stat);

  pushCheck(button_pushed);
  
  if (configMode == true) {
  #ifdef DNS_ON
    if (dnsServer != nullptr) dnsServer->processNextRequest();
  #endif
    if (httpServer != nullptr) httpServer->handleClient();

    //make sure timeout set and we are not preparing for firmware update
    if (configMode_timeout > 0 && firmwareUpdateFlg == false) {
      static unsigned long firmUpdate_tmr = millis();
      if (millis() - firmUpdate_tmr > configMode_timeout) {
        configMode = false;
        DBG("Config mode timeout");
        resetDeinit();
      }
    }
  }
  
  return configMode;
}

void configManager::readWifiCredentials(void) {
#ifdef USE_EEPROM
  EEPROM.begin(EEpromSize);
  EEPROM.get(EEpromStartAddr, configCredentials);
  EEPROM.end();
#elif defined (USE_FILE_FS)
  fs_init(true);
  fs_ReadConfig();
  FILE_SYS.end();
#endif
  DBG("ssid: %s", configCredentials.ssid);
  DBG("pass: %s", configCredentials.pass);
  DBG("host: %s", configCredentials.host);
#ifdef add_device_name
  DBG("device: %s", configCredentials.device);
#endif
}

void configManager::saveWifiCredentials(void) {
#ifdef USE_EEPROM
    EEPROM.begin(EEpromSize);
    EEPROM.put(EEpromStartAddr, configCredentials);
    if (EEPROM.commit()) {
      DBG("EEPROM successfully written.");
    } else {
      DBG("ERROR! EEPROM commit failed");
    }
    EEPROM.end();
#elif defined (USE_FILE_FS)
    fs_init(false);
    fs_WriteSettings();
    FILE_SYS.end();
#endif
    DBG("ssid: %s, pass: %s, host: %s", configCredentials.ssid, configCredentials.pass, configCredentials.host);
#ifdef add_device_name
    DBG("device: %s", configCredentials.device);
#endif
  delay(100);
}

void configManager::deleteWifiCredentials(void) {
#ifdef USE_EEPROM
  clearEEpromSettings();
#elif defined (USE_FILE_FS)
  fs_deleteConfig();
#endif
  delay(100);
}

const char* configManager::getWifiSsid(void) {
  if (configCredentials.ssid[0] == STR_END || configCredentials.ssid[0] == 0) return wifi_ssid_default;
  return configCredentials.ssid;
}

const char* configManager::getWifiPass(void) {
  if (configCredentials.pass[0] == STR_END || configCredentials.pass[0] == 0) return wifi_pass_default;
  return configCredentials.pass;
}

const char* configManager::getWifiHost(void) {
  //set default name if not exist.
  if (configCredentials.host[0] == STR_END || configCredentials.host[0] == 0) return wifi_host_default;
  return configCredentials.host;
}

#ifdef add_device_name
const char* configManager::getDeviceName(void) {
  //set default name if not exist.
  if (configCredentials.device[0] == STR_END || configCredentials.device[0] == 0) return device_name_default;
  return configCredentials.device;
}
#endif

bool configManager::getConfigMode(void) {
  return configMode;
}

#ifdef USE_FILE_FS

bool configManager::fs_WriteSettings(void) {
  int writeSize = 0;
  File _fl = FILE_SYS.open(configFilePath, "w");
  if (!_fl) {
    DBG("file open failed1");
  } else {
    writeSize = _fl.write(reinterpret_cast<unsigned char*>(&configCredentials), sizeof(configCredentials_t));
    DBG("bytes written: %d", writeSize);
    _fl.close();  //Close file
  }
  if (writeSize == sizeof(configCredentials_t)){
    return true;
  }
  return false;
}

void configManager::fs_init(bool createNewSettingFile) {
  //Initialize File System
  if(FILE_SYS.begin()) {
    DBG("SPIFFS Init ok");
  } else {
    DBG("SPIFFS Init failed");
  }

  if (createNewSettingFile) {
    if (!FILE_SYS.exists(configFilePath)) {
      DBG("File not found");
      //create file with empty values
      if (fs_WriteSettings() == false) return;
    }
  }
}

void configManager::fs_ReadConfig(void) {
  File _fl = FILE_SYS.open(configFilePath, "r");
  if (!_fl) {
    DBG("file open failed2");
  } else {
    DBG("Read Config to File");
    _fl.readBytes(reinterpret_cast<char*>(&configCredentials), sizeof(configCredentials_t));
    _fl.close();  //Close file
  }
}

void configManager::fs_format(void) {
  //Format File System
  if(FILE_SYS.format()) {
    DBG("File System Formatted");
  } else {
    DBG("File System Formatting Error");
  }
}

void configManager::fs_deleteConfig(void) {
  fs_init(false);
  if (FILE_SYS.remove(configFilePath)) {
    DBG("config file deleted successfully");
  } else {
    DBG("Failed to delete config file");
  }
  FILE_SYS.end();
}

void configManager::fs_writeWifiConfig(void) {
  strcpy(configCredentials.ssid, wifi_ssid_default);
  strcpy(configCredentials.pass, wifi_pass_default);
  strcpy(configCredentials.host, wifi_host_default);
#ifdef add_device_name
  strcpy(configCredentials.device, device_name_default);
#endif
  fs_WriteSettings();
}

#elif defined (USE_EEPROM)

void configManager::clearEEpromSettings(void) {
  EEPROM.begin(EEpromSize);
  for (int i = 0 ; i < EEpromSize ; i++) {
    EEPROM.write(i, STR_END);
  }
  if (EEPROM.commit()) {
    DBG("EEPROM successfully deleted.");
  } else {
    DBG("ERROR! EEPROM commit failed");
  }
  EEPROM.end();
}


#endif
