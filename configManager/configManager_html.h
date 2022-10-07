/*
 * Config Manager v1.0.0
 * 
 * Copyright (c) 2022 Mr. Green's Workshop https://www.MrGreensWorkshop.com
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef CONFIG_MANAGER_HTML_H_
#define CONFIG_MANAGER_HTML_H_

static const char htmlStart[] PROGMEM =
R"(<!DOCTYPE html>
  <html lang='en'>
  <head>
    <meta charset='utf-8'>
    <meta name='viewport' content='width=device-width,initial-scale=1'/>
    <style>
      .button {
        color:white;
        padding:5px 10px;
        border-radius:10px;
        text-decoration:none;
      }
      .gray{
        background:#595959;
      }
      .green{
        background:#35ac48;
      }
      .blue{
        background:#4878c4;
      }
      .brown{
        background:#c09d37;
      }
      .red{
        background:#FF0000;
      }
      input:invalid:focus {
        background-color: tomato;
      }
    </style>
  </head>
<body>)";
     
static const char htmlEnd[] PROGMEM =
R"(</body>
</html>)";

static const char htmlIndexPageStart[] PROGMEM =
R"(<br>
<center>
<h3>Config Menu</h3>
<h4>
<a href='/credentials' class='button gray'>Set Credentials</a><br><br>)";

static const char htmlIndexPageCustomButton[] PROGMEM =
R"(<a href='/customsettings' class='button brown'>Custom Settings</a><br><br>)";

static const char htmlIndexPageEnd[] PROGMEM =
R"(
<a href='/firmupdate' class='button blue'>Firmware Update</a><br><br>
<a href='/restart' class='button green'>Reset</a><br><br>
</h4>
</center>)";

static const char htmlCredentialsPage[] PROGMEM =
R"(<br>
<center>
<h3>Enter Credentials</h3>
<h4>
<form action='/save' method='POST'>)";

static const char htmlSsidInput[] PROGMEM =
R"(SSID: <input pattern='.{4,32}' type='text' name='ssid' required title="4 to 32 characters")";

static const char htmlPassInput[] PROGMEM =
R"(Pass: <input pattern='.{8,64}' type='text' name='pass' required title="8 to 64 characters")";

static const char htmlHostInput[] PROGMEM =
R"(Host: <input pattern='.{,16}' type='text' name='host' title="max 16 characters")";

#ifdef add_device_name
static const char htmlDeviceInput[] PROGMEM =
R"(Device: <input pattern='.{,16}' type='text' name='dev' title="max 10 characters")";
#endif

static const char htmlIndexPageCredentialsSaveButton[] PROGMEM =
R"(<input type='submit' name='SUBMIT' value='Save'></form><br><br>)";

#ifdef add_delete_credentials_button
static const char htmlIndexPageDeleteCredentialsButton[] PROGMEM =
R"(<a class='button red' href='/deletecredentials'>Delete Settings</a><br><br>)";
#endif

static const char htmlIndexPageCredentialsBackButton[] PROGMEM =
R"(<a class='button blue' href='/'>Back</a><br><br>)";

static const char htmlCredentialsPageEnd[] PROGMEM =
R"(</h4>
</center>)";

static const char htmlRebootMsg[] PROGMEM =
R"(<html><body><center><h1>Rebooting</h1></center></body></html>)";

#endif  // CONFIG_MANAGER_HTML_H_
