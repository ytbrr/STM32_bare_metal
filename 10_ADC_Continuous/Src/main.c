#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "ADC.h"

#define GPIOAEN 					(1U << 0)
#define GPIOA_5						(1U << 5)
#define LED_PIN						GPIOA_5

uint32_t sensor_value;

int main(void)
{

	uart2_tx_init();
	pa1_adc_init();
	start_conversion();

	while(1)
	{
		sensor_value = adc_read();
		printf("sensor value: %d\n\r" , (int)sensor_value);
	}
}


