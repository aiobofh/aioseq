#include <assert.h>
#include <string.h>

#include "types.h"
#include "event.h"

#define MAX_EVENT_ITEMS 4096

typedef struct
{
  int items;
  event_type_args_t item[MAX_EVENT_ITEMS];
} event_t;

static bool event_initialized = false;

event_t event;

void event_init()
{
  memset(&event, 0, sizeof(event));
  event_initialized = true;
}

void event_add(event_type_t type, event_type_args_t args)
{
  assert(true == event_initialized);

  args.none.type = type;
  event.item[event.items] = args;

  event.items++;
}

int event_count()
{
  return event.items;
}

void event_clear()
{
  event.items = 0;
}

void event_get(int idx, event_type_args_t** args) {
  assert(0 <= idx);
  assert(idx < event.items);

  *args = &event.item[idx];
}

void event_cleanup()
{
  assert(true == event_initialized);
}
