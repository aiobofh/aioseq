#ifndef _EVENT_H_
#define _EVENT_H_

typedef enum
{
  EVENT_TYPE_NOTE_ON,
  EVENT_TYPE_NOTE_OFF,
  EVENT_TYPE_CONTROLLER
} event_type_t;

typedef struct
{
  event_type_t type;
  int output;
} event_type_none_t;

typedef struct
{
  event_type_t type;
  int output;
  unsigned char note;
  unsigned char velocity;
  unsigned char channel;
} event_type_note_on_t;

typedef struct
{
  event_type_t type;
  int output;
  unsigned char note;
  unsigned char velocity;
  unsigned char channel;
} event_type_note_off_t;

typedef struct
{
  event_type_t type;
  int output;
  unsigned char parameter;
  unsigned char value;
  unsigned char channel;
} event_type_controller_t;

typedef union {
  event_type_none_t none;
  event_type_note_on_t event_type_note_on;
  event_type_note_off_t event_type_note_off;
  event_type_controller_t event_type_controller;
} event_type_args_t;

void event_init();
void event_add(event_type_t type, event_type_args_t args);
int event_count();
void event_clear();
void event_get(int idx, event_type_args_t** args);
void event_cleanup();

#endif
