#ifndef _HELPERS_H_
#define _HELPERS_H_


#endif

/* WANNADO: Make this less quick'n'dirty. */
static inline void rtrim(char* in) {
  int l = strlen(in);
  in[l - 1] = 0;
}
