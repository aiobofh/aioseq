#include <assert.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <signal.h>

#include "defaults.h"
#include "error.h"
#include "helpers.h"
#include "project.h"
#include "studio.h"
#include "timer.h"
#include "editor.h"
#include "midi.h"
#include "event.h"
#include "update.h"

extern bool debug_enabled;

bool m_quit = false;

/*
static char* ask_for_project_filename(char* filename, char *name)
{
  assert(0 == strlen(filename));

  char buf[MAX_FILE_NAME_LENGTH];
  suggest_filename(filename, name, DEFAULT_PROJECT_FILE_EXT);
  fprintf(stdout, "Save the project '%s' as (%s): ", name, filename);
  char* r = fgets(buf, sizeof(buf), stdin);
  assert(NULL != r);

  rtrim(buf);

  if (buf[0] == 0) {
    return filename;
  }

  strcpy(filename, buf);

  return filename;
}
*/
/*
static bool ask_for_overwrite(char* filename, char* name)
{
  char buf[10];
  bool valid_answer = false;
  while (false == valid_answer) {
    fprintf(stdout, "Overwrite '%s'? (yes/no): ", filename);
    char* r = fgets(buf, sizeof(buf), stdin);
    if (NULL == r) {
      continue;
    }

    rtrim(buf);

    if (3 == strlen(buf) || 2 == strlen(buf)) {
      if (0 == strcmp(buf, "yes")) {
        return true;
      }
      else if (0 == strcmp(buf, "no")) {
        return false;
      }
    }
  }
  return false;
}
*/
static void usage(const char* progname)
{
  fprintf(stdout, "USAGE: %s [options] <project>\n"
          "\n"
          " -d, --debug       Enable debuging outputs\n"
          " -h, --help        Show this usage text.\n"
          " -s, --studio=FILE Studio configuration to use. By default\n"
          "                   this file is used first, if not found\n"
          "                   %s is used, if it does not exist\n"
          "                   %s is used as last resort.\n"
          "\n"
          "Report bugs to bug-aioseq@aio.nu\n",
          progname,
          DEFAULT_USER_STUDIO_FILE_NAME,
          DEFAULT_SYSTEM_STUDIO_FILE_NAME);
}

void quit() {
  editor_cleanup();
}

/*
 * Called if the process receives SIGINT (Ctrl+C)
 */
void int_handler(int dummy) {
  m_quit = true; /* Force the main loop to exit and shut down the program */
}

int main(int argc, char* argv[])
{
  int loop_count = 0;

  struct {
    bool debug_enabled;
    bool help;
    char* studio;
    char* project;
  } opts;

  debug_enabled = false;

  memset(&opts, 0, sizeof(opts));

  /*
   * Handle command line options.
   */
  while (1) {
    int c;
    int option_index = 0;
    static struct option long_options[] = {
      {"debug",  no_argument,       0, 'd'},
      {"help",   no_argument,       0, 'h'},
      {"studio", required_argument, 0, 's'},
      {NULL,     0,                 0, 0}
    };

    c = getopt_long(argc, argv, "dhs:", long_options,
                    &option_index);

    if (-1 == c) {
      break;
    }

    switch (c) {
    case 'd':
      opts.debug_enabled = true;
      break;
    case 'h':
      opts.help = true;
      break;
    case 's':
      opts.studio = optarg;
      break;
    }
    option_index++;
  }

  /*
   * Handle user specified command line arguments.
   */
  debug_enabled = opts.debug_enabled;

  editor_init();

  /*
   * Sanity check the user input passed to the command line options.
   */
  if (optind < argc) {
    opts.project = argv[optind];
    debug("project1: %s", opts.project);
    optind++;
    if (optind < argc) {
      while (optind < argc) {
        fprintf(stderr, "Unknown option '%s'\n", argv[optind++]);
      }
      usage(argv[0]);
      return EXIT_FAILURE;
    }
  }

  if (true == opts.help) {
    usage(argv[0]);
    return EXIT_SUCCESS;
  }

  update_init();

  /*
   * Initialize the storage memory.
   */
  midi_init();
  studio_init();
  project_init();


  /*
   * Try to load the user-specified studio file.
   */
  if (NULL != opts.studio) {
    debug("Studio file provided");
    if (false == studio_load(opts.studio)) {
      return EXIT_FAILURE;
    }
  }
  else {
    if (false == studio_load(DEFAULT_USER_STUDIO_FILE_NAME)) {
      if (false == studio_load(DEFAULT_SYSTEM_STUDIO_FILE_NAME)) {
        studio_load(NULL);
      }
    }
  }

  /*
   * Try to load a project.
   */
  if (false == project_load(opts.project)) {
    return false;
  }

  editor_set_quantization(0);

  project_reset();
  /*
  editor_refresh_song_idx();
  editor_refresh_part_idx();
  editor_refresh_pattern_idx();
  editor_refresh_tempo();
  editor_refresh_status();
  editor_refresh_pattern();
  */
  editor_refresh_windows();

  timer_setup();

  signal(SIGINT, int_handler);

  event_init();

  /*
   * Timed main loop, this is where the magic happens.
   */
  while (m_quit == false) {
    timer_wait();
    update_commit();
    editor_read_kbd();
    midi_poll_events();
    project_update();
    if (0 == loop_count && project_get_project_mode() != PROJECT_MODE_STOPPED) {
      project_step();
    }
    loop_count++;
    if (loop_count > 255 / project_get_tempo()) {
      loop_count = 0;
    }
    editor_refresh_windows();
  }

  event_cleanup();
  timer_cleanup();
  midi_cleanup();

  project_save(NULL, editor_ask_for_project_filename, editor_ask_for_overwrite);
  studio_save(NULL);

  /*
   * Move this to after project and studio save when GUI requesters for
   * overwrite and filname are done.
   */
  editor_cleanup();

  return 0;
}
