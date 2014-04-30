/**
 * @file aioseq-pattern-editor.cc
 */
#include "project.hh"
#include "sequencer.hh"
#include "pattern_editor.hh"

/**
 * Main program for the aioseq pattern editor.
 *
 * @param argc Command arguments count.
 * @param argv A pointer to an array of character-strings, one for each
 *             argument.
 *
 * @return Program exit status.
 */
int MAIN(int argc, char* argv[]) {
  // Change this to use tha socket-based sequencer RPC.
  string name = "New project";
  Tracks tracks;
  tracks.push_back(TrackFactory::New(1, 1));
  tracks.push_back(TrackFactory::New(1, 1));
  tracks.push_back(TrackFactory::New(1, 1));
  Sequencer sequencer(ProjectFactory::New(name, &tracks));
  sequencer.set_pattern_length(100);
  PatternEditor pattern_editor(&sequencer);
  return pattern_editor.main(argc, argv);
}
