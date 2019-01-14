#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>

#include "gpio.h"
#include "pwm.h"


void pwm_run(uint32_t divi, uint32_t divf, uint32_t range, uint32_t data, int delay_sec)
	{
	// disable pwm
	pwm_disable(1);

	// set pwm clock
	pwm_setclk(divi, divf);

	// channel 1
	pwm_range(1, range);
	pwm_data(1, data);

	// enable pwm
	pwm_enable(1);
	printf("pwm_ctl 2a: 0x%08lx\n", (long)PWM_REG_CTL);

	// delay
	struct timespec delay;
	delay.tv_sec = delay_sec;
	nanosleep(&delay, NULL);

	// disable pwm
	pwm_disable(1);
	}


// test 
int main(void)
	{
	// initialize gpio
	if (gpio_init() < 0)
		{
		printf("pwm: gpio init failure\n");
		return 1;
		}

	// initialize pwm
	if (pwm_init() < 0)
		{
		printf("pwm: pwm init failure\n");
		return 1;
		}

	// disable pwm channels
	pwm_disable(1);
	pwm_disable(2);

	// save and initialize pins
	uint8_t pin_1 = 18; // channel 1
	uint8_t fsel_save_1 = gpio_fsel_get(pin_1);
	uint8_t level_save_1 = gpio_level(pin_1);
	gpio_fsel_set(pin_1, GPIO_FSEL_ALT5);

	uint8_t pin_2 = 19; // channel 2
	uint8_t fsel_save_2 = gpio_fsel_get(pin_2);
	uint8_t level_save_2 = gpio_level(pin_2);
	gpio_fsel_set(pin_2, GPIO_FSEL_ALT5);


	// set up PWM channels
printf("pwm_ctl a: 0x%08lx\n", (long)PWM_REG_CTL);
	// set channel 1: M/S, use data reg, pwm mode
	pwm_control_set(((PWM_REG_CTL & ~(PWM_CTL_MSEN1_MASK | PWM_CTL_USEF1_MASK | PWM_CTL_MODE1_MASK))
					| PWM_CTL_MSEN1_MS | PWM_CTL_USEF1_DATA | PWM_CTL_MODE1_PWM));
	// set channel 2: M/S, use data reg, pwm mode
	pwm_control_set(((PWM_REG_CTL & ~(PWM_CTL_MSEN2_MASK | PWM_CTL_USEF2_MASK | PWM_CTL_MODE2_MASK))
					| PWM_CTL_MSEN2_MS | PWM_CTL_USEF2_DATA | PWM_CTL_MODE2_PWM));

printf("pwm_ctl b: 0x%08lx\n", (long)PWM_REG_CTL);


	unsigned int div1 = 20;
	unsigned int rng1 = 819;
	unsigned int rng2 = 2048;

	pwm_run(div1, 0, rng1, rng1/2, 3);
	pwm_run(div1, 0, rng2, rng2/2, 3);
	pwm_run(div1, 1024, rng2, rng2/2, 1);
	pwm_run(div1, 2048, rng2, rng2/2, 1);
	pwm_run(div1, 3072, rng2, rng2/2, 1);

printf("pwm_ctl c: 0x%08lx\n", (long)PWM_REG_CTL);

	// disable pwm channels
	pwm_disable(1);
	pwm_disable(2);


	// restore pins
	gpio_fsel_set(pin_1, fsel_save_1);
	level_save_1 ? gpio_set(pin_1) : gpio_clear(pin_1);
	gpio_fsel_set(pin_2, fsel_save_2);
	level_save_2 ? gpio_set(pin_2) : gpio_clear(pin_2);

	return 0;
	}
