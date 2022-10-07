/*
 * Config Manager v1.0.0
 * 
 * Copyright (c) 2022 Mr. Green's Workshop https://www.MrGreensWorkshop.com
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef CONFIG_MANAGER_SETTINGS_H_
#define CONFIG_MANAGER_SETTINGS_H_

#define DNS_ON //only for Raspberry Pi Pico W for now.

//where to save configs. USE_EEPROM or USE_FILE_FS choose one of them
//for Raspberry Pi Pico W, if you set USE_FILE_FS, in Arduino IDE, Click Tools > Flash Size > select FS: 64KB
#define USE_EEPROM

//adds device name to settings (for plugs) (to disable comment it)
#define add_device_name

//adds delete credentials button to settings (to disable comment it)
#define add_delete_credentials_button

//Default Wifi Host Name (to disable set to NULL)
#define default_wifi_host_name "NewIotDev"

//Default Wifi Credentials (to disable set to NULL)
#define default_wifi_ssid NULL
#define default_wifi_pass NULL

//Default Device Name (to disable set to NULL)
#ifdef add_device_name
#define default_device_name "SmartDevice"
#endif

//Config Wifi AP Credentials
// http://192.168.42.1
#define config_ap_ssid "SmartDeviceConfig"
#define config_ap_pass "adminadmin"

//Firmware update HTTP Auth Credentials (to disable http auth set both to nullptr)
#define config_firm_update_http_user "admin"           
#define config_firm_update_http_pass "your_long_pass"

//Config mode exit timeout, set 0 to disable it.
#define config_mode_timeout (5UL * 60000UL) //5min

#endif  // CONFIG_MANAGER_SETTINGS_H_
