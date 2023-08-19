#include "virtual_io.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <stdlib.h>  // exit()
#include <sys/types.h>  // mkdir()
#include <sys/stat.h>  // mkdir()
#include <errno.h>

static bool interactive = false;
static bool test_function = false;
static std::istream* input = NULL;
static std::ostream* usbstream = NULL;
static std::ostream* ledstream = NULL;
static unsigned cycle = 0;

bool isInteractive(void) {
  return interactive;
}

bool testFunctionExecutionRequested() {
  return test_function;
}

unsigned currentCycle(void) {
  return cycle;
}
void nextCycle(void) {
  cycle++;
}

void logUSBEvent(std::string descrip, void* data, int length) {
  if (usbstream) {
    *usbstream << "Cycle " << std::dec << currentCycle() << ": " << descrip << ": 0x" << std::hex;
    unsigned char* report = (unsigned char*) data;
    for (int i = 0; i < length; i++) *usbstream << std::setfill('0') << std::setw(2) << (unsigned int)(report[i]); // pad with 0's to total of 2 characters
    *usbstream << std::endl;
  }
}

void logUSBEvent_keyboard(std::string descrip) {
  if (usbstream) {
    *usbstream << "Cycle " << std::dec << currentCycle() << ": " << descrip << std::endl;
  }
}

void logLEDStates(std::string descrip) {
  if (ledstream) {
    *ledstream << "Cycle " << std::dec << currentCycle() << ": " << descrip << std::endl;
  }
}

bool initVirtualInput(int argc, char* argv[]) {
  if (argc < 2 || strcmp(argv[1], "?") == 0) {
    printHelp();
    return false;
  }

  if (strcmp(argv[1], "-i") == 0) {
    interactive = true;
    input = &std::cin;
  } else if (strcmp(argv[1], "-t") == 0) {
    test_function = true;
  } else {
    interactive = false;
    input = new std::ifstream(argv[1]);
    if (!input || !(*input)) {
      std::cerr << "Error opening input file \"" << argv[1] << "\"" << std::endl;
      return false;
    }
  }

  if (argc > 2 && strcmp(argv[2], "-q") != 0) {
    std::cerr << "Error: ignoring unknown argument '" << argv[2] << "'" << std::endl;
    if (mkdir("results", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) && errno != EEXIST) {
      std::cerr << "Error creating directory 'results', errno " << errno << std::endl;
      return false;
    }
    usbstream = new std::ofstream("results/USB.txt");
    ledstream = new std::ofstream("results/LED.txt");
  }

  return true;
}

std::string getLineOfInput(bool anythingHeld) {
  if (interactive) {
    std::cout << "Enter a command for this scan cycle, or ? or 'help' for help." << std::endl;
    if (anythingHeld) std::cout << "+> ";
    else std::cout << "> ";
  }
  std::string line;
  std::getline(*input, line);
  if (!interactive && !(*input)) exit(0); // reached EOF or other file error
  return line;
}

void printHelp(void) {
  std::cout << "\nUsage:\n" << std::endl;
  std::cout << "(Running with no arguments or with the argument '?' will print this help message and quit.)\n" << std::endl;
  std::cout << "This program expects either one or two arguments, which must be one of:" << std::endl;
  std::cout << "  1. An input file/script, with format given below, or" << std::endl;
  std::cout << "  2. \"-i\", to run interactively, where you can interactively enter commands and see results." << std::endl;
  std::cout << "  3. \"-t\", to run a test function that is specified in the sketch file." << std::endl;
  std::cout << "  4. \"-t -q\", to run tests quietly (suppressing `results/` output streams)." << std::endl;
  std::cout << "\nIn either case, for each scan cycle you will specify zero or more input 'commands', that is," << std::endl;
  std::cout << "  actions to take on the keys of the virtual keyboard.  Each line of the input file, or each" << std::endl;
  std::cout << "  prompt (in interactive mode), represents one scan cycle; a blank line or empty prompt means" << std::endl;
  std::cout << "  to do nothing to the inputs this scan cycle (held keys will still remain held, though)." << std::endl;
  std::cout << "\nOutput, in terms of HID reports (packets sent to the host computer, for real hardware), is" << std::endl;
  std::cout << "  printed to stdout as it happens, in summarized/human-readable form.  Raw HID output and" << std::endl;
  std::cout << "  serial output (through the 'Serial' object) are collected and redirected to various files" << std::endl;
  std::cout << "  in a subdirectory \"results\" of the current directory." << std::endl;
  std::cout << "\nSerial input is currently unsupported - sketches requesting it will still build, but will" << std::endl;
  std::cout << "  find nothing is ever transmitted to them on the serial port." << std::endl;
  std::cout << "\n--- Commands ---" << std::endl;
  std::cout << "\n1. BASICS\n" << std::endl;
  std::cout << "In any given scan cycle, you can 'tap' a virtual key simply by entering its name." << std::endl;
  std::cout << "To 'tap' multiple keys in one cycle, enter each of their names separated by a space." << std::endl;
  std::cout << "Keys can be identified either by their (row,col) coordinate, or by their \"physical\" names." << std::endl;
  std::cout << "Keys' coordinate names are simply of the form (row,col).  E.g. (0,1) or (2,10) or (1,0)." << std::endl;
  std::cout << "  (Don't put any extra whitespace inside the coordinate name.)" << std::endl;
  std::cout << "A key's \"physical\" name is the (unshifted) text printed on the key on the standard QWERTY" << std::endl;
  std::cout << "  Model 01.  The key always has the same name regardless of what the keymap in the current" << std::endl;
  std::cout << "  Kaleidoscope sketch may or may not be doing.  As an exception to the printed-name rule, we" << std::endl;
  std::cout << "  distinguish physical keys with the same text (ctrl, shift, and fn) with 'l' or 'r' indicating the hand." << std::endl;
  std::cout << "Here is a list of all the valid key \"physical\" names:" << std::endl;
  std::cout << "  prog 1 2 3 4 5 led any 6 7 8 9 0 num ` q w e r t y u i o p = pgup a s d f g tab enter h j k l ; '" << std::endl;
  std::cout << "  pgdn z x c v b esc fly n m , . / - lctrl bksp cmd lshift lfn rshift alt space rctrl rfn" << std::endl;
  std::cout << "The comment character '#' instructs the program to ignore the rest of the line (either in the" << std::endl;
  std::cout << "  script, or in interactive mode)." << std::endl;
  std::cout << "\nExample script:" << std::endl;
  std::cout << "  t             # first scan cycle: tap the physical T key" << std::endl;
  std::cout << "  esc           # next scan cycle: tap the physcial esc key" << std::endl;
  std::cout << "                # take no action for a scan cycle" << std::endl;
  std::cout << "  (2,1)         # tap the key in row 2, column 1" << std::endl;
  std::cout << "  lshift e      # tap the lshift and e keys simultaneously" << std::endl;
  std::cout << "  s (1,3) (3,8) # tap the s key, the key at (1,3), and the key at (3,8) simultaneously" << std::endl;
  std::cout << "  p q lfn fly   # tap the p, q, lfn, and fly keys simultaneously" << std::endl;
  std::cout << "\n2. ADVANCED\n" << std::endl;
  std::cout << "In addition to key names and the comment command '#', there are various other commands available." << std::endl;
  std::cout << "Key names are always in all lowercase (defined as symbols that appear in the unshifted positions" << std::endl;
  std::cout << "  on the standard QWERTY Model 01); uppercase/shifted symbols denote commands." << std::endl;
  std::cout << "Commands can be inserted anywhere in the input line, and affect the handling of keys following." << std::endl;
  std::cout << "The default command, which we used above, is 'tap' (where the key is 'down' for just this cycle)." << std::endl;
  std::cout << "'tap' can also be explicitly specified by 'T', as in \"T b\" to 'tap' the physical B key." << std::endl;
  std::cout << "You can hold virtual keys down using the 'D' (down) command.  The key will remain held until you" << std::endl;
  std::cout << "  say otherwise. In interactive mode, while keys are held, the prompt changes from '>' to '+>'." << std::endl;
  std::cout << "You can release a previously held virtual key using the 'U' command." << std::endl;
  std::cout << "Commands affect all following keys within the line unless overridden. So, \"D lshift u\" holds" << std::endl;
  std::cout << "  both lshift and u. To hold lshift and tap u, either enter \"D lshift T u\", or \"u D lshift\"." << std::endl;
  std::cout << "An exception to the above rule is the command 'C', which releases all currently held keys." << std::endl;
  std::cout << "One final command, 'Q', will quit the program.  In non-interactive mode (i.e. with an input" << std::endl;
  std::cout << "  script), the end of the script also implicitly indicates the end of the program." << std::endl;
  std::cout << "\nAdvanced script example:" << std::endl;
  std::cout << "  h            # tap the physical H key" << std::endl;
  std::cout << "  D lshift     # hold the physical lshift key down" << std::endl;
  std::cout << "  c (0,3)      # tap both c and the key at (0,3) (with lshift held)" << std::endl;
  std::cout << "  D alt        # hold alt (in addition to lshift)" << std::endl;
  std::cout << "  U lshift T e # Release lshift, and tap e in the same cycle" << std::endl;
  std::cout << "               # Do nothing for a scan cycle (but keep alt held)" << std::endl;
  std::cout << "  C            # Release all held keys (in this case, just alt)" << std::endl;
  std::cout << "  enter D (1,12) # Tap the physical enter key, and hold the key at (1,12)" << std::endl;
  std::cout << "  fly          # Tap the fly key (with (1,12) held)" << std::endl;
  std::cout << "  Q            # Quit the program" << std::endl;
  std::cout << std::endl;
}
