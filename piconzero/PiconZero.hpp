/**
 * @file Class Declaration of the PiconZero Class.
 * @author Alex Kneipp
 */
#ifndef INC_2020_ROBOT_CODE_PICONZERO_HPP
#define INC_2020_ROBOT_CODE_PICONZERO_HPP

#include "piconzero.h"
#include <memory>

class PiconZero {
public:
    /**
     * Possible directions for data on a piconzero port to travel.
     */
    enum PortDirection
    {
        INPUT,
        OUTPUT
    };
    /**
     * All possible port configurations for the piconzero (Some are unsupported).
     */
    enum PortConfig
    {
        DIGITAL = 0,
        ANALOG = 1,
        DS18B20 = 2,
        DUTY_CYCLE = 4,
        PULSE_WIDTH = 5,
        PWM = 11,
        SERVO = 12,
        WS2812B = 13
    };


    /**
     * Get the singleton instance of the PiconZero, or create a new one and return it.
     * @return
     *  A singleton instance of a PiconZero object
     */
    static PiconZero* getInstance() {
        // The singleton has not yet been created
        if (PiconZero::instance == nullptr) {
            PiconZero::instance = new PiconZero();
        }
        return PiconZero::instance;
    }

    /**
     *
     * @param port
     * @param dir
     * @param cfg
     */
    void setPortConfig(uint8_t port, PortDirection dir, PortConfig cfg);

    /**
     *
     * @param motor_num
     * @param value
     * @return
     */
    int setMotor(uint8_t motor_num, int8_t value);

    /**
     *
     * @param port_num
     * @return
     */
    int16_t readInput(uint8_t port_num);

    /**
     *
     * @param port_num
     * @param value
     */
    void setServo(uint8_t port_num, int8_t value);

    /**
     *
     * @param port_num
     * @param value
     */
    void setPWM(uint8_t port_num, int8_t value);

    ~PiconZero();
private:
    /**
     * Construct a singleton PiconZero object
     */
    PiconZero()
    {
        this->pz.reset(pz_create());
        pz_init(this->pz.get());
    }
    std::unique_ptr<PiconZero_t> pz;
    static PiconZero* instance;
};

#endif //INC_2020_ROBOT_CODE_PICONZERO_HPP
