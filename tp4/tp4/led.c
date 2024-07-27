/*
 * led.c
 *
 * Created: 05/07/2024 17:15:48
 *  Author: jose
 */ 
// LEDs disponibles para la modificación de su brillo
typedef enum {OP_RED, OP_GREEN, OP_BLUE, OP_ALL} LED_Option;

// Valor a escribir en la salida
// Si es ánodo común (VCC), se escribe un 0 para encender
// Si es cátodo común (GND), se escribe un 1 para encender

#define ANODO_COMUN 1

#if ANODO_COMUN
#define NIVEL_ON 0
#define NIVEL_OFF 1
#else
#define NIVEL_ON 1
#define NIVEL_OFF 0
#endif



// Archivo de cabecera
#include "led.h"
#include "pwm.h"
// Declaración de tipos
typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} LED_UserValues;

// Prototipos de funciones privadas
static void setAlto(uint8_t);
static void setBajo(uint8_t);

// Variables privadas
static LED_UserValues userValues = {0,0,0};

void LED_Init() {
	// Configurar cada pin conectado a un LED como salida
	DDRB |= (1<<PIN_RED)|(1<<PIN_GREEN)|(1<<PIN_BLUE);
	
	// Apagar todos los LEDs por defecto
	LED_OFF(PIN_RED);
	LED_OFF(PIN_GREEN);
	LED_OFF(PIN_BLUE);
}

void LED_Update(uint8_t red, uint8_t green, uint8_t blue) {
	
	// Asignar la intensidad de rojo
	LED_UpdateRed(red, 1);
	
	// Asignar la intensidad de verde
	LED_UpdateGreen(green, 1);
	
	// Asignar la intensidad de azul
	LED_UpdateBlue(blue, 1);
}

void LED_UpdateRed(uint8_t bright, uint8_t save){
	// El LED rojo se controla mediante interrupciones de T0
	// La ISR de TIMER0_OVF enciende el LED
	// La ISR de TIMER0_COMPA apaga el LED
	
	// Si la intensidad es 0, se debe mantener apagado el LED
	if (bright == 0) {
		// Desactivar interrupción de Overflow
		RELOJ_T0_OVF_Interrupt_Disable();
		
		// Forzar apagado del LED
		LED_OFF(PIN_RED);
	}
	else if (bright == 255) {
		// Sino, si la intensidad es máxima, se debe mantener encendido
		// Desactivar interrupción de Compare A
		RELOJ_T0_COMPA_Interrupt_Disable();
		
		// Forzar encendido del LED
		LED_ON(PIN_RED);
		} else {
		// Sino...
		// Activar interrupciones de T0
		RELOJ_T0_OVF_Interrupt_Enable();
		RELOJ_T0_COMPA_Interrupt_Enable();
		
		// Actualizar valor de brillo
		RELOJ_T0_UpdateOCR(bright);
	}
	
	// Guardar intensidad personalizada
	if (save) userValues.red = bright;
}

void LED_UpdateGreen(uint8_t bright, uint8_t save) {
	// El LED verde se controla mediante señal PWM de OC1B
	// Se trabaja en Fast PWM, modo invertido.
	
	// Si la intensidad es 0, se debe mantener apagado el LED
	if (bright == 0) {
		// Desconectar señal OC1B
		RELOJ_T1_PWM_B_Disable();
		
		// Forzar apagado del LED
		LED_OFF(PIN_GREEN);
		} else {
		// Sino...
		// Habilitar señal OC1B
		RELOJ_T1_PWM_B_Enable();
		
		// Actualizar valor de brillo
		RELOJ_T1_PWM_B_UpdateOCR(bright);
	}
	
	// Guardar intensidad personalizada
	if (save) userValues.green = bright;
}

void LED_UpdateBlue(uint8_t bright, uint8_t save) {
	// El LED azul se controla mediante señal PWM de OC1A
	// Se trabaja en Fast PWM, modo invertido.
	
	// Si la intensidad es 0, se debe mantener apagado el LED
	if (bright == 0) {
		// Desconectar señal OC1A
		RELOJ_T1_PWM_A_Disable();
		
		// Forzar apagado del LED
		LED_OFF(PIN_BLUE);
		} else {
		// Sino...
		// Habilitar señal OC1A
		RELOJ_T1_PWM_A_Enable();
		
		// Actualizar valor de brillo
		RELOJ_T1_PWM_A_UpdateOCR(bright);
	}
	
	// Guardar intensidad personalizada
	if (save) userValues.blue = bright;
}

void LED_ON(uint8_t pin){
	// Poner un nivel alto o bajo según macro NIVEL_ON
	
	#if NIVEL_ON
	setAlto(pin);
	#else
	setBajo(pin);
	#endif
}

void LED_OFF(uint8_t pin){
	// Poner un nivel alto o bajo según macro NIVEL_OFF
	
	#if NIVEL_OFF
	setAlto(pin);
	#else
	setBajo(pin);
	#endif
}

static void setAlto(uint8_t pin){
	PORTB |= (1<<pin);
}

static void setBajo(uint8_t pin){
	PORTB &= ~(1<<pin);
}

// Rutina de Servicio de Interrupción de Timer 0 - Overflow
// De forma análoga al Modo Invertido de Timer 1, empieza en nivel bajo
ISR(TIMER0_OVF_vect)
{
	LED_ON(PIN_RED);
}

// Rutina de Servicio de Interrupción de Timer 0 - Compare Match A
// Luego de haberse generado un nivel bajo, se pasa a nivel alto
ISR(TIMER0_COMPA_vect)
{
	LED_OFF(PIN_RED);
}
