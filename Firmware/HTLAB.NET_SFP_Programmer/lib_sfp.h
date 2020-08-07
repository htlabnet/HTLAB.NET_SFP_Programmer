#ifndef LIB_SFP_H
#define LIB_SFP_H

#include <Arduino.h>
#include <Wire.h>
#include "settings.h"
#include "lib_sfp_const.h"
#include "lib_cli.h"

void sfp_begin();
void sfp_detect();
void sfp_dump();
void sfp_dump_list(uint8_t device);
uint8_t sfp_dump_byte(uint8_t device, uint8_t addr);
void sfp_status();

#endif
