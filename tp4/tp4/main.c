/*
 * tp4.c
 *
 * Created: 01/07/2024 15:18:29
 * Author : jose
 */ 

#include <avr/io.h>
#include "pwm.h"
#include "serialPort.h"
#include "adc.h"
#include "led.h"
#define F_CPU 16000000UL
#include <util/delay.h> 
#define BR9600 (0x67)	// 0x67=103 configura BAUDRATE=9600@16MHz

char msg1[] = "Bienvenido al controlador RGB por UART.\r\n";
char msg2[] = "Envíe 'R' para modificar el rojo, 'G' para el verde, 'B' para el azul.\r\n";

volatile char RX_Buffer=0;

volatile uint8_t flag_intensidad=1;
uint8_t intensidad;

int main(void) {
	volatile char dato = 0;
	// Inicialización de Timer
	RELOJ_T1_Init();
	RELOJ_T0_Init();
	RELOJ_Start_Both();
	// Inicialización de LEDs
	LED_Init();

	// Inicialización de Conversor ADC
	ADC_Init();
	ADC_Enable(); // Habilitar el ADC para que esté listo para conversiones
	// Habilitar interrupciones globales
	

	
	uint8_t red = 255;
	uint8_t green = 0;
	uint8_t blue = 0;

	SerialPort_Init(BR9600);
	SerialPort_TX_Enable();			// Activo el Transmisor del Puerto Serie
	SerialPort_RX_Enable();			// Activo el Receptor del Puerto Serie 

	SerialPort_Send_String(msg1);
	SerialPort_Send_String(msg2);
	SerialPort_RX_Interrupt_Enable();
	sei();
	
    while (1) 
    {
		if (flag_intensidad == 0){
			dato= RX_Buffer;
			flag_intensidad=1;
			
			switch (dato) {
				case 'R':
					SerialPort_Send_String("Modificando el color Rojo.\r\n");
					
					while (flag_intensidad){
						// Cambiar el color del LED según el valor del sensor de luz
						intensidad = ADC_GetResult();
						red = intensidad;
						green = 0;
						blue = 0;
						// Generar señales PWM para los LEDs con el color seleccionado
						LED_Update(red, green, blue);
						_delay_ms(100);
					}
				
					break;
				
				case 'G':
				
					SerialPort_Send_String("Modificando el color Verde.\r\n");
					// Enviar confirmación por UART
					
					while (flag_intensidad){
						// Cambiar el color del LED según el valor del sensor de luz
						intensidad = ADC_GetResult();
						red = 0;
						green = intensidad;
						blue = 0;
						// Generar señales PWM para los LEDs con el color seleccionado
						LED_Update(red, green, blue);
						_delay_ms(100);
					}
				
					break;
				
				case 'B':
					SerialPort_Send_String("Modificando el color Azul.\r\n");
					
					while (flag_intensidad){
						// Cambiar el color del LED según el valor del sensor de luz
						intensidad = ADC_GetResult();
						red = 0;
						green = 0;
						blue = intensidad;
						// Generar señales PWM para los LEDs con el color seleccionado
						LED_Update(red, green, blue);
						_delay_ms(100);
					}
					break;
			}
			
		}
		 
		 
    }
	return 0;
}

ISR(USART_RX_vect){
	RX_Buffer = UDR0; //la lectura del UDR borra flag RXC
	flag_intensidad=0;
}


