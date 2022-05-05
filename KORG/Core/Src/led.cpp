/*
 * led.cpp
 *
 *  Created on: May 4, 2022
 *      Author: Christopher.Downey
 */
#include<led.hpp>
using namespace IO;
using namespace std;

/*
 * @brief	Function to initialize an LED object.
 *
 * @param 	LED_GPIO The port of the GPIO connected to the LED
 * @param 	LED_Pin The pin number of the GPIO connected to the LED
 */
void Led::init( GPIO_TypeDef* LED_GPIO, uint16_t LED_Pin ){
	_LED_GPIO = LED_GPIO;
	_LED_Pin = LED_Pin;
	currentState = OFF;
}

/*
 * @brief	Function to turn off an LED
 */
void Led::off(){
	HAL_GPIO_WritePin(_LED_GPIO, _LED_Pin, (GPIO_PinState) LED_LOW_ACTIVE);
}
/*
 * @brief 	Function to turn on an LED
 */
void Led::on(){
	HAL_GPIO_WritePin(_LED_GPIO, _LED_Pin, (GPIO_PinState) (1 - LED_LOW_ACTIVE));
}
/*
 * @brief 	Function to show "breathing" animation on an LED.
 *
 * @note	Function counts periods of the breathing animation and uses it to index
 * 			a lookup table which contains a "sawtooth" like array of duty cycles to create
 * 			a PWM signal for the LED.
 */
void Led::breathe(){

	currentState = BREATHE;

	//moving to next period of animation
	if(_brightnessCounter >= LED_BREATHE_PERIOD){
		_brightnessCounter = 0;
		_breatheCounter ++;
	}
	//restarting the animation
	else if(_breatheCounter >= LED_N_BGHT_ENTRIES){
		_breatheCounter = 0;
	}
	//increment within a period
	else{
		_brightnessCounter++;
	}
	//index LUT to find duty cycle
	_brightnessPeriod = brightnessLookup[_breatheCounter];
	//turn on LED if within duty cycle limit
	if(_brightnessCounter <= _brightnessPeriod){
		on();
	}
	else{
		off();
	}
}
/*
 * @brief	Function to set the brightness of an LED by creating a PWM signal whose duty cycle
 * 			is proportional to the input brightness level
 *
 * @param	brightnessLevel Input brightness as a percentage
 */
void Led::dim(uint8_t brightnessLevel){

	currentState = DIM;
	_brightnessLevel = brightnessLevel;

	// _brightnessCounter counts from 1 to 200 to give 30Hz
	if(_brightnessCounter >= LED_BREATHE_PERIOD){
		_brightnessCounter = 0;
	}
	else{
		_brightnessCounter++;
	}
	// conversion from percentage to index of LUT
	float brightnessIdx =  (float)LED_N_BGHT_LEVELS * ((float)_brightnessLevel/LED_BRIGHTNESS_MAX);
	_brightnessPeriod = brightnessLookup[(uint8_t) brightnessIdx];

	//turn on LED if within duty cycle limit
	if(_brightnessCounter <= _brightnessPeriod){
		on();
	}
	else{
		off();
	}

}

/*
 * @brief	Function to blink an LED three times at 4Hz
 */
void Led::blink(){
	//handling returns to previous state
	if(currentState != BLINK){
		_returnState = currentState;
		currentState = BLINK;
	}
	//blinkCounter counts through the period of a blink
	_blinkCounter++;

	//since the led needs to be off then on then off to visibly blink regardless of previous state
	//the blink period is divided in to three equal parts
	if(_blinkCounter >= LED_BLINK_OFFSET && _blinkCounter <= LED_BLINK_PERIOD/3){
		off();
	}
	else if(_blinkCounter >= LED_BLINK_PERIOD/3 && _blinkCounter <= 2*LED_BLINK_PERIOD/3){
		on();
	}
	else{
		off();
	}

	//reaching the end of a blink period
	if(_blinkCounter >= LED_BLINK_PERIOD){
		//reseting blink period counter and incrementing blink repetition counter
		_blinkCounter = 0;
		_blinkRepCounter ++;
		//reaching the end of three blink periods
		if(_blinkRepCounter >= LED_BLINK_COUNT){
			_blinkRepCounter = 0;
			currentState = _returnState;
		}
	}
}
/*
 * @brief	Function of blink an LED three times at 4Hz then hold the LED Off for 1 second.
 */
void Led::blinkHoldOff(){
	//checking if we have transitioned from a different state and not returned from the blink state
	if(currentState != BLINK_HOLD_OFF && currentState != BLINK){
		_holdState = currentState;
		currentState = BLINK_HOLD_OFF;
		//executing three blinks
		blink();
	}
	else{
		//holding off
		off();
		_holdCounter++;
	}
	//reaching the end of the hold period
	if(_holdCounter >= LED_HOLD_PERIOD){
		//reseting the counter and returning to the original state
		_holdCounter = 0;
		currentState = _holdState;
	}
}
/*
 * @brief	Function of blink an LED three times at 4Hz then hold the LED Off for 1 second.
 */
void Led::blinkHoldOn(){
	//checking if we have transitioned from a different state and not returned from the blink state
	if(currentState != BLINK_HOLD_ON && currentState != BLINK){
		_holdState = currentState;
		currentState = BLINK_HOLD_ON;
		//executing three blinks
		blink();
	}
	else{
		//holding on
		on();
		_holdCounter++;
	}
	//reaching the end of the hold period
	if(_holdCounter >= LED_HOLD_PERIOD){
		//reseting the counter and returning to the original state
		_holdCounter = 0;
		currentState = _holdState;
	}
}

// call reqularly to update hardware
void Led::update(){
	switch(currentState){

	case OFF:
		off();
		break;

	case ON:
		on();
		break;

	case DIM:
		dim(_brightnessLevel);
		break;

	case BREATHE:
		breathe();
		break;

	case BLINK:
		blink();
		break;

	case BLINK_HOLD_ON:
		blinkHoldOn();
		break;

	case BLINK_HOLD_OFF:
		blinkHoldOff();
		break;

	case HOLD:
		break;

	default:
		currentState = OFF;
		break;
	}

}

