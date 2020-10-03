/**
 * robot.cpp
 *
 * Main entry point for the 2020-robot-code project
 *
 * @author Connor Henley, @thatging3rkid
 */

// standard includes
#include <memory>
#include <iostream>

// Picon Zero includes
#include "piconzero/PiconZero.hpp"

// debugging includes
#include "core/utils/dbgprint.h"
#include "core/utils/dbgcommit.h"

int main(int argc, char* argv[]) {

    // Startup info
    dbg_info("starting up robot code...\n");
    dbg_info("git hash = %08x\n", GIT_COMMIT_HASH);

    // Initialize the Picon Zero
    //PiconZero* pz = PiconZero::getInstance();
    //pz.


    while (true) {
        // Read keyboard inputs
        // TODO

        // Set motor outputs
        // TODO
    }

    // Clean-up Picon Zero
    // TODO
}
