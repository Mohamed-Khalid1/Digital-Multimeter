/*
 * Main.c
 *
 *  Created on: Feb 16, 2024
 *      Author: KHALED
 */

#include <avr/io.h>
#include <util/delay.h>
#include "gpio.h"
#include "lcd.h"
#include "ADC.h"

#define INPUT_PORT		PORTA_ID
#define VOLTMETER_PIN	PIN0_ID
#define AMMETER_PIN		PIN1_ID
#define AC_METER_PIN	PIN2_ID
#define OHMMETER_PIN	PIN3_ID

#define CTRL_PORT		PORTB_ID
#define HIGH_SEL_PIN	PIN0_ID
#define MED2_SEL_PIN	PIN1_ID
#define MED1_SEL_PIN	PIN2_ID
#define LOW_SEL_PIN		PIN3_ID

#define DC_V_SEL_PIN	PIN4_ID
#define AC_V_SEL_PIN	PIN5_ID
#define DC_I_SEL_PIN	PIN6_ID
#define AC_I_SEL_PIN	PIN7_ID
#define RES_SEL_PORT	PORTA_ID
#define RES_SEL_PIN		PIN5_ID

#define MUX_SEL_PORT	PORTD_ID
#define HIGH_VOLT_PIN	PIN0_ID
#define MED_VOLT_PIN	PIN1_ID
#define LOW_VOLT_PIN	PIN2_ID

#define MSB_CURT_PIN	PIN3_ID
#define LSB_CURT_PIN	PIN4_ID

#define MSB_RES_PIN		PIN5_ID
#define LSB_RES_PIN		PIN6_ID

#define ACtoDC_PIN		PIN7_ID

void Voltmeter(float32 Reading, uint8 Scale_Sel);
void Ammeter(float32 Reading, uint8 Scale_Sel);
void Ohmmeter(float32 Reading, uint8 Scale_Sel);

int main(void) {
	GPIO_setupPortDirection(INPUT_PORT, PORT_INPUT);	// ADC input Port
	GPIO_writePort(INPUT_PORT, 0x00);					// Deactivate pull-up resistor for the Port

	GPIO_setupPortDirection(CTRL_PORT, PORT_INPUT);		// Scale & Meter Selection Port (Control Panel)
	GPIO_writePort(CTRL_PORT, 0xFF);					// Activate pull-up resistor for the whole port
	GPIO_setupPinDirection(RES_SEL_PORT, RES_SEL_PIN, PIN_INPUT);
	GPIO_writePin(RES_SEL_PORT, RES_SEL_PIN, LOGIC_HIGH);

	GPIO_setupPortDirection(MUX_SEL_PORT, PORT_OUTPUT);
	GPIO_writePort(MUX_SEL_PORT, 0x00);

	ADC_config_t Var_RES = {AVCC, D_F_128};
	ADC_init(&Var_RES);

	LCD_init();

	float32 Voltage;
	uint8 Scale_Sel;
	uint8 Meter_Sel;

	while(1)
	{
		Scale_Sel = 0x0F & GPIO_readPort(CTRL_PORT);	// Reading the first 4 bins

		Meter_Sel = (GPIO_readPin(RES_SEL_PORT, RES_SEL_PIN) << PIN4_ID) | (GPIO_readPort(CTRL_PORT) >> PIN4_ID);

		switch (Scale_Sel)				/* Meter scale selection (same selection for the 3 meters)
									   	   only one input will pass according to the ADC input selection */
		{
			/* Highest Range */
		case 0b1110:
			/* Ammeter & Ohmmeter Scale Selection */
			GPIO_writePin(MUX_SEL_PORT, MSB_RES_PIN, LOGIC_HIGH);
			GPIO_writePin(MUX_SEL_PORT, LSB_RES_PIN, LOGIC_HIGH);

			GPIO_writePin(MUX_SEL_PORT, MSB_CURT_PIN, LOGIC_HIGH);
			GPIO_writePin(MUX_SEL_PORT, LSB_CURT_PIN, LOGIC_HIGH);

			/* Voltmeter 200 V Selection */
			GPIO_writePin(MUX_SEL_PORT, HIGH_VOLT_PIN, LOGIC_LOW);

			GPIO_writePin(MUX_SEL_PORT, MED_VOLT_PIN, LOGIC_HIGH);	// For MUX Protection
			break;

		case 0b1101:
			/* Ammeter & Ohmmeter Scale Selection */
			GPIO_writePin(MUX_SEL_PORT, MSB_RES_PIN, LOGIC_HIGH);
			GPIO_writePin(MUX_SEL_PORT, LSB_RES_PIN, LOGIC_LOW);

			GPIO_writePin(MUX_SEL_PORT, MSB_CURT_PIN, LOGIC_HIGH);
			GPIO_writePin(MUX_SEL_PORT, LSB_CURT_PIN, LOGIC_LOW);

			/* Voltmeter 20 V Selection */
			GPIO_writePin(MUX_SEL_PORT, HIGH_VOLT_PIN, LOGIC_HIGH);
			GPIO_writePin(MUX_SEL_PORT, MED_VOLT_PIN, LOGIC_HIGH);
			break;

		case 0b1011:
			/* Ammeter & Ohmmeter Scale Selection */
			GPIO_writePin(MUX_SEL_PORT, MSB_RES_PIN, LOGIC_LOW);
			GPIO_writePin(MUX_SEL_PORT, LSB_RES_PIN, LOGIC_HIGH);

			GPIO_writePin(MUX_SEL_PORT, MSB_CURT_PIN, LOGIC_LOW);
			GPIO_writePin(MUX_SEL_PORT, LSB_CURT_PIN, LOGIC_HIGH);

			/* Voltmeter 2 V Selection */
			GPIO_writePin(MUX_SEL_PORT, HIGH_VOLT_PIN, LOGIC_HIGH);
			GPIO_writePin(MUX_SEL_PORT, MED_VOLT_PIN, LOGIC_LOW);
			GPIO_writePin(MUX_SEL_PORT, LOW_VOLT_PIN, LOGIC_LOW);
			break;

			/* Lowest Range */
		case 0b0111:
			/* Ammeter & Ohmmeter Scale Selection */
			GPIO_writePin(MUX_SEL_PORT, MSB_RES_PIN, LOGIC_LOW);
			GPIO_writePin(MUX_SEL_PORT, LSB_RES_PIN, LOGIC_LOW);

			GPIO_writePin(MUX_SEL_PORT, MSB_CURT_PIN, LOGIC_LOW);
			GPIO_writePin(MUX_SEL_PORT, LSB_CURT_PIN, LOGIC_LOW);

			/* Voltmeter 0.2 V Selection */
			GPIO_writePin(MUX_SEL_PORT, HIGH_VOLT_PIN, LOGIC_HIGH);
			GPIO_writePin(MUX_SEL_PORT, MED_VOLT_PIN, LOGIC_LOW);
			GPIO_writePin(MUX_SEL_PORT, LOW_VOLT_PIN, LOGIC_HIGH);
			break;

		default:
			LCD_displayString("error 404");
			_delay_ms(500);
			LCD_clearScreen();
			break;
		}

		_delay_ms(50);										// MUX switching time

		switch (Meter_Sel)
		{
		case 0b11110:
			Voltage = ADC_read(VOLTMETER_PIN);
			Voltmeter(Voltage, Scale_Sel);
			break;

		case 0b11101:
			GPIO_writePin(MUX_SEL_PORT, ACtoDC_PIN, LOGIC_LOW);
			_delay_ms(50);
			Voltage = ADC_read(AC_METER_PIN);
			Voltmeter(Voltage, Scale_Sel);
			break;

		case 0b11011:
			Voltage = ADC_read(AMMETER_PIN);
			Ammeter(Voltage, Scale_Sel);
			break;

		case 0b10111:
			GPIO_writePin(MUX_SEL_PORT, ACtoDC_PIN, LOGIC_HIGH);
			_delay_ms(50);
			Voltage = ADC_read(AC_METER_PIN);
			Ammeter(Voltage, Scale_Sel);
			break;

		case 0b01111:
			Voltage = ADC_read(OHMMETER_PIN);
			Ohmmeter(Voltage, Scale_Sel);
			break;

		default:
			LCD_displayString("error 404");
			_delay_ms(500);
			LCD_clearScreen();
			break;
		}
	}
	return 0;
}

void Voltmeter(float32 Reading, uint8 Scale_Sel)
{
	float32 Scale;
	float32 Voltage;

	switch (Scale_Sel)
	{
	case 0b1110:
		Scale = 200/5.0;
		break;

	case 0b1101:
		Scale = 20/5.0;
		break;

	case 0b1011:
		Scale = 2/5.0;
		break;

	case 0b0111:
		Scale = 0.2/5.0;
		break;

	default:
		Scale = 0;
		break;
	}

	Voltage = Reading * Scale;

	if(Voltage < 1){
		LCD_floatToString(Voltage*1000);
		LCD_displayString(" mV");
		_delay_ms(500);

		LCD_clearScreen();
	}
	else{
		LCD_floatToString(Voltage);
		LCD_displayString(" V");
		_delay_ms(500);

		LCD_clearScreen();
	}

	return;
}

void Ammeter(float32 Reading, uint8 Scale_Sel)
{
	float32 Scale;
	float32 Current;

	switch (Scale_Sel)
		{
		case 0b1110:
			Scale = 100 / (33.3 * 7.5);
			break;

		case 0b1101:
			Scale = 10 / (33.3 * 7.5);
			break;

		case 0b1011:
			Scale = 1 / (33.3 * 7.5);
			break;

		case 0b0111:
			Scale = 0.1 / (33.3 * 7.5);
			break;

		default:
			Scale = 0;
			break;
		}

	Current = Reading * Scale;

	if(Current < 1){
		LCD_floatToString(Current*1000);
		LCD_displayString(" mA");
		_delay_ms(500);

		LCD_clearScreen();
	}
	else{
		LCD_floatToString(Current);
		LCD_displayString(" A");
		_delay_ms(500);

		LCD_clearScreen();
	}

	return;
}

void Ohmmeter(float32 Reading, uint8 Scale_Sel)
{
	uint32 Scale;
	float32 Resistance;

	switch (Scale_Sel)
	{
	case 0b1110:
		Scale = 1000000;
		break;

	case 0b1101:
		Scale = 100000;
		break;

	case 0b1011:
		Scale = 10000;
		break;

	case 0b0111:
		Scale = 1000;
		break;

	default:
		Scale = 0;
		break;
	}

	Resistance = (Reading / (10 - Reading)) * Scale;

	if(Resistance >= 1000000){
		LCD_floatToString(Resistance/1000000);
		LCD_displayString(" M");
		LCD_displayCharacter(0xF4);
		_delay_ms(500);

		LCD_clearScreen();
	}
	else if(Resistance >= 1000){
		LCD_floatToString(Resistance/1000);
		LCD_displayString(" K");
		LCD_displayCharacter(0xF4);
		_delay_ms(500);

		LCD_clearScreen();
	}
	else if(Resistance < 1){
		LCD_floatToString(Resistance*1000);
		LCD_displayString(" m");
		LCD_displayCharacter(0xF4);
		_delay_ms(500);

		LCD_clearScreen();
	}
	else{
		LCD_floatToString(Resistance);
		LCD_displayCharacter(' ');
		LCD_displayCharacter(0xF4);
		_delay_ms(500);

		LCD_clearScreen();
	}

	return;
}
