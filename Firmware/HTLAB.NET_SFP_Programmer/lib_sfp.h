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
uint8_t sfp_read_byte(uint8_t device, uint8_t addr);
uint8_t sfp_write_byte(uint8_t device, uint8_t addr, uint8_t data);
void sfp_clock();
void sfp_clock_set();
void sfp_clock_test();
void sfp_status();

#endif
