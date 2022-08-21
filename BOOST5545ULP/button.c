#include <button.h>

TEST_STATUS initialize_button(GPIO_Handle gpioObject)
{
	Int16 retVal;
	CSL_GpioPinConfig    config;

	/* Set Bus for GPIOs */
	CSL_FINST(CSL_SYSCTRL_REGS->EBSR, SYS_EBSR_PPMODE, MODE1);

	/* GPIO_config API to make PIN12 as input pin */
	config.pinNum    = BUTTON0;
    config.direction = CSL_GPIO_DIR_INPUT;
    config.trigger   = CSL_GPIO_TRIG_RISING_EDGE;
    retVal = GPIO_configBit(gpioObject,&config);
	if(CSL_SOK != retVal)
	{
		C55x_msgWrite("test failed - GPIO_configBit\n");
	     return (TEST_FAIL);
	}

    /** test GPIO_config API to make PIN13 as i/p */
	config.pinNum    = BUTTON1;
    config.direction = CSL_GPIO_DIR_INPUT;
    config.trigger   = CSL_GPIO_TRIG_RISING_EDGE;
    retVal = GPIO_configBit(gpioObject,&config);
	if(CSL_SOK != retVal)
	{
		C55x_msgWrite("test failed - GPIO_configBit\n");
	     return (TEST_FAIL);
	}

    /** test GPIO_config API to make PIN14 as i/p */
	config.pinNum    = BUTTON2;
    config.direction = CSL_GPIO_DIR_INPUT;
    config.trigger   = CSL_GPIO_TRIG_RISING_EDGE;
    retVal = GPIO_configBit(gpioObject,&config);
	if(CSL_SOK != retVal)
	{
		C55x_msgWrite("test failed - GPIO_configBit\n");
	     return (TEST_FAIL);
	}

	return (TEST_PASS);
}
