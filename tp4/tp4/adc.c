/*
 * adc.c
 *
 * Created: 5/7/2024 14:16:19
 *  Author: Barcala
 */ 
#include "adc.h"

void ADC_Init() {
	// Configurar pin de ADC3 como entrada analógica (PC3)
	DDRC &= ~(1<<PORTC3); // Configurar el pin como entrada (0)
	DIDR0 |= (1<<PORTC3); // Desactivar el buffer digital para reducir consumo

	// Establecer VCC como referencia (REFS1:0 = 1)
	ADMUX = (1<<REFS0); // Usar VCC como referencia para conversiones

	// Seleccionar ADC3 como entrada analógica (MUX3:0 = 3)
	ADMUX |= (1<<MUX1)|(1<<MUX0); // Usar ADC3 (PC3) como entrada analógica

	// Ubicar los 10 bits desde la izquierda (ADLAR = 1)
	ADMUX |= (1<<ADLAR); // Alinear el resultado de 10 bits a la izquierda en ADCH

	// Establecer prescaler de 128, mediante ADPS2:0 = 7
	// Se garantiza una buena performance hasta 200 kHz
	// Para 16 MHz, la frecuencia del ADC será 125 kHz
	ADCSRA = (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // Configurar el prescaler en 128

	// Habilitar interrupción de conversión finalizada
	//ADCSRA |= (1<<ADIE); // Permitir la interrupción de finalización de conversión
}

void ADC_Enable()
{
	// Habilitar conversor ADC mediante bit ADC Enable
	ADCSRA |= (1<<ADEN); // Habilitar el conversor ADC
}

void ADC_Disable()
{
	// Deshabilitar conversor ADC
	ADCSRA &= ~(1<<ADEN); // Deshabilitar el conversor ADC
}

uint8_t ADC_GetResult()
{
	// Devuelvo parte alta del registro ADC
	// ADC utiliza 10 bits (1024 niveles), con 8 bits: 4 niveles x bit
	
	ADCSRA |= (1<<ADSC);//start conversion
	while((ADCSRA&(1<<ADIF))==0);//wait for conversion to finish
	ADCSRA |= (1<<ADIF); //borrar flag
	
	return ADCH; // Devolver el resultado de la conversión del ADC (parte alta)
}

