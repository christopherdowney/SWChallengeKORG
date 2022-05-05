/*
 * led.hpp
 *
 *  (c) KORG Germany GmbH
 *  SW ENGINEER CODE CHALLENGE
 *
 *  Created on: Apr 28, 2022
 *      Author: Maximilian Rest (c)
 */


#ifndef KDE_LIB_LED_HPP_
#define KDE_LIB_LED_HPP_

#include <stm32f3xx_hal.h>

#define LED_BRIGHTNESS_MAX           100
#define LED_N_BGHT_LEVELS             32
#define LED_N_BGHT_ENTRIES           (2*LED_N_BGHT_LEVELS)

#define LED_BREATHE_PERIOD           200
#define LED_BREATHE_OFFSET             3

#define LED_BLINK_PERIOD            1500
#define LED_BLINK_COUNT                3
#define LED_BLINK_OFFSET              10

#define LED_HOLD_PERIOD             6000

#define LED_LOW_ACTIVE                 1

static const uint8_t brightnessLookup[LED_N_BGHT_ENTRIES] = {
		0, 6, 13, 19, 26, 32, 39, 45, 52, 58, 65, 71, 77, 84, 90, 97, 103, 110, 116, 123, 129, 135, 142, 148, 155, 161, 168, 174, 181, 187, 194, 200
		, 200, 194, 187, 181, 174, 168, 161, 155, 148, 142, 135, 129, 123, 116, 110, 103, 97, 90, 84, 77, 71, 65, 58, 52, 45, 39, 32, 26, 19, 13, 6, 0
};

enum LED_STATE{
    OFF,
    ON,
    DIM,
    BREATHE,
    BLINK,
    BLINK_HOLD_ON,
    BLINK_HOLD_OFF,
    HOLD
};


namespace IO {

    class Led{
        public:
            // init of hardware, led in OFF state
            void init( GPIO_TypeDef* LED_GPIO, uint16_t LED_Pin );

            // setters for main states
            void off( );
            void on( );
            void breathe();                     // show "breathing" animation
            void dim(uint8_t brightnessLevel);  // set to defined brightness

            // setters for intermediate/confirmation states. return to previous main state after finishing
            void blink();                       // blink 3x @4Hz
            void blinkHoldOff();                // blink 3x @4Hz, then keep OFF for 1 second
            void blinkHoldOn();                 // blink 3x @4Hz, then keep OFF for 1 second

            // call reqularly to update hardware
            void update();

            LED_STATE     currentState;

        private:
            GPIO_TypeDef* _LED_GPIO;
            uint16_t      _LED_Pin;
            LED_STATE     _returnState;
            // change/add here as you need, just ideas:
            LED_STATE     _holdState;
            uint8_t       _brightnessLevel;
            uint8_t       _brightnessCounter;
            uint8_t		  _brightnessPeriod;
            uint8_t		  _breatheCounter;
            uint16_t	  _holdCounter;
            uint16_t      _blinkCounter;
            uint16_t	  _blinkRepCounter;

    };

};


#endif /* KDE_LIB_LED_HPP_ */
