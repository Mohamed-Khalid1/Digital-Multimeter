/*
 * ADC.h
 *
 *  Created on: Jul 2, 2023
 *      Author: KHALED
 */

#ifndef ADC_H_
#define ADC_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define REF_V 			5				// Reference voltage
#define RES 			1024			// Resolution : 10 bits > RES = 2^10

/* Defines different voltage references with their code */
typedef enum {
	AREF,
	AVCC,
	INTL = 3
}Vref_code;

/* Defines all possible pre-scaler values with their code */
typedef enum {
	D_F_2 = 1,
	D_F_4,
	D_F_8,
	D_F_16,
	D_F_32,
	D_F_64,
	D_F_128
}prescaler_code;

/* Used in initializing the ADC settings : pre-scaler & voltage reference */
typedef struct {
	Vref_code 		ADC_REF_V;
	prescaler_code 	ADC_Prescaler;
}ADC_config_t;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Initialize the AVR's ADC settings:
 * 1. Enable the ADC pin
 * 2. Set the pre-scaler value
 * 3. Select the reference voltage
 */
void ADC_init(const ADC_config_t* Config_Ptr);

/*
 * Description :
 * Reads the input analog voltage on specified ADC channel
 * Stores the corresponding decimal value of the voltage into "ADC" Register
 * Returns the equivalent voltage level
 */
float32 ADC_read(uint8 ADC_channel);

#endif /* ADC_H_ */
