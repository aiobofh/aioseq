#ifndef KEY_MAP_INITIALIZED
#define KEY_MAP_INITIALIZED
#define OCTAVE(OCT) \
  "C-" # OCT, "C#" # OCT, "D-" # OCT, "D#" # OCT, "E-" # OCT, "F-" # OCT, \
  "F#" # OCT, "G-" # OCT, "G#" # OCT, "A-" # OCT, "A#" # OCT, "B-" # OCT
const char* key_map [128] = { "---",
                              OCTAVE(1),
                              OCTAVE(2),
                              OCTAVE(3),
                              OCTAVE(4),
                              OCTAVE(5),
                              OCTAVE(6),
                              OCTAVE(7),
                              OCTAVE(8),
                              OCTAVE(9),
                              OCTAVE(a),
                              "C-b", "C#b", "D-b", "D#b", "E-b", "F-b",
                              "off" };
#undef OCTAVE
#endif
