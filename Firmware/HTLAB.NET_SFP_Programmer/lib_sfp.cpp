#include "lib_sfp.h"

void sfp_begin() {
  Wire.begin();
  Wire.setClock(SFP_DEFAULT_I2C_CLOCK);
  #if !SFP_INTERNAL_PULLUP
    pinMode(SDA, INPUT);
    pinMode(SCL, INPUT);
  #endif
}


void sfp_detect() {
  cli_send("Detect");
  cli_send_return(false);
  for(uint8_t device = 1; device < 127; device++){
    Wire.beginTransmission(device);
    if (Wire.endTransmission() == 0) {
      cli_send("  SFP I2C found at address 0x");
      cli_send_hex(device);
      cli_send(" ...!");
      cli_send_return(false);
    }
  }
  cli_send("Done");
  cli_send_return(false);
}


void sfp_dump() {
  cli_send("Dump");
  cli_send_return(false);
  cli_send_return(false);
  for(uint8_t device = 1; device < 127; device++){
    Wire.beginTransmission(device);
    if (Wire.endTransmission() == 0) {
      sfp_dump_list(device);
    }
  }
  cli_send("Done");
  cli_send_return(false);
}


void sfp_dump_list(uint8_t device) {
  cli_send("  0x");
  cli_send_hex(device);
  cli_send("/  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
  cli_send_return(false);
  for(uint16_t i = 0; i <= 0xFF; i++){
    if((i & 0x0F) == 0x00){
      cli_send("  0x");
      cli_send_hex(i);
      cli_send(":");
    }
    cli_send(" ");
    cli_send_hex(sfp_dump_byte(device, i));
    if((i & 0x0F) == 0x0F){
      cli_send_return(false);
    }
  }
  cli_send_return(false);
}


uint8_t sfp_dump_byte(uint8_t device, uint8_t addr) {
  Wire.beginTransmission(device);
  Wire.write(addr);
  Wire.endTransmission();
  Wire.requestFrom(device, 1);
  return Wire.read();
}


void sfp_status() {
  cli_send("Status");
  cli_send_return(false);

  Wire.beginTransmission(0x50);
  if (Wire.endTransmission() != 0) return;

  // DATA FIELDS - ADDRESS A0H
  cli_send("DATA FIELDS - ADDRESS A0H (I2C ADDRESS 50H)");
  cli_send_return(false);

  // BASE ID FIELDS
  cli_send("BASE ID FIELDS");
  cli_send_return(false);

  // Identifier [0] 1Byte
  cli_send("  Identifier:\t\t0x");
  cli_send_hex(sfp_dump_byte(0x50, 0));
  cli_send(" -> ");
  cli_send(SFP_IDENTIFIER[sfp_dump_byte(0x50, 0)]);
  cli_send_return(false);

  // Ext. Identifier [1] 1Byte
  cli_send("  Ext. Identifier:\t0x");
  cli_send_hex(sfp_dump_byte(0x50, 1));
  if (sfp_dump_byte(0x50, 1) == 0x04) {
    cli_send_return(false);
    cli_send("\t");
  }
  cli_send(" -> ");
  cli_send(SFP_EXTENDED_IDENTIFIER[sfp_dump_byte(0x50, 1)]);
  cli_send_return(false);

  // Connector [2] 1Byte
  cli_send("  Connector:\t\t0x");
  cli_send_hex(sfp_dump_byte(0x50, 2));
  cli_send(" -> ");
  cli_send(SFP_CONNECTOR[sfp_dump_byte(0x50, 2)]);
  cli_send_return(false);

  // Transceiver [3-10] 8Byte
  cli_send("  Transceiver:\t\t");
  uint64_t code = 0;
  for (uint8_t i = 3; i <= 10; i++) {
    cli_send("0x");
    cli_send_hex(sfp_dump_byte(0x50, i));
    code = code << 8;
    code |= (uint64_t)sfp_dump_byte(0x50, i);
    cli_send(" ");
  }
  cli_send_return(false);
  for (uint8_t i = 0; i < 64; i++) {
    if ((code >> (63 - i)) & 0b01) {
      cli_send("\t  * ");
      cli_send(SFP_COMPLIANCE_CODE[i]);
      cli_send_return(false);
    }
  }

  // Encoding [11] 1Byte
  cli_send("  Encoding:\t\t0x");
  cli_send_hex(sfp_dump_byte(0x50, 11));
  cli_send(" -> ");
  cli_send(SFP_ENCODING[sfp_dump_byte(0x50, 11)]);
  cli_send_return(false);

  // BR, Nominal [12] 1Byte
  cli_send("  BR, Nominal:\t\t0x");
  cli_send_hex(sfp_dump_byte(0x50, 12));
  cli_send(" -> ");
  cli_send(String(sfp_dump_byte(0x50, 12) * 100));
  cli_send(" MBd");
  cli_send_return(false);

  // Rate Identifier [13] 1Byte
  cli_send("  Rate Identifier:\t0x");
  cli_send_hex(sfp_dump_byte(0x50, 13));
  cli_send(" -> ");
  cli_send(SFP_RATE_IDENTIFIER[sfp_dump_byte(0x50, 13)]);
  cli_send_return(false);

  // Length(SMF,km) [14] 1Byte
  cli_send("  Length(SMF,km):\t0x");
  cli_send_hex(sfp_dump_byte(0x50, 14));
  cli_send(" -> ");
  cli_send(String(sfp_dump_byte(0x50, 14)));
  cli_send(" km");
  cli_send_return(false);

  // Length (SMF) [15] 1Byte
  cli_send("  Length (SMF):\t\t0x");
  cli_send_hex(sfp_dump_byte(0x50, 15));
  cli_send(" -> ");
  cli_send(String(sfp_dump_byte(0x50, 15) * 100));
  cli_send(" m");
  cli_send_return(false);

  // Length (50um) [16] 1Byte
  cli_send("  Length (50um):\t0x");
  cli_send_hex(sfp_dump_byte(0x50, 16));
  cli_send(" -> ");
  cli_send(String(sfp_dump_byte(0x50, 16) * 10));
  cli_send(" m");
  cli_send_return(false);

  // Length (62.5um) [17] 1Byte
  cli_send("  Length (62.5um):\t0x");
  cli_send_hex(sfp_dump_byte(0x50, 17));
  cli_send(" -> ");
  cli_send(String(sfp_dump_byte(0x50, 17) * 10));
  cli_send(" m");
  cli_send_return(false);

  // Length (OM4 or copper cable) [18] 1Byte
  cli_send("  Length (OM4):\t\t0x");
  cli_send_hex(sfp_dump_byte(0x50, 18));
  cli_send(" -> ");
  cli_send(String(sfp_dump_byte(0x50, 18) * 10));
  cli_send(" m");
  cli_send_return(false);

  cli_send("  Length (Copper):\t0x");
  cli_send_hex(sfp_dump_byte(0x50, 18));
  cli_send(" -> ");
  cli_send(String(sfp_dump_byte(0x50, 18)));
  cli_send(" m");
  cli_send_return(false);

  // Length (OM3) [19] 1Byte
  cli_send("  Length (OM3):\t\t0x");
  cli_send_hex(sfp_dump_byte(0x50, 19));
  cli_send(" -> ");
  cli_send(String(sfp_dump_byte(0x50, 19) * 10));
  cli_send(" m");
  cli_send_return(false);

  // Vendor name [20-35] 16Byte
  cli_send("  Vendor name:\t\t");
  for (uint8_t i = 20; i <= 35; i++) {
    cli_send(sfp_dump_byte(0x50, i));
  }
  cli_send_return(false);

  // Ext. Transceiver [36] 1Byte
  cli_send("  Ext. Identifier:\t0x");
  cli_send_hex(sfp_dump_byte(0x50, 36));
  cli_send(" -> ");
  cli_send(SFP_EXTENDED_COMPLIANCE_CODE[sfp_dump_byte(0x50, 36)]);
  cli_send_return(false);

  // Vendor OUI [37-39] 3Byte
  cli_send("  Vendor OUI:\t\t");
  for (uint8_t i = 37; i <= 39; i++) {
    cli_send_hex(sfp_dump_byte(0x50, i));
  }
  cli_send_return(false);

  // Vendor PN [40-55] 16Byte
  cli_send("  Vendor PN:\t\t");
  for (uint8_t i = 40; i <= 55; i++) {
    cli_send(sfp_dump_byte(0x50, i));
  }
  cli_send_return(false);

  // Vendor rev [56-59] 4Byte
  cli_send("  Vendor rev:\t\t");
  for (uint8_t i = 56; i <= 59; i++) {
    cli_send(sfp_dump_byte(0x50, i));
  }
  cli_send_return(false);

  // Wavelength [60-61] 2Byte
  cli_send("  Wavelength:\t\t");
  uint16_t wavelength = 0;
  wavelength = (uint16_t)sfp_dump_byte(0x50, 60);
  wavelength = wavelength << 8;
  wavelength |= (uint16_t)sfp_dump_byte(0x50, 61);
  cli_send(String(wavelength));
  cli_send(" nm");
  cli_send_return(false);

  // Unallocated [62] 1Byte

  // CC_BASE [63] 1Byte
  cli_send("  CC_BASE:\t\t0x");
  cli_send_hex(sfp_dump_byte(0x50, 63));
  cli_send(" -> ");
  uint16_t checksum = 0;
  for (uint8_t i = 0; i <= 62; i++) {
    checksum += sfp_dump_byte(0x50, i);
  }
  if (sfp_dump_byte(0x50, 63) == (checksum & 0xFF)) {
    cli_send("OK [0-62 Byte Data is Valid]");
  } else {
    cli_send("NG [0-62 Byte Data is NOT Valid]");
  }
  cli_send_return(false);

  // EXTENDED ID FIELDS
  cli_send("EXTENDED ID FIELDS");
  cli_send_return(false);

  // Options [64-65] 2Byte
  cli_send("  Options:\t\t");
  uint16_t option = 0;
  for (uint8_t i = 64; i <= 65; i++) {
    cli_send("0x");
    cli_send_hex(sfp_dump_byte(0x50, i));
    option = option << 8;
    option |= (uint16_t)sfp_dump_byte(0x50, i);
    cli_send(" ");
  }
  cli_send_return(false);
  for (uint8_t i = 0; i < 16; i++) {
    if ((option >> (15 - i)) & 0b01) {
      cli_send("\t  * ");
      cli_send(SFP_OPTION[i]);
      cli_send_return(false);
    }
  }

  // BR, max [66] 1Byte
  cli_send("  BR, max:\t\t0x");
  cli_send_hex(sfp_dump_byte(0x50, 66));
  cli_send(" -> ");
  cli_send(String(sfp_dump_byte(0x50, 66) ));
  cli_send(" %");
  cli_send_return(false);

  // BR, min [67] 1Byte
  cli_send("  BR, min:\t\t0x");
  cli_send_hex(sfp_dump_byte(0x50, 67));
  cli_send(" -> ");
  cli_send(String(sfp_dump_byte(0x50, 67) ));
  cli_send(" %");
  cli_send_return(false);

  // Vendor SN [68-83] 16Byte
  cli_send("  Vendor SN:\t\t");
  for (uint8_t i = 68; i <= 83; i++) {
    cli_send(sfp_dump_byte(0x50, i));
  }
  cli_send_return(false);

  // Date code [84-91] 8Byte
  cli_send("  Date code:\t\t20");
  for (uint8_t i = 84; i <= 91; i++) {
    cli_send(sfp_dump_byte(0x50, i));
    if (i % 2 == 1 && i < 89) {
      cli_send("/");
    } else if (i == 89) {
      cli_send(" : ");
    }
  }
  cli_send_return(false);

  // Diagnostic Monitoring Type [92] 1Byte
  cli_send("  Diagnostic:\t\t0x");
  cli_send_hex(sfp_dump_byte(0x50, 92));
  cli_send_return(false);

  // Enhanced Options [93] 1Byte
  cli_send("  Enhanced Options:\t0x");
  cli_send_hex(sfp_dump_byte(0x50, 93));
  cli_send_return(false);

  // SFF-8472 Compliance [94] 1Byte
  cli_send("  Compliance:\t\t0x");
  cli_send_hex(sfp_dump_byte(0x50, 94));
  cli_send_return(false);

  // CC_EXT [95] 1Byte
  cli_send("  CC_EXT:\t\t0x");
  cli_send_hex(sfp_dump_byte(0x50, 95));
  cli_send(" -> ");
  checksum = 0;
  for (uint8_t i = 64; i <= 94; i++) {
    checksum += sfp_dump_byte(0x50, i);
  }
  if (sfp_dump_byte(0x50, 95) == (checksum & 0xFF)) {
    cli_send("OK [64-94 Byte Data is Valid]");
  } else {
    cli_send("NG [64-94 Byte Data is NOT Valid]");
  }
  cli_send_return(false);

  // VENDOR SPECIFIC ID FIELDS
  cli_send("VENDOR SPECIFIC ID FIELDS");
  cli_send_return(false);

  // Vendor Specific [96-127] 32Byte
  // Reserved [128-255] 128Byte
  // Reserved for SFF-8079

  Wire.beginTransmission(0x51);
  if (Wire.endTransmission() != 0) return;

  // DATA FIELDS - ADDRESS A2H
  cli_send("DATA FIELDS - ADDRESS A2H (I2C ADDRESS 51H)");
  cli_send_return(false);

  // DIAGNOSTIC AND CONTROL/STATUS FIELDS
  cli_send("DIAGNOSTIC AND CONTROL/STATUS FIELDS");
  cli_send_return(false);

  // A/W Thresholds [0-39] 40Byte
  // Optional A/W Thresholds [40-55] 16Byte
  // Ext Cal Constants [56-91] 36Byte
  // Unallocated [92-94] 3Byte
  // CC_DMI [95] 1Byte
  // Diagnostics [96-105] 10Byte
  // Optional Diagnostics [106-109] 4Byte
  // Status/Control [110] 1Byte
  // Reserved [111] 1Byte
    // Reserved for SFF-8079
  // Alarm Flags [112-113] 2Byte
  // Unallocated [114] 1Byte
  // CDR Unlocked [115] 1Byte
  // Warning Flags [116-117] 2Byte
  // Ext Status/Control [118-119] 2Byte

  // GENERAL USE FIELDS
  cli_send("GENERAL USE FIELDS");
  cli_send_return(false);

  // Vendor Specific [120-126] 7Byte
  // Table Select [127] 1Byte





}