//
// #####################################################################
//
//        HTLAB.NET SFP Programmer
//          http://htlab.net/
//
//        Copyright (C) 2020
//          Hideto Kikuchi / PJ (@pcjpnet) - http://pc-jp.net/
//          Tsukuba Science Inc. - http://www.tsukuba-kagaku.co.jp/
//
//        !!!!! NOT ALLOWED COMMERCIAL USE !!!!!
//
// #####################################################################
//

//
// ########## Compatible Boards ##########
//    - Seeeduino XIAO
//    - (Arduino MKR ZERO)
//

//
// ########## Pin Assignments ##########
//  A0/D0/DAC     - 
//  A1/D1         - 
//  A2/D2         - 
//  A3/D3         - 
//  A4/D4/SDA     - SFP SDA (Need 4.7k Pull-up)
//  A5/D5/SCL     - SFP SCL (Need 4.7k Pull-up)
//  A6/D6/TX      - 
//  A7/D7/RX      - 
//  A8/D8/SCK     - 
//  A9/D9/MISO    - 
//  A10/D10/MOSI  - 
//


#include "settings.h"
#include "lib_cli.h"
#include "lib_sfp.h"


void setup() {
  delay(1000);
  Serial.begin(CLI_BAUD);
  delay(1000);
  #if CLI_WAIT_OPEN
    while (!Serial);
  #endif
  cli_begin(Serial);

  cli_command_add("version", show_version, "Show Version");
  //cli_command_add("password", sfp_password, "SFP Enter Password");
  //cli_command_add("write", sfp_write, "SFP Write");
  //cli_command_add("read", sfp_read, "SFP Read");
  cli_command_add("status", sfp_status, "SFP Status");
  cli_command_add("dump", sfp_dump, "Dump SFP ROM");
  cli_command_add("detect", sfp_detect, "Detect SFP I2C Bus");

  sfp_begin();
}


void loop() {
  cli_task();
}


void show_version() {
  Serial.print("  Version: ");
  Serial.println("");
}
