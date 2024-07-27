/*
 * led.h
 *
 * Created: 05/07/2024 17:15:33
 *  Author: jose
 */ 


#ifndef LED_H_
#define LED_H_
#include <avr/io.h>
#include <avr/interrupt.h>
#include "pwm.h"
// Pin conectado a cada LED
#define PIN_RED PORTB5
#define PIN_GREEN PORTB2
#define PIN_BLUE PORTB1
// Configura los pines conectados al LED como salida
void LED_Init();

// Actualiza los valores de intensidad para cada color
void LED_Update(uint8_t red, uint8_t green, uint8_t blue);

// Actualiza el valor de intensidad del LED rojo
void LED_UpdateRed(uint8_t bright, uint8_t save);

// Actualiza el valor de intensidad del LED verde
void LED_UpdateGreen(uint8_t bright, uint8_t save);

// Actualiza el valor de intensidad del LED azul
void LED_UpdateBlue(uint8_t bright, uint8_t save);

// Apaga el LED ubicado en el PIN de Port B especificado
void LED_ON(uint8_t);

// Enciende el LED ubicado en el PIN de Port B especificado
void LED_OFF(uint8_t);



#endif /* LED_H_ */