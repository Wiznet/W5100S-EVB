#ifndef __SERIALCOMMAND_H_
#define __SERIALCOMMAND_H_

int _read(int file, char *ptr, int len);
int _write(int file, char *ptr, int len);
char _getchar_nb();
char _getchar_b();
uint16_t gets_echo(uint8_t* buf, uint16_t maxlen);


#endif
