/*
 * adc.h
 *
 * Created: 5/7/2024 14:14:34
 *  Author: Barcala
 */ 


#ifndef ADC_H_
#define ADC_H_
#include <avr/io.h>

// Archivo de cabecera del Microcontrolador
#include <avr/io.h>

// Inicializa la configuración del ADC
void ADC_Init();

// Habilita el ADC para realizar conversiones
void ADC_Enable();

// Deshabilita el ADC para detener las conversiones
void ADC_Disable();

// Obtiene el resultado de la conversión analógico-digital
uint8_t ADC_GetResult();


#endif /* ADC_H_ */