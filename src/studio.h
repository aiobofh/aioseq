#ifndef _STUDIO_H_
#define _STUDIO_H_

#include "types.h"

void studio_init();
bool studio_load(const char* filename);
bool studio_save(const char* filename);

#endif
