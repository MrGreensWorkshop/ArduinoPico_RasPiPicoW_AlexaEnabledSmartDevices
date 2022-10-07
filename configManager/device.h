/*
 * Config Manager v1.0.0
 * 
 * Copyright (c) 2022 Mr. Green's Workshop https://www.MrGreensWorkshop.com
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef __DEVICE_H__
#define __DEVICE_H__

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #define MCU ESP
#elif defined(ARDUINO_RASPBERRY_PI_PICO_W)
  #include <pico/cyw43_arch.h>
  #define MCU rp2040
#else
	#error Platform not supported
#endif

#endif //   __DEVICE_H__