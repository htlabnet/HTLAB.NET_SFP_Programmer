#ifndef LIB_CLI_H
#define LIB_CLI_H

#include <Arduino.h>
#include "settings.h"

typedef struct _command {
  char *command;
  void (*function)();
  char *help;
  struct _command *next;
} command;

void cli_begin(Serial_ &_Stream);
void cli_init();
void cli_task();
void cli_char_handler(char buf);
void cli_cmd_parse(char *line);
void cli_send(char c);
void cli_send(const char str[]);
void cli_send(const String &s);
void cli_send(unsigned char b, int base);
void cli_send_prompt();
void cli_send_return(bool prompt);
void cli_send_unrecog();
void cli_send_hex(uint8_t val);
void cli_command_add(char *cmd_str, void (*function)(), char *help_str);
void cli_help();

#endif
