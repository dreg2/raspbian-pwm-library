#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include <sys/mman.h>
#include <time.h>

#include "gpio.h"
#include "pwm.h"

// base pointers
volatile uint32_t *clk_base = MAP_FAILED;
volatile uint32_t *pwm_base = MAP_FAILED;

// delay values for nanosleep calls
struct timespec delay_1_mic   = {0, 1   * 1000}; // 1 microsecond
struct timespec delay_10_mic  = {0, 10  * 1000}; // 10 microseconds
struct timespec delay_110_mic = {0, 110 * 1000}; // 110 microseconds

//----------------------------------------------------------------------------------------------------
// initialize pwm
//----------------------------------------------------------------------------------------------------
int8_t pwm_init(void)
	{
	// open memory device
	int fd;
	if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0)
		{
		perror("PWM: open /dev/mem failed:");
		return -1;
		}

	// map register memory to base pointers
	pwm_base = (uint32_t *)mmap(NULL, (size_t)getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, fd, (PERI_BASE + PWM_REG_OFFSET));
	clk_base = (uint32_t *)mmap(NULL, (size_t)getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, fd, (PERI_BASE + CM_PWM_REG_OFFSET));
	close(fd);

	if (pwm_base == MAP_FAILED)
		{
		perror("PWM: pwm mmap failed:");
		return -1;
		}

	if (clk_base == MAP_FAILED)
		{
		perror("PWM: clk mmap failed:");
		return -1;
		}

	return 0;
	}

//----------------------------------------------------------------------------------------------------
// get pwm control register
//----------------------------------------------------------------------------------------------------
uint32_t pwm_control_get(void)
	{
	return PWM_REG_CTL;
	}

//----------------------------------------------------------------------------------------------------
// set pwm control register
//----------------------------------------------------------------------------------------------------
void pwm_control_set(uint32_t value)
	{
	PWM_REG_CTL = value;
	nanosleep(&delay_10_mic, NULL);
	}

//----------------------------------------------------------------------------------------------------
// enable pwm channel
//----------------------------------------------------------------------------------------------------
void pwm_enable(unsigned int channel)
	{
	// enable PWM channels
	if (channel == 1)
		PWM_REG_CTL = ((PWM_REG_CTL & ~PWM_CTL_PWEN1_MASK) | PWM_CTL_PWEN1_ENA);
	else if (channel == 2)
		PWM_REG_CTL = ((PWM_REG_CTL & ~PWM_CTL_PWEN2_MASK) | PWM_CTL_PWEN2_ENA);
	nanosleep(&delay_10_mic, NULL);
	}

//----------------------------------------------------------------------------------------------------
// disable pwm channel
//----------------------------------------------------------------------------------------------------
void pwm_disable(unsigned int channel)
	{
	// enable PWM channels
	if (channel == 1)
		PWM_REG_CTL = ((PWM_REG_CTL & ~PWM_CTL_PWEN1_MASK) | PWM_CTL_PWEN1_DIS);
	else if (channel == 2)
		PWM_REG_CTL = ((PWM_REG_CTL & ~PWM_CTL_PWEN2_MASK) | PWM_CTL_PWEN2_DIS);
	nanosleep(&delay_10_mic, NULL);
	}

//----------------------------------------------------------------------------------------------------
// set pwm range register
//----------------------------------------------------------------------------------------------------
void pwm_range(unsigned int channel, uint32_t value)
	{
	if (channel == 1)
		PWM_REG_RNG1 = value;
	else if (channel == 2)
		PWM_REG_RNG2 = value;
	nanosleep(&delay_10_mic, NULL);
	}

//----------------------------------------------------------------------------------------------------
// set pwm data register
//----------------------------------------------------------------------------------------------------
void pwm_data(unsigned int channel, uint32_t value)
	{
	if (channel == 1)
		PWM_REG_DAT1 = value;
	else if (channel == 2)
		PWM_REG_DAT2 = value;
	nanosleep(&delay_10_mic, NULL);
	}

//----------------------------------------------------------------------------------------------------
// set pwm clock 
//----------------------------------------------------------------------------------------------------
void pwm_setclk(uint32_t divisor_i, uint32_t divisor_f)
	{
//	__sync_synchronize(); // hardware memory barrier

	// keep integer divisor between 2 and 4095
	if (divisor_i > 4095)
		divisor_i = 4095;
	else if (divisor_i < 2)
		divisor_i = 2;

	// keep fractional divisor between 0 and 4095
	if (divisor_i > 4095)
		divisor_i = 4095;

printf("clk_ctl 1: 0x%08lx\n", (long)CM_PWM_REG_CTL);
	// disable clock
	CM_PWM_REG_CTL = ((CM_PWM_REG_CTL & ~(CM_PWM_PSWD_MASK | CM_PWM_CTL_ENAB_MASK))
					| (CM_PWM_PSWD | CM_PWM_CTL_ENAB_DIS));

	// necessary delay to prevent clock problems in balanced mode
	nanosleep(&delay_110_mic, NULL);

	// wait for not busy
	while (CM_PWM_REG_CTL & CM_PWM_CTL_BUSY_MASK)
		{
printf(".");
		nanosleep(&delay_1_mic, NULL);
		}
printf("\n");

	// set clock divisor
	CM_PWM_REG_DIV = (CM_PWM_PSWD | (divisor_i << 12) | divisor_f);

	// set to use oscillator source
	CM_PWM_REG_CTL = ((CM_PWM_REG_CTL & ~(CM_PWM_PSWD_MASK | CM_PWM_CTL_SRCE_MASK))
					| (CM_PWM_PSWD | CM_PWM_CTL_SRCE_OSC));

	// enable clock
	CM_PWM_REG_CTL = ((CM_PWM_REG_CTL & ~(CM_PWM_PSWD_MASK | CM_PWM_CTL_ENAB_MASK))
					| (CM_PWM_PSWD | CM_PWM_CTL_ENAB_ENA));
printf("clk_ctl 2: 0x%08lx\n", (long)CM_PWM_REG_CTL);
//	__sync_synchronize(); // hardware memory barrier
	}

