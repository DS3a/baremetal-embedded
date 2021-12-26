#![no_std]
#![no_main]

use cortex_m_rt::entry;
pub use stm32f3_discovery::{stm32f3xx_hal};
use core::ptr;

/*
        TASKS
    *enable portA in RCC

    *set PA5 as GPIO Output

    *use ODR to play with the gpio pins
*/

const PERIPH_BASE: u32 = 0x4000_0000; // the base register for all the peripherals

const AHB2_OFFSET: u32 = 0x0800_0000; // offset for AHB2 bus
const AHB2_BASE: u32 = (PERIPH_BASE + AHB2_OFFSET);
// USED TO play with portA

const AHB1_OFFSET: u32 = 0x2_0000;
const AHB1_BASE: u32 = (PERIPH_BASE + AHB1_OFFSET);
// 0x4002_0000

const RCC_OFFSET: u32 = 0x1000;
const RCC_BASE: u32 = (AHB1_BASE + RCC_OFFSET);

const RCC_AHBENR_OFFSET: u32 = 0x14;
const RCC_AHBENR_BASE: u32 = (RCC_BASE + RCC_AHBENR_OFFSET); //0x4002_1014
// this is to enable the portA

const IOPAEN: u32 = 1<<17;
// write this to RCC_AHBENR_BASE TO ENABLE PORTA

const GPIOA_OFFSET: u32 = 0x00;
const GPIOA_BASE: u32 = (AHB2_BASE + GPIOA_OFFSET);

const MODE_R_OFFSET: u32 = 0x00;
const GPIOA_MODE_R_BASE: u32 = (GPIOA_BASE + MODE_R_OFFSET);
/*
        To set gpioA pA5 as output, we need to set bit 10 to 1 and bit 11 to 0
        write (1<<10)&(~(1<<11)) to GPIOA_MODE_R_BASE
*/

const OD_R_OFFSET: u32 = 0x14;
const GPIOA_OD_R_BASE: u32 = (GPIOA_BASE + OD_R_OFFSET);
// pin 5 is on bit 5

#[entry]
fn main() -> ! {
    unsafe {
    let mut x: u8 = 0;
        ptr::write_volatile(RCC_AHBENR_BASE as *mut u32, *(RCC_AHBENR_BASE as *mut u32) | (IOPAEN));
        // enabling portA

        ptr::write_volatile(GPIOA_MODE_R_BASE as *mut u32, *(GPIOA_MODE_R_BASE as *mut u32) | ((1<<10)&(!(1<<11))));
        // setting pin 5 as gpio output

        loop {
            ptr::write_volatile(GPIOA_OD_R_BASE as *mut u32, *(GPIOA_OD_R_BASE as *mut u32) | 1<<5);
            // turning led on
            for i in 1..10000 {
                if x == 0 {
                    x += 1;
                } else {
                    x = 0;
                }
            } // makeshift forloop

            ptr::write_volatile(GPIOA_OD_R_BASE as *mut u32, *(GPIOA_OD_R_BASE as *mut u32) & (!(1<<5)));
            // turning led off
            for i in 1..10000 {
                if x == 0 {
                    x += 1;
                } else {
                    x = 0;
                }
            } // makeshift forloop
        }
    }
}


#[panic_handler]
fn my_panic(_info: &core::panic::PanicInfo) -> ! {
    loop {}
}
