/**
 * @file aioseq-pattern-editor.cc
 */
#include "project.hh"
#include "sequencer.hh"
#include "pattern_editor.hh"

int MAIN(int argc, char* argv[]) {
  // Change this to use tha socket-based sequencer RPC.
  string name = "New project";
  Sequencer sequencer(ProjectFactory::New(name));
  PatternEditor pattern_editor(&sequencer);
  return pattern_editor.main(argc, argv);
}
