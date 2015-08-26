#include <stdio.h>

extern int m_line_counter;
extern char m_config_buf[10000];

#ifndef _CONFIG_H_
#define _CONFIG_H_

int m_line_counter;
char m_config_buf[10000];

typedef enum {
  MODE_WRITE,
  MODE_READ
} file_mode_t;

#define FSTR(FILE, MODE, FMT, VALUE)                                    \
  m_line_counter++;                                                     \
  if (MODE_WRITE == MODE) {                                             \
    fprintf(FILE, FMT DEFAULT_CONFIG_SEPARATOR "%s\n", VALUE);          \
  }                                                                     \
  else {                                                                \
    fgets(m_config_buf, sizeof(m_config_buf), FILE);                    \
    char* p = strstr(m_config_buf, DEFAULT_CONFIG_SEPARATOR);           \
    p += sizeof(DEFAULT_CONFIG_SEPARATOR) - 1;                          \
    p[strlen(p) - 1] = 0;                                               \
    strcpy(VALUE, p);                                                   \
  }

#define FINT(FILE, MODE, FMT, VALUE)                                    \
  m_line_counter++;                                                     \
  if (MODE_WRITE == MODE) {                                             \
    fprintf(FILE, FMT DEFAULT_CONFIG_SEPARATOR "%d\n", VALUE);          \
  }                                                                     \
  else {                                                                \
    fgets(m_config_buf, sizeof(m_config_buf), FILE);                    \
    char* p = strstr(m_config_buf, DEFAULT_CONFIG_SEPARATOR);           \
    p += sizeof(DEFAULT_CONFIG_SEPARATOR) - 1;                          \
    p[strlen(p) - 1] = 0;                                               \
    VALUE = atoi(p);                                                    \
  }

#define FSTR_1(FILE, MODE, FMT, VALUE, ARG)                             \
  m_line_counter++;                                                     \
  if (MODE_WRITE == MODE) {                                             \
    fprintf(FILE, FMT DEFAULT_CONFIG_SEPARATOR "%s\n", ARG, VALUE);     \
  }                                                                     \
  else {                                                                \
    fgets(m_config_buf, sizeof(m_config_buf), FILE);                    \
    char* p = strstr(m_config_buf, DEFAULT_CONFIG_SEPARATOR);           \
    p += sizeof(DEFAULT_CONFIG_SEPARATOR) - 1;                          \
    p[strlen(p) - 1] = 0;                                               \
    strcpy(VALUE, p);                                                   \
  }

#define FINT_1(FILE, MODE, FMT, VALUE, ARG)                             \
  m_line_counter++;                                                     \
  if (MODE_WRITE == MODE) {                                             \
    fprintf(FILE, FMT DEFAULT_CONFIG_SEPARATOR "%d\n", ARG, VALUE);     \
  }                                                                     \
  else {                                                                \
    fgets(m_config_buf, sizeof(m_config_buf), FILE);                    \
    char* p = strstr(m_config_buf, DEFAULT_CONFIG_SEPARATOR);           \
    p += sizeof(DEFAULT_CONFIG_SEPARATOR) - 1;                          \
    p[strlen(p) - 1] = 0;                                               \
    VALUE = atoi(p);                                                    \
  }

#endif
