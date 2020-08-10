#include "lib_cli.h"

Serial_ *IO_Stream;

uint8_t line[CLI_BUFFER_SIZE];
uint8_t *line_ptr;
uint8_t *cursor_ptr;
static command *cmd, *cmd_list;
bool input_mode;
uint8_t input_buffer[CLI_BUFFER_SIZE];
bool break_mode;

void cli_begin(Serial_ &_Serial) {
  IO_Stream = &_Serial;
  line_ptr = line;
  *line_ptr = '\0';
  cmd_list = NULL;
  input_mode = false;
  break_mode = false;
  cli_command_add("help", cli_help, "Show Help");
  cli_init();
}


void cli_init() {
  // VT Reset
  cli_send("\ec");
  IO_Stream->flush();
  delay(10);
}


void cli_task() {
  if (IO_Stream->available() > 0) {
    uint8_t buf = IO_Stream->read();
    switch (buf) {

      case 0x0D:  // CR
      case 0x0A:  // LF
        *line_ptr = '\0';
        cli_cmd_parse((char *)line);
        cli_send_return(!input_mode);
        line_ptr = line;
        cursor_ptr = line;
        *line_ptr = '\0';
        input_mode = false;

        if (IO_Stream->available() >= 1) {
          buf = IO_Stream->read();
          if (buf != '\n') {
            cli_char_handler(buf);
          }
        }
        break;

      case 0x08:  // BS
      case 0x7F:  // DEL
        if (line_ptr > line) {
          cli_send("\b \b");
          line_ptr--;
          cursor_ptr--;
        } else {
          cli_send(0x07);
        }
        break;

      case 0x09:  // HT (Tab)
        if (*line != '\0') {
          
        } else {
          cli_send(0x07);
          cli_send_return(true);
        }
        break;

      case 0x1B:  // ESC
        if (IO_Stream->available() >= 2) {
          if (IO_Stream->read() == 0x5B) {
            switch (IO_Stream->read()) {
              case 0x41:  // UP
              case 0x42:  // DOWN
                if (!input_mode) {
                  cli_send(0x07);
                }
                break;
              case 0x43:  // RIGHT
                if (cursor_ptr < line_ptr) {
                  cli_send(*cursor_ptr);
                  cursor_ptr++;
                } else {
                  cli_send(0x07);
                }
                break;
              case 0x44:  // LEFT
                if (cursor_ptr > line) {
                  cli_send(0x08);
                  cursor_ptr--;
                } else {
                  cli_send(0x07);
                }
                break;
            }
          }
        }
        break;

      case 0x3F:  // ?
        if (!input_mode) {
          cli_send(buf);
          cli_send_return(false);
          cli_help();
          cli_send_return(true);
        } else {
          cli_char_handler(buf);
        }
        break;

      case 0x03:  // ETX (Ctrl + C)
        input_mode = false;
        break_mode = true;
        break;

      case 0x1A:  // SUB (Ctrl + Z)
        input_mode = false;
        break_mode = true;
        break;

      default:
        cli_char_handler(buf);
        break;
    }
  }
}


char* cli_input_handler() {
  input_mode = true;
  line_ptr = line;
  cursor_ptr = line;
  *line_ptr = '\0';
  while(true) {
    cli_task();
    if (!input_mode) break;
  }
  return (char *)input_buffer;
}


int cli_input_int_handler() {
  char *val;
  val = cli_input_handler();
  return atoi(val);
}


unsigned long cli_input_ulong_handler() {
  char *val;
  val = cli_input_handler();
  return strtoul(val, NULL, 0);
}


void cli_char_handler(char buf) {
  cli_send(buf);
  *line_ptr++ = buf;
  cursor_ptr++;
}


bool cli_break_handler() {
  return break_mode;
}


void cli_break_clear() {
  break_mode = false;
}


void cli_cmd_parse(char *line) {
  if (input_mode) {
    strcpy((char *)input_buffer, line);
    return;
  }
  if (*line == '\0') {
    return;
  }
  cli_send_return(false);
  command *cmd_current;
  for (cmd_current = cmd; cmd_current != NULL; cmd_current = cmd_current->next) {
    if (strcmp(line, cmd_current->command) == 0){
      cmd_current->function();
      return;
    }
  }
  cli_send_unrecog();
}


void cli_send(char c) {
  IO_Stream->write(c);
}


void cli_send(const char str[]) {
  IO_Stream->write(str);
}


void cli_send(const String &s) {
  IO_Stream->write(s.c_str(), s.length());
}

void cli_send(unsigned char b, int base) {
  IO_Stream->print((unsigned long) b, base);
}


void cli_send_prompt() {
  cli_send(CLI_PROMPT);
}


void cli_send_return(bool prompt) {
  cli_send(CLI_LINE_FEED);
  if (prompt) cli_send_prompt();
}


void cli_send_unrecog() {
  cli_send(CLI_UNRECOG);
}


void cli_send_hex(uint8_t val) {
  cli_send((val >> 4), HEX);
  cli_send((val & 0x0F), HEX);
}


void cli_command_add(char *cmd_str, void (*function)(), char *help_str) {
    cmd = (command *)malloc(sizeof(command));

    char *cmd_name = (char *)malloc(strlen(cmd_str) + 1);
    strcpy(cmd_name, cmd_str);
    cmd_name[strlen(cmd_str)] = '\0';

    char *help = (char *)malloc(strlen(help_str) + 1);
    strcpy(help, help_str);
    help[strlen(help_str)] = '\0';

    cmd->command = cmd_name;
    cmd->function = function;
    cmd->help = help;
    cmd->next = cmd_list;
    cmd_list = cmd;
}


void cli_help() {
  cli_send("Exec commands:");
  cli_send_return(false);
  command *cmd_current;
  for (cmd_current = cmd; cmd_current != NULL; cmd_current = cmd_current->next) {
    cli_send("    ");
    cli_send(cmd_current->command);
    cli_send("\t\t");
    cli_send(cmd_current->help);
    cli_send_return(false);
  }
}
