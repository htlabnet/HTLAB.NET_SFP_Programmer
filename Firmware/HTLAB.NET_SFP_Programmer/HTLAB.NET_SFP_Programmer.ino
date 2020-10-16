//
// #####################################################################
//
//        HTLAB.NET SFP Programmer
//          https://htlab.net/computer/network/arduino-sfp-programmer/
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
//    - Arduino MKR ZERO
//    - (Arduino Due)
//

//
// ########## Pin Assignments (Seeeduino XIAO) ##########
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

//
// ########## Pin Assignments (Arduino MKR ZERO) ##########
//  D0/PA22       - 
//  D1/PA23       - 
//  D2/PA10       - 
//  D3/PA11       - 
//  D4/PB10       - 
//  D5/PB11       - 
//  D6/PA20       - 
//  D7/PA21       - 
//  D8/PA16/MOSI  - 
//  D9/PA17/SCK   - 
//  D10/PA19/MISO - 
//  D11/PA08/SDA  - SFP SDA (Need 4.7k Pull-up)
//  D12/PA09/SCL  - SFP SCL (Need 4.7k Pull-up)
//  D13/PB23/RX   - 
//  D14/PB22/TX   - 
//  D15/A0/PA02   - 
//  D16/A1/PB02   - 
//  D17/A2/PB03   - 
//  D18/A3/PA04   - 
//  D19/A4/PA05   - 
//  D20/A5/PA06   - 
//  D21/A6/PA07   - 
//


#include "settings.h"
#include "lib_cli.h"
#include "lib_sfp.h"
#include "version.h"

const char VERSION[] = VER_BRANCH_STR "-" VER_RUN_NUM_STR "-" VER_RUN_ID_STR " @ " 
                       "20" VER_YEAR_STR "/" VER_MON_STR "/" VER_DATE_STR "-" 
                       VER_HOUR_STR ":" VER_MIN_STR ":" VER_SEC_STR;


void setup() {
  delay(1000);
  Serial.begin(CLI_BAUD);
  delay(1000);
  #if CLI_WAIT_OPEN
    while (!Serial);
  #endif
  cli_begin(Serial);

  cli_command_add("writetest", sfp_write_test, "Test SFP ROM Write & Verify");
  //cli_command_add("writebyte", sfp_write, "SFP Write 1Byte");
  cli_command_add("version", show_version, "Show Version");
  cli_command_add("status", sfp_status, "SFP Status");
  //cli_command_add("readbyte", sfp_read, "SFP Read 1Byte");
  cli_command_add("password", sfp_write_password_input, "Input Password");
  cli_command_add("dump", sfp_dump, "Dump SFP ROM");
  cli_command_add("detect", sfp_detect, "Detect SFP I2C Bus");
  cli_command_add("clocktest", sfp_clock_test, "Test SFP I2C Bus Clock Speed");
  cli_command_add("clockset", sfp_clock_set, "Set SFP I2C Bus Clock Speed");
  cli_command_add("clock", sfp_clock, "Show SFP I2C Bus Clock Speed");
  //cli_command_add("attackrev", sfp_attack_rev, "SFP Password Brute Force Attack Reverse");
  cli_command_add("attack", sfp_attack, "SFP Password Brute Force Attack");

  sfp_begin();
  show_version();
  cli_send_return(true);
}


void loop() {
  cli_task();
}


void show_version() {
  Serial.println("######################################################################");
  Serial.println("");
  Serial.println("      H T L A B . N E T    S F P    P r o g r a m m e r");
  Serial.println("");
  Serial.println("      https://htlab.net/computer/network/arduino-sfp-programmer/");
  Serial.println("");
  Serial.println("      Copyright (C) 2020");
  Serial.println("        Hideto Kikuchi / PJ (@pcjpnet) - http://pc-jp.net/");
  Serial.println("        Tsukuba Science Inc. - http://www.tsukuba-kagaku.co.jp/");
  Serial.println("");
  Serial.print("      F/W Version: ");
  Serial.print(VERSION);
  Serial.println("");
  Serial.println("######################################################################");
  Serial.flush();
  delay(10);
}
