/**
 * @file Function definitions for the PiconZero high-level class.
 * @author Alex Kneipp
 */
#include "PiconZero.hpp"
#include <iostream>

PiconZero* PiconZero::instance = nullptr;

PiconZero* PiconZero::getInstance() {
    //The singleton has not yet been created
    if (this->instance == nullptr) {
        this->instance = new PiconZero();
    }
    return this->instance;
}

void PiconZero::setPortConfig(uint8_t port, PortDirection dir, PortConfig cfg) {
    if (dir == INPUT) {
        //The values for our enum and the piconzero enum are the same for input
        pz_setInputConfig(this->pz.get(), port, (PZ_InputConfig_t) cfg, 0);
    }
        //PortDirection is OUTPUT
    else {
        //The values for our enum are the same as the piconzero enum but offset by 10,
        //Unless it's DIGITAL, then it's the same
        if (cfg != DIGITAL) {
            pz_setOutputConfig(this->pz.get(), port, (PZ_OutputConfig_t) (cfg - 10));
        } else {
            pz_setOutputConfig(this->pz.get(), port, (PZ_OutputConfig_t) (cfg));
        }
    }
}

int PiconZero::setMotor(uint8_t motor_num, int8_t value) {
    return pz_setMotor(this->pz.get(), motor_num, value);
}

int16_t PiconZero::readInput(uint8_t port_num) {
    errno = 0;
    int16_t rval = 0;
    //readInput does not return the read value, but an error code
    errno = pz_readInput(this->pz.get(), port_num, &rval);
    if (errno == 0) {
        return rval;
    }
    return -1;
}

void PiconZero::setServo(uint8_t port_num, int8_t value) {
    //TODO not yet implemented
    std::cerr << "PiconZero::setServo is not yet implemented" << std::endl;
}

void PiconZero::setPWM(uint8_t port_num, int8_t value) {
    //TODO not yet implemented
    std::cerr << "PiconZero::setPWM is not yet implemented" << std::endl;
}

PiconZero::~PiconZero() {
    //reset the singleton so we can properly re-create a piconzero if necessary
    this->instance = nullptr;
    pz_destroy(this->pz.get());
    this->pz.release();
}