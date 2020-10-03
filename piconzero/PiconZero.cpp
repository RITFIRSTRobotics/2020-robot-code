//
// Created by alex on 10/2/20.
//

#include "PiconZero.hpp"

PiconZero::PiconZero()
{
    this.pz = pz_create();
    pz_init(this.pz);
}

void PiconZero::setPortDirection(uint8_t port, PortDirection dir)
{

}
void PiconZero::setMotor(uint8_t motor_num, int8_t value)
{

}
int8_t PiconZero::getInput(uint8_t port_num)
{

}
void PiconZero::setServo(uint8_t port_num, int8_t value)
{

}
void PiconZero::setPWM(uint8_t port_num, int8_t value)
{

}

PiconZero::~PiconZero()
{

}
