/*
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/


//#include <stm32f4xx.h>
// Board LED is bit 5 of port A.
//#define LED_PIN 5
//#define LED_ON() GPIOA->BSRRL |= (1 << 5)
//#define LED_OFF() GPIOA->BSRRH |= (1 << 5)
//#define LED_TOGGLE() GPIOA->ODR ^= (1 << 5)
//
//#define MPU9150_WHO_AM_I           0x75   // R

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/usart.h>

#include "clock.h"
#include "uart_handler.h"

/* for USB to work, we need a USB clock of 48MHz */
const clock_scale_t hsi_8mhz_3v3 = {
	 /* 84MHz */
		16, //.pllm
		336, //.plln
		4, //.pllp
		7, //.pllq
		FLASH_ACR_ICE | FLASH_ACR_DCE |	FLASH_ACR_LATENCY_3WS, //.flash_config
		RCC_CFGR_HPRE_DIV_NONE, //.hpre
		RCC_CFGR_PPRE_DIV_4, //.ppre1
		RCC_CFGR_PPRE_DIV_2, //.ppre2
		1, //.power_save
		(uint32_t)21000000, //.apb1_frequency
		(uint32_t)42000000, //.apb2_frequency
};

//
//int i2c_setup()
//{
//
////	i2c_peripheral_disable(I2C1);
//
//	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO8 | GPIO9);
//	gpio_set_af(GPIOB, GPIO_AF4, GPIO8 | GPIO9);
//	gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, GPIO8 | GPIO9);
//
//	/* i2c peripheral runs on APB1, and we set the APB1 frequency to 21MHz in clock_setup() */
//	i2c_set_clock_frequency(I2C1, I2C_CR2_FREQ_21MHZ);
//
//	i2c_set_fast_mode(I2C1);
//
//	/* we would like to have 400kHz, that is 21Mhz/400kHz=52.5. Closest: 53, gives 396226Hz~400kHz i2c clock */
//	i2c_set_ccr(I2C1, 30);
//
//
//	i2c_peripheral_enable(I2C1);
//
//
//
//}




static void gpio_setup(void)
{
	/* gpio setup for blink LED */
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5);

	/* Setup GPIO pins for USART2 transmit. */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2 | GPIO3);

	/* Setup USART2 TX pin as alternate function. */
	gpio_set_af(GPIOA, GPIO_AF7, GPIO2 | GPIO3);



}

Clock_Manager Clk_Mgr;
Uart_Handler Uart_Mgr;


int main() {

	int i = 0;
	int j = 0;
	int c = 0;

	uint16_t temperature = 0;

	Clk_Mgr.setup(&hsi_8mhz_3v3);
	Uart_Mgr.setup();

//	i2c_setup();
	gpio_setup();


	while(1)
	{	
		/* Using API function gpio_toggle(): */
		gpio_toggle(GPIOA, GPIO5);	/* LED on/off */

//		usart_send_blocking(USART2, c + '0'); /* USART2: Send byte. */
//		c = (c == 9) ? 0 : c + 1;	/* Increment c. */
//		if ((j++ % 80) == 0) {		/* Newline after line full. */
//			usart_send_blocking(USART2, '\r');
//			usart_send_blocking(USART2, '\n');
//		}
//
//		i2c_peripheral_enable(I2C1);
//
//		i2c_send_start(I2C1);
//
//
//		/* Waiting for START is send and switched to master mode. */
//		while (!((I2C_SR1(I2C1) & I2C_SR1_SB)
//			& (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY))));
//
//		i2c_send_7bit_address(I2C1, 0x68, I2C_WRITE);
//
//
//		/* Waiting for address is transferred. */
//		while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR));
//
//		I2C_SR2(I2C1);
//
//
//		i2c_send_data(I2C1, 0x6B); /* temperature register */
//
//		while (!(I2C_SR1(I2C1) & (I2C_SR1_BTF | I2C_SR1_TxE)));
//
//		/* Send START condition. */
//		i2c_send_start(I2C1);
//
//		/* Waiting for START is send and switched to master mode. */
//		while (!((I2C_SR1(I2C1) & I2C_SR1_SB)
//			& (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY))));
//
//		/* Say to what address we want to talk to. */
//		i2c_send_7bit_address(I2C1, 0x68, I2C_READ);
//
//		/* Waiting for address is transferred. */
//		while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR));
//
//		I2C_SR2(I2C1);
//
//		/* Cleaning I2C_SR1_ACK. */
//		I2C_CR1(I2C1) &= ~I2C_CR1_ACK;
//
////		i2c_get_data()
//
//		/* Now the slave should begin to send us the first byte. Await BTF. */
////		while (!(I2C_SR1(I2C1) & I2C_SR1_BTF))
////		{
//		temperature = (uint8_t)(I2C_DR(I2C1) << 8); /* MSB */
//
////		}
//
//		i2c_peripheral_disable(I2C1);

		temperature = Uart_Mgr.get_rx_data();

		usart_send_blocking(USART2, temperature);
		usart_send_blocking(USART2, '\r');
		usart_send_blocking(USART2, '\n');


	}


}