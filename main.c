#include "codec_3206.h"
#include "led.h"
#include "button.h"
#include "oled.h"

int main(void)
{
	Int16 status;

	/* Initialize the platform */
	status = initPlatform();
	if(status != Platform_EOK)
	{
		printf("Systen_init Failed\n\r");
		return (-1);
	}

	/* Initialize the codec */
	status = initialize_codec();
	if(status != 0)
	{
        printf("Codec_init Failed\n\r");
        return (-1);
	}

	/* Initialize GPIO */
    CSL_GpioObj           GpioObj;
    GPIO_Handle           hGPIO;
    hGPIO = GPIO_open(&GpioObj,&status);
    if((NULL == hGPIO) || (CSL_SOK != status))
    {
        C55x_msgWrite("GPIO_open failed\n\r");
        return (-1);
    }
	GPIO_reset(hGPIO);

    /* Initialize LEDs */
	status = initialize_led(hGPIO);

    /* Initialize BUTTONs */
    status = initialize_button(hGPIO);

    /* Initialize OLED */
    status = initialize_oled(hGPIO);

    status = setline(0);
    status = setOrientation(1);
    status = printstr("DZIALA :-)");
    status = setline(1);
    status = setOrientation(1);
    status = printstr("RUSZAMY SIE");
    status = scrollDisplayLeft();
/*
	status = GPIO_write(hGPIO, LED0, 1);
	C55x_delay_msec(2000);
    status = GPIO_write(hGPIO, LED0, 0);
    C55x_delay_msec(2000);
    status = GPIO_write(hGPIO, LED1, 1);
    C55x_delay_msec(2000);
    status = GPIO_write(hGPIO, LED1, 0);
    C55x_delay_msec(2000);
    status = GPIO_write(hGPIO, LED1, 1);
    C55x_delay_msec(2000);
    status = GPIO_write(hGPIO, LED1, 0);
    C55x_delay_msec(2000);
    status = GPIO_write(hGPIO, LED1, 1);
    C55x_delay_msec(2000);
    status = GPIO_write(hGPIO, LED1, 0);
    C55x_delay_msec(2000);
*/

    //Int16 msec;
    //Int16 sample, data1, data2;
    Uint16   readBuffer;

    while(1){
        /*for ( msec = 0 ; msec < 5000 ; msec++ )
        {
            for ( sample = 0 ; sample < 48 ; sample++ )
            {
                I2S_readLeft(&data1);
                I2S_readRight(&data2);
                I2S_writeLeft(data1);
                I2S_writeRight(data2);
            }

        }
*/
        GPIO_read(hGPIO,BUTTON1,&readBuffer);
        if(readBuffer){
            status = GPIO_write(hGPIO, LED1, 1);
        }else{
            status = GPIO_write(hGPIO, LED1, 0);
        }
    }

	return (0);

}
