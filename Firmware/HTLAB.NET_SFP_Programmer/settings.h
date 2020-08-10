#ifndef SETTINGS_H
#define SETTINGS_H


// ########## Settings ##########

#define CLI_BAUD 9600
#define CLI_WAIT_OPEN true
#define CLI_BUFFER_SIZE 100
#define CLI_PROMPT ">"
#define CLI_UNRECOG "% Unrecognized command"
#define CLI_LINE_FEED "\r\n"

#define SFP_INTERNAL_PULLUP true
#define SFP_I2C_CLOCK_DEFAULT 400000
#define SFP_I2C_CLOCK_MIN 100000
#define SFP_I2C_CLOCK_MAX 3400000
#define SFP_CLOCK_TEST_INCREMENT 100000
#define SFP_CLOCK_TEST_DEVICE 0x50
#define SFP_CLOCK_TEST_ADDR 20
#define SFP_PASSWORD_TEST_DEVICE 0x50
#define SFP_PASSWORD_TEST_ADDR 0
#define SFP_ATTACK_SHOW_INFO_COUNT 10000

// ########## Settings ##########


#endif
