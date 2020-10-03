//
// Created by alex on 10/2/20.
//

#ifndef INC_2020_ROBOT_CODE_PICONZERO_HPP
#define INC_2020_ROBOT_CODE_PICONZERO_HPP

#include "piconzero.h"

class PiconZero{
public:
    enum PortDirection
    {
        INPUT,
        OUTPUT
    };
    PiconZero();

    void setPortDirection(uint8_t port, PortDirection dir);
    void setMotor(uint8_t motor_num, int8_t value);
    int8_t getInput(uint8_t port_num);
    void setServo(uint8_t port_num, int8_t value);
    void setPWM(uint8_t port_num, int8_t value);

    ~PiconZero();
private:
    std::unique_ptr<PiconZero_t, pz_destroy> pz;
};

#endif //INC_2020_ROBOT_CODE_PICONZERO_HPP
