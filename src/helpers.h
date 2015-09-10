#ifndef _HELPERS_H_
#define _HELPERS_H_

#define wrap(a, b) ((a > b) ? ((a > (b + 1)) ? b : 0) : a)
#define rtrim(in)                               \
  {                                             \
    int l = strlen(in);                         \
    in[l - 1] = 0;                              \
  }

#endif
