#include <led.h>


TEST_STATUS initialize_led(GPIO_Handle gpioObject)
{
	CSL_Status           status;
	CSL_GpioPinConfig    config;

	/* Enable clocks to all peripherals */
	CSL_SYSCTRL_REGS->PCGCR1 = 0x0000;
	CSL_SYSCTRL_REGS->PCGCR2 = 0x0000;
	CSL_FINST(CSL_SYSCTRL_REGS->EBSR, SYS_EBSR_PPMODE, MODE6);

	/* Configure GPIO pin 16 as output pin */
	config.pinNum    = LED0;
	config.direction = CSL_GPIO_DIR_OUTPUT;
	config.trigger   = CSL_GPIO_TRIG_CLEAR_EDGE;

	status = GPIO_configBit(gpioObject, &config);
	if(CSL_SOK != status)
	{
		C55x_msgWrite("GPIO_configBit failed\n\r");
	    return (TEST_FAIL);
	}

	/* Configure GPIO pin 17 as output pin */
	config.pinNum    = LED1;
	config.direction = CSL_GPIO_DIR_OUTPUT;
	config.trigger   = CSL_GPIO_TRIG_CLEAR_EDGE;

	status = GPIO_configBit(gpioObject, &config);
	if(CSL_SOK != status)
	{
		C55x_msgWrite("GPIO_configBit failed\n\r");
	    return (TEST_FAIL);
	}

	/* Configure GPIO pin 28 as output pin */
	config.pinNum    = LED2;
	config.direction = CSL_GPIO_DIR_OUTPUT;
	config.trigger   = CSL_GPIO_TRIG_CLEAR_EDGE;

	status = GPIO_configBit(gpioObject, &config);
	if(CSL_SOK != status)
	{
		C55x_msgWrite("GPIO_configBit failed\n\r");
	    return (TEST_FAIL);
	}
    status = GPIO_write(gpioObject, LED2, 0);
    status = GPIO_write(gpioObject, LED0, 0);
    status = GPIO_write(gpioObject, LED1, 0);
    return (TEST_PASS);

}
