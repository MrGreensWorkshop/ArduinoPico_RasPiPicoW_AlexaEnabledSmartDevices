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

I also made Config Manager for ease of installation. I'm happy if you consider [supporting me](#you-can-support-my-work). Thanks.

### Features

- Fauxmo Belkin Plug for Raspberry Pi Pico W (ported from: FauxmoESP v2.4.4)
- Fauxmo Philips Light for Raspberry Pi Pico W (ported from: FauxmoESP v3.4)
- Config Manager
  - Wifi and device settings over Web interface
  - Firmware update and FileSystem upload feature with HTTP basic authentication
- Examples
  - DimmableSmartLight
    - On/Off control and brightness control of a Led
  - SmartLight
    - On/Off control of digital output
  - FauxmoPhilipsLight_Basic
    - Basic example of Fauxmo Philips Light (On/Off)
  - FauxmoBelkinPlug_Basic
    - Basic example of Fauxmo Belkin Plug (On/Off)

### Release (example.zip)

I put Config Manager and Fauxmo to corresponding sample folders so that you can open and compile projects easily. I've also included precompiled binaries for Raspberry Pi Pico W, for examples with Config Manager.

```
<examples>/
│
├── DimmableSmartLight
│   ├── <Config Manager>
│   └── src
│       └── <FauxmoPhilipsLight>
├── SmartLight
│   ├── <Config Manager>
│   └── src
│       └── <FauxmoPhilipsLight> (interchangeable with FauxmoBelkinPlug)
│           (If you want to change it, replace src folder with FauxmoBelkinPlug_Basic/src)
├── FauxmoPhilipsLight_Basic
│   └── src
│       └── <FauxmoPhilipsLight>
└── FauxmoBelkinPlug_Basic
    └── src
        └── <FauxmoBelkinPlug>
```

### Compilation

1. Check out [this video](https://www.youtube.com/channel/UCf-BfpqjqvPKGXFc22rkR9w) on YouTube.
1. Install [Arduino IDE](https://www.arduino.cc/en/software) 1.8 or 2.0 to your OS.
1. Install [Raspberry Pi Pico Arduino core](https://github.com/earlephilhower/arduino-pico).
1. Install [AsyncTCP_RP2040W library](https://github.com/khoih-prog/AsyncTCP_RP2040W).
1. From the [latest release](https://github.com/MrGreensWorkshop/arduinoPico_RasPiPicoW_AlexaEnabledSmartDevices/releases/latest), download the "examples.zip" under the Assets, and unzip.
1. Open one of the examples in Arduino IDE.
1. Select the board from, `Menu > Tools > Board > Raspberry Pi RP2040 boards > Raspberry Pi Pico W`.
1. Select your Wifi region from, `Menu > Tools > WiFi Region`.
1. To compile and get the binary, click `Menu > Sketch > Export compiled Binary` and your binary will be exported to opened example folder.

### Running

1. Build this test circuit for Raspberry Pi Pico W

<img src="/docs/CircuitSketchForRaspberryPiPicoW.jpg" height="300" alt="Amazon Alexa enabled smart device test circuit for Raspberry Pi Pico W">

2. Get the binary
    - You can compile examples and get the binary as explained above.
    - Or you can use precompiled binary files inside of each example folder if applicable.
1. Put the Raspberry Pi Pico W into bootloader mode by pushing bootsel button, while plugging to your computer. 
1. Copy the ".uf2" file to your Raspberry Pi Pico W's storage.

### Current Status

#### Raspberry Pi Pico W

| Arduino core | AsyncTCP_RP2040W | Fauxmo | Gen | Works |
| :----------- | ---------------- | ------ | --- | ----- |
| [v2.6.0](https://github.com/earlephilhower/arduino-pico/tree/2.6.0) | [v1.1.0](https://github.com/khoih-prog/AsyncTCP_RP2040W/tree/v1.1.0) |	[Fauxmo Belkin Plug v1.0.0](/library/FauxmoBelkinPlug/src/) | 1,2 | OK |
| [v2.6.0](https://github.com/earlephilhower/arduino-pico/tree/2.6.0) | [v1.1.0](https://github.com/khoih-prog/AsyncTCP_RP2040W/tree/v1.1.0) |	[Fauxmo Philips Light v1.0.0](/library/FauxmoPhilipsLight/src/) | 1,2 | OK |

#### ESP8266

| Arduino core | ESPAsyncTCP      | Fauxmo | Gen | Works |
| :----------- | ---------------- | ------ | --- | ----- |
| [v2.3.0](https://github.com/esp8266/Arduino/tree/2.3.0) | [v1.1.2](https://github.com/me-no-dev/ESPAsyncTCP/tree/a57560d977602145cfadae84ae28f2ec69b361f5) | [Fauxmo Belkin Plug v1.0.0](/library/FauxmoBelkinPlug/src/) | 1,2 | OK |

Fauxmo Philips Light should be supported all three generation of Amazon echo devices. Since I only have first and second generation Amazon Echo devices, I didn't test with third generation devices. Please let me know if you have third generation devices and if it's working. If you don't know which device you have, please check [this page](https://www.amazon.com/gp/help/customer/display.html?nodeId=GHRYQ6GHE4A5TUD2).

Fauxmo Belkin Plug only works with Gen 1,2 because to work with Gen 3, TCP port must be fixed to 80.

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

| Library                              | file(s) |
| :----------------------------------- | ------- |
| [FauxmoESP v3.4](https://github.com/vintlabs/fauxmoESP/tree/3.4), Copyright (C) 2016-2020 by Xose Pérez, 2020 by Paul Vint. FauxmoESP is distributed under the terms of the MIT License. | [FauxmoBelkinPlug](https://github.com/MrGreensWorkshop/arduinoPico_RasPiPicoW_AlexaEnabledSmartDevices/tree/95f45b6efa152797f380ea1a021564a6f752f7ff/library/FauxmoBelkinPlug/src) |
| [FauxmoESP v2.4.4](https://github.com/vintlabs/fauxmoESP/tree/2.4.4), Copyright (C) 2016-2018 by Xose Pérez. FauxmoESP is distributed under the terms of the MIT License. | [FauxmoPhilipsLight](https://github.com/MrGreensWorkshop/arduinoPico_RasPiPicoW_AlexaEnabledSmartDevices/tree/95f45b6efa152797f380ea1a021564a6f752f7ff/library/FauxmoPhilipsLight/src) |
