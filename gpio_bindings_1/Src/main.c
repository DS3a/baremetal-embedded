#include "stm32f303xe.h"

#define GPIOAEN 		(1U << 17)
//  to set in RCC
#define PIN5 		   (1U << 5)
#define LED_PIN		 PIN5

int main(void) {

	RCC->AHBENR |= GPIOAEN;

	GPIOA->MODER |= ((1U<<10) & (~(1U<<11)));
	while(1) {

		/*
		GPIOA->ODR ^= LED_PIN;
		for (int i=0; i<100000;i++) {}
		*/

		/* It's better to use BSRR instead.
		 *  as we don't need to read it,
		 *  and then modify and then reset it
		 */
		GPIOA->BSRR |= LED_PIN;
		for (int i=0; i<100000;i++) {}

		GPIOA->BSRR |= (LED_PIN<<16);
		for (int i=0; i<100000;i++) {}
	}
}
