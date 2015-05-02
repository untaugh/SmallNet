#ifndef TALKER_H
#define TALKER_H

#define SDA PC4
#define SCL PC5

enum deviceType {
  TPYE_UNKNOWN, 
  TYPE_BUTTON,
  TYPE_LIGHT
};

void strToCmd(uint8_t *str, uint8_t *cmd);

uint8_t strToInt(char *str);

#endif // TALKER_H
