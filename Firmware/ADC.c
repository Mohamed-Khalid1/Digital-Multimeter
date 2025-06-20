/*
 * ADC.c
 *
 *  Created on: Jul 2, 2023
 *      Author: KHALED
 */

#include <avr/io.h>
#include "common_macros.h" /* For GET_BIT Macro */
#include "ADC.h"
#include "gpio.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Initialize the AVR's ADC settings:
 * 1. Enable the ADC pin
 * 2. Set the pre-scaler value
 * 3. Select the reference voltage
 */
void ADC_init(const ADC_config_t* Config_ptr)
{
	SET_BIT(ADCSRA, ADEN);			// Enable ADC pin ADEN

	/* Set the pre-scaler value with safety mask */
	ADCSRA = (ADCSRA & 0xF8)|(Config_ptr->ADC_Prescaler);

	/* Select the reference voltage with safety mask */
	ADMUX = (ADMUX & 0x3F)|(Config_ptr->ADC_REF_V << REFS0);
}

/*
 * Description :
 * Reads the input analog voltage on specified ADC channel
 * Stores the corresponding decimal value of the voltage into "ADC" Register
 * Returns the equivalent voltage level
 */
float32 ADC_read(uint8 ADC_channel)
{
	/* Configure the direction for ADC pin as input pin */
	GPIO_setupPinDirection(PORTA_ID, ADC_channel, PIN_INPUT);

	/* Select the ADC channel ADC7:ADC0 using MUX[2:0] */
	ADMUX = ADC_channel | (ADMUX & 0xF0);

	SET_BIT(ADCSRA, ADSC);				// Conversion starts

	while(BIT_IS_SET(ADCSRA, ADSC));	// Wait until conversion is complete

	return ADC * ((float32)REF_V/RES);	// Returns the equivalent voltage (0 ~ 5 V)
}
