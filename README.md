## Amazon Alexa enabled smart devices with Raspberry Pi Pico W


[<img src="https://img.shields.io/github/issues/MrGreensWorkshop/arduinoPico_RasPiPicoW_AlexaEnabledSmartDevices" alt="GitHub issues" data-no-image-viewer>](https://github.com/MrGreensWorkshop/arduinoPico_RasPiPicoW_AlexaEnabledSmartDevices/issues)
[<img src="https://img.shields.io/github/forks/MrGreensWorkshop/arduinoPico_RasPiPicoW_AlexaEnabledSmartDevices" alt="GitHub forks" data-no-image-viewer>](https://github.com/MrGreensWorkshop/arduinoPico_RasPiPicoW_AlexaEnabledSmartDevices/blob/main/README.md#readme)
[<img src="https://img.shields.io/github/stars/MrGreensWorkshop/arduinoPico_RasPiPicoW_AlexaEnabledSmartDevices" alt="GitHub stars" data-no-image-viewer>](https://github.com/MrGreensWorkshop/arduinoPico_RasPiPicoW_AlexaEnabledSmartDevices/blob/main/README.md#readme)
[<img src="https://img.shields.io/github/license/MrGreensWorkshop/arduinoPico_RasPiPicoW_AlexaEnabledSmartDevices" alt="GitHub license" data-no-image-viewer>](https://github.com/MrGreensWorkshop/arduinoPico_RasPiPicoW_AlexaEnabledSmartDevices/blob/main/LICENSE.txt)
[<img src="https://shields.io/badge/Github%20Sponsors-Support%20me-blue?logo=GitHub+Sponsors" alt="Support me on GitHub Sponsors" data-no-image-viewer>](https://github.com/sponsors/MrGreensWorkshop "Support me on GitHub Sponsors")
[<img src="https://shields.io/badge/Patreon-Support%20me-blue?logo=Patreon" alt="Support me on Patreon" data-no-image-viewer>](https://patreon.com/MrGreensWorkshop "Support me on Patreon")
[<img src="https://shields.io/badge/Ko--fi-Tip%20me-blue?logo=kofi" alt="Tip me via Ko-fi" data-no-image-viewer>](https://ko-fi.com/MrGreensWorkshop "Tip me via Ko-fi")

---

### Introduction

There is an open source project called [FauxmoESP](https://github.com/vintlabs/fauxmoESP) which emulates lights and plugs for Amazon Echo devices using ESP microcontrollers. Back in the day, I used to use this library with ESP8266 to make Alexa enabled devices. So I ported to Raspberry Pi Pico W and added some features that I think were necessary.

I also made Config manager for ease of installation. I'm happy if you consider [supporting me](#you-can-support-my-work). Thanks.

### Features

- FauxmoBelkinPlug for Raspberry Pi Pico W (ported from: FauxmoESP v2.4.4)
- FauxmoPhilipsLight for Raspberry Pi Pico W (ported from: FauxmoESP v3.4)
- Config manager
  - Wifi and device settings over Web interface
  - Firmware update and FileSystem upload feature with HTTP basic authentication
- Examples
  - Stock example of FauxmoBelkinPlug
  - Stock example of FauxmoPhilipsLight
  - Smart light
  - Dimmable smart light

### Compilation

1. Check out [this video](https://www.youtube.com/channel/UCf-BfpqjqvPKGXFc22rkR9w) on YouTube.
1. Install [Arduino IDE](https://www.arduino.cc/en/software) 1.8 or 2.0 to your OS.
1. Install [Raspberry Pi Pico Arduino core](https://github.com/earlephilhower/arduino-pico).
1. Install [AsyncTCP_RP2040W library](https://github.com/khoih-prog/AsyncTCP_RP2040W).
1. [Download](https://github.com/MrGreensWorkshop/arduinoPico_RasPiPicoW_AlexaEnabledSmartDevices/releases/latest) the least release named "examples.zip" and Unzip.
1. Open one of the examples in Arduino IDE.
1. Select the board from, `Menu > Tools > Board > Raspberry Pi RP2040 boards > Raspberry Pi Pico W`.
1. Select your Wifi region from, `Menu > Tools > WiFi Region`.
1. To compile and get the binary, click "Export compiled Binary" from, `Menu > Sketch > WiFi Region` and your binary will be exported to currently opened example folder.

### Running

1. Get the binary
    - You can compile examples and get the binary as explained above.
    - Or you can use precompiled binary files inside of each example folder.
1. Put the Raspberry Pi Pico into bootloader mode by pushing bootsel button, while plugging to your computer. 
1. Copy the ".UF2" file to your Raspberry Pi Pico W's storage.

### Current Status

#### Raspberry Pi Pico W

| Arduino core | AsyncTCP_RP2040W | Fauxmo | Gen | Works |
| :----------- | ---------------- | ------ | --- | ----- |
| v2.6.0 | v1.1.0 |	FauxmoBelkinPlug | 1,2 | OK |
| v2.6.0 | v1.1.0 |	FauxmoPhilipsLight | 1,2 | OK |

#### ESP8266

| Arduino core | ESPAsyncTCP      | Fauxmo | Gen | Works |
| :----------- | ---------------- | ------ | --- | ----- |
| v2.3.0 | v1.1.0 | FauxmoBelkinPlug | 1,2 | OK |

FauxmoPhilipsLight should be supported all three generation of Amazon echo devices. Since I only have first and second generation Amazon Echo devices, I didn't test with third generation devices. Please let me know if you have third generation devices and if it's working. If you don't know which device you have, please check [this page](https://www.amazon.com/gp/help/customer/display.html?nodeId=GHRYQ6GHE4A5TUD2).

FauxmoBelkinPlug only works with Gen 1,2 because only one port(80) is allowed. 

### Documentation

Check out [this video](https://www.youtube.com/channel/UCf-BfpqjqvPKGXFc22rkR9w) on YouTube

### Credits

I want to thank all projects that gave me an opportunity to make this project possible. Please consider to support these projects too.

- [FauxmoESP](https://github.com/vintlabs/fauxmoESP) This is a library for ESP8266/ESP32-based devices that emulates Philips Hue lights and thus allows you to control them using this protocol, in particular from Alexa-powered devices like the Amazon Echo or the Dot.
- [Raspberry Pi Pico Arduino core](https://github.com/earlephilhower/arduino-pico) Raspberry Pi Pico Arduino core, for all RP2040 boards.
- [AsyncTCP_RP2040W library](https://github.com/khoih-prog/AsyncTCP_RP2040W) Asynchronous TCP library for Raspberry Pi Pico W with CYW43439 WiFi.

### You Can Support My Work

Creating projects like this takes a great amount of time. Much appreciated if you consider supporting me so that I can continue projects like this and creating new contents for everyone.

- You can support me on [GitHub Sponsors](https://github.com/sponsors/MrGreensWorkshop "Support me on GitHub Sponsors") (monthly or one time)
- You can be one of my patrons on [Patreon](https://patreon.com/MrGreensWorkshop "Be my Patron") (monthly)
- You can tip me via [Ko-fi](https://ko-fi.com/MrGreensWorkshop "Tip Me via Ko-fi") (one time)

### Contribute

Pull Requests are welcome. Please check the instructions in the Issues and Pull Request templates.
<!--
### Contributors

Thank you for your contributions!
-->
### License

As it says in the [MIT license](https://github.com/MrGreensWorkshop/arduinoPico_RasPiPicoW_AlexaEnabledSmartDevices/blob/main/LICENSE.txt), you can use my code anywhere as long as you include the license and copyright notice.

`Copyright (c) 2022 Mr. Green's Workshop https://www.MrGreensWorkshop.com`

Please add link to my page so I can get credit for my work.

Thank you!

### Other Licenses

This project incorporates libraries written below. Without these libraries, I couldn't make this project possible.

| Library                              | file |
| :----------------------------------- | ---- |
| [FauxmoESP v3.4](https://github.com/vintlabs/fauxmoESP/tree/3.4), Copyright (C) 2016-2020 by Xose Pérez, 2020 by Paul Vint. FauxmoESP is distributed under the terms of the MIT License. | [/library/FauxmoBelkinPlug/src](https://github.com/MrGreensWorkshop/arduinoPico_RasPiPicoW_AlexaEnabledSmartDevices/tree/95f45b6efa152797f380ea1a021564a6f752f7ff/library/FauxmoBelkinPlug/src) |
| [FauxmoESP v2.4.4](https://github.com/vintlabs/fauxmoESP/tree/2.4.4), Copyright (C) 2016-2018 by Xose Pérez. FauxmoESP is distributed under the terms of the MIT License. | [/library/FauxmoPhilipsLight/src](https://github.com/MrGreensWorkshop/arduinoPico_RasPiPicoW_AlexaEnabledSmartDevices/tree/95f45b6efa152797f380ea1a021564a6f752f7ff/library/FauxmoPhilipsLight/src) |