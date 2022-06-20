// Finding the LED

// PORT : A
// PIN : 5
//

#define PERIPH_BASE (0x40000000UL)
// The base register where all the peripherals in the mcu lie

#define AHB2_OFFSET (0x08000000UL)
#define AHB2_BASE (PERIPH_BASE + AHB2_OFFSET)
// AHB is the bus used to communicate with portA

#define GPIOA_OFFSET (0x0000U)
#define GPIOA_BASE (AHB2_BASE + GPIOA_OFFSET)
// THE base register for GPIO, which is used to set the pins
// and control/read it

#define AHB1_OFFSET (0x20000UL)
#define AHB1_BASE (PERIPH_BASE + AHB1_OFFSET)

#define RCC_OFFSET (0x1000U)
#define RCC_BASE (AHB1_BASE + RCC_OFFSET)
// AHB1 has the RCC port
// RCC is used to enable buses

#define RCC_AHBENR_OFFSET (0x14UL)
#define RCC_AHBENR_BASE (*(volatile unsigned int *)(RCC_BASE + RCC_AHBENR_OFFSET))

#define IOPAEN (1U<<17)
// to ENABLE portA

#define MODE_R_OFFSET (0x00UL)
#define GPIOA_MODE_R_BASE (*(volatile unsigned int *)(GPIOA_BASE + MODE_R_OFFSET))
/*
 * to set gpioA pin 5 as output, we need to set bit 10 to 1
 * and bit 11 to 0
 * that can be done by writing (1U<<10)&(~(1<<11)) to GPIOA_MODE_R_BASE
 * */

#define OD_R_OFFSET (0x14U)
#define GPIOA_OD_R_BASE (*(volatile unsigned int *)(GPIOA_BASE + OD_R_OFFSET))
// THis is the output data pin register


// applied volatile to the registers we're going to manipulate
#define PIN5 (1U<<5)
#define LED_PIN PIN5


int main(void) {
	/*
	 * 				Tasks
	 * 1. enable portA through RCC_AHBEN_R
	 * 2. set PA5 as output
	 * 3. set PA5 as high
	 */

	// TASK 1
	RCC_AHBENR_BASE |= IOPAEN;

	// TASK 2
	GPIOA_MODE_R_BASE |= (1U<<10)&(~(1U<<11));

	while (1) {
		GPIOA_OD_R_BASE |= PIN5; // to turn the pin on
		for (int i=0; i<1000000; i++) {} // makeshift delay function
		GPIOA_OD_R_BASE &= ~PIN5; // to turn the pin off
		for (int i=0; i<1000000; i++) {}
	}
}
