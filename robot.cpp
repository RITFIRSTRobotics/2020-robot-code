/**
 * robot.cpp
 *
 * Main entry point for the 2020-robot-code project
 *
 * @author Connor Henley, @thatging3rkid
 */

// standard includes
#include <memory>
#include <ncurses.h>
#include <iostream>

// Picon Zero includes
#include "piconzero/PiconZero.hpp"

// debugging includes
#include "core/utils/dbgprint.h"
#include "core/utils/dbgcommit.h"

using namespace std;

int main(int argc, char* argv[]) {
    // Initialize keyboard scanning
    initscr();

    // Startup info
    dbg_info("starting up robot code...\n");
    dbg_info("git hash = %08x\n", GIT_COMMIT_HASH);

    // Initialize the Picon Zero
    PiconZero* pz = PiconZero::getInstance();           // breaking??????

    // Store keyboard input
    char keyin = 0;

    dbg_info("Robot code started!\n\r");
    while (true) {
        // Read keyboard inputs
        // TODO
        keyin = 0;
        keyin = getch();

        if (keyin > 0) {
          switch(keyin) {
            case 'w':
              dbg_info("Forward!\n\r");
              break;
            case 'a':
              dbg_info("Left!\n\r");
              break;
            case 's':
              dbg_info("Backward!\n\r");
              break;
            case 'd':
              dbg_info("Right\n\r");
              break;
          }
        }

        // Set motor outputs
        // TODO
    }

    // Clean-up Picon Zero
    // TODO
}
