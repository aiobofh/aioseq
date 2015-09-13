#include <stdio.h>
#include <assert.h>

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
    char* r;                                                            \
    r = fgets(m_config_buf, sizeof(m_config_buf), FILE);                \
    assert(NULL != r);                                                  \
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
    char* r;                                                            \
    r = fgets(m_config_buf, sizeof(m_config_buf), FILE);                \
    assert(NULL != r);                                                  \
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
    char* r;                                                            \
    r = fgets(m_config_buf, sizeof(m_config_buf), FILE);                \
    assert(NULL != r);                                                  \
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
    char* r;                                                            \
    r = fgets(m_config_buf, sizeof(m_config_buf), FILE);                \
    assert(NULL != r);                                                  \
    char* p = strstr(m_config_buf, DEFAULT_CONFIG_SEPARATOR);           \
    p += sizeof(DEFAULT_CONFIG_SEPARATOR) - 1;                          \
    p[strlen(p) - 1] = 0;                                               \
    VALUE = atoi(p);                                                    \
  }

#define file_format_args(TYPE) \
  file_t* file, file_mode_t mode, char* prefix, TYPE* data

#define fstr(MEMBER)                                            \
  if (NULL == prefix) {                                         \
    FSTR(file, mode, # MEMBER, data->MEMBER);                   \
  } else {                                                      \
    FSTR_1(file, mode, "%s." # MEMBER, data->MEMBER, prefix);   \
  }

#define fint(MEMBER)                                            \
  if (NULL == prefix) {                                         \
    FINT(file, mode, # MEMBER, data->MEMBER);                   \
  } else {                                                      \
    FINT_1(file, mode, "%s." # MEMBER, data->MEMBER, prefix);   \
  }

#define farray(PLURAL, SINGULAR)                                     \
  fint(PLURAL);                                                      \
  for (int idx = 0; idx < data->PLURAL; idx++) {                     \
    char pfx[128];                                                   \
    if (NULL == prefix) {                                            \
      sprintf(pfx, "%s[%d]", # SINGULAR, idx);                       \
    }                                                                \
    else {                                                           \
      sprintf(pfx, "%s.%s[%d]", prefix, # SINGULAR, idx);            \
    }                                                                \
    SINGULAR ## _file_format(file, mode, pfx, &data->SINGULAR[idx]); \
  }

#define farray_with_pattern_idx(PLURAL, SINGULAR)                    \
  fint(PLURAL);                                                      \
  for (int idx = 0; idx < data->PLURAL; idx++) {                     \
    char pfx[128];                                                   \
    if (NULL == prefix) {                                            \
      sprintf(pfx, "%s[%d]", # SINGULAR, idx);                       \
    }                                                                \
    else {                                                           \
      sprintf(pfx, "%s.%s[%d]", prefix, # SINGULAR, idx);            \
    }                                                                \
    SINGULAR ## _file_format(file, mode, pfx, &data->SINGULAR[idx],  \
                             pattern_idx);                           \
  }

#define farray_generate_pattern_idx(PLURAL, SINGULAR)                \
  fint(PLURAL);                                                      \
  for (int idx = 0; idx < data->PLURAL; idx++) {                     \
    char pfx[128];                                                   \
    if (NULL == prefix) {                                            \
      sprintf(pfx, "%s[%d]", # SINGULAR, idx);                       \
    }                                                                \
    else {                                                           \
      sprintf(pfx, "%s.%s[%d]", prefix, # SINGULAR, idx);            \
    }                                                                \
    SINGULAR ## _file_format(file, mode, pfx, &data->SINGULAR[idx],  \
                             idx);                                   \
  }


#endif
