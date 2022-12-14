#include "codec_3206.h"

CSL_I2sHandle   hI2s = 0;

/**
 *
 * \brief This function used to Enable and initalize the I2S module
 *
 * \param void
 *
 * \return
 * \n      TEST_PASS  - Test Passed
 * \n      TEST_FAIL  - Test Failed
 *
 */
TEST_STATUS initialise_i2s_interface(void)
{
    I2S_Config      hwConfig;
    Int16           result = 0;

    /* Open the device with instance 2 */
    hI2s = I2S_open(I2S_INSTANCE2, DMA_POLLED, I2S_CHAN_STEREO); //I2S_POLLED

    /* Set the value for the configure structure */
    hwConfig.dataFormat     = I2S_DATAFORMAT_LJUST;
    hwConfig.dataType       = I2S_STEREO_ENABLE;
    hwConfig.loopBackMode   = I2S_LOOPBACK_DISABLE;
    hwConfig.fsPol          = I2S_FSPOL_LOW;
    hwConfig.clkPol         = I2S_RISING_EDGE;
    hwConfig.datadelay      = I2S_DATADELAY_ONEBIT;
    hwConfig.datapack       = I2S_DATAPACK_ENABLE;
    hwConfig.signext        = I2S_SIGNEXT_DISABLE;
    hwConfig.wordLen        = I2S_WORDLEN_16;
    hwConfig.i2sMode        = I2S_SLAVE;
    hwConfig.clkDiv         = I2S_CLKDIV2; // don't care for slave mode
    hwConfig.fsDiv          = I2S_FSDIV32; // don't care for slave mode
    hwConfig.FError         = I2S_FSERROR_DISABLE;
    hwConfig.OuError        = I2S_OUERROR_DISABLE;
    /*
    hwConfig.dataType           = I2S_STEREO_ENABLE;
    hwConfig.loopBackMode       = I2S_LOOPBACK_DISABLE;
    hwConfig.fsPol              = I2S_FSPOL_LOW;
    hwConfig.clkPol             = I2S_RISING_EDGE;
    hwConfig.datadelay          = I2S_DATADELAY_ONEBIT;
    hwConfig.datapack           = I2S_DATAPACK_ENABLE;
    hwConfig.signext            = I2S_SIGNEXT_DISABLE;
    hwConfig.wordLen            = I2S_WORDLEN_32;
    hwConfig.i2sMode            = I2S_SLAVE;
    hwConfig.FError             = I2S_FSERROR_ENABLE;
    hwConfig.OuError            = I2S_OUERROR_ENABLE;
    */
    /* Configure hardware registers */
    result += I2S_setup(hI2s, &hwConfig);
    result += I2S_transEnable(hI2s, TRUE);

    return result;

}

/**
 *
 * \brief This function Reads incoming I2S left channel word and writes it
 *      to the location of "data".
 *
 * \param   *data - Pointer to location if I2S data destination
 *
 * \return void
 *
 */
void I2S_readLeft(Int16* data)
{
    ioport  CSL_I2sRegs   *regs;

    regs = hI2s->hwRegs;
    while((0x08 & regs->I2SINTFL) == 0);  // Wait for receive interrupt to be pending
    *data = regs->I2SRXLT1 ;              // 16 bit left channel receive audio data
}

/**
 *
 * \brief This function used to Writes I2S left channel word
 *
 * \param  data -I2S left data
 *
 * \return void
 *
 */
void I2S_writeLeft(Int16 data)
{
    ioport  CSL_I2sRegs   *regs;

    regs = hI2s->hwRegs;
    while((CSL_I2S_I2SINTFL_XMITSTFL_MASK & regs->I2SINTFL) == 0);  // Wait for transmit interrupt to be pending
    regs->I2STXLT1 = (data) ;            // 16 bit left channel transmit audio data
}

/**
 *
 * \brief This function Reads incoming I2S right channel word and writes it
 *      to the location of "data".
 *
 * \param   *data - Pointer to location if I2S data destination
 *
 * \return void
 *
 */
void I2S_readRight(Int16* data)
{
    ioport  CSL_I2sRegs   *regs;

    regs = hI2s->hwRegs;
//  while((0x08 & regs->I2SINTFL) == 0);  // Wait for receive interrupt to be pending
    *data = regs->I2SRXRT1 ;              // 16 bit left channel receive audio data
}

/**
 *
 * \brief This function used to Writes I2S right channel word
 *
 * \param  data -I2S right data
 *
 * \return void
 *
 */
void I2S_writeRight(Int16 data)
{
    ioport  CSL_I2sRegs   *regs;

    regs = hI2s->hwRegs;
//  while((CSL_I2S_I2SINTFL_XMITSTFL_MASK & regs->I2SINTFL) == 0);  // Wait for transmit interrupt to be pending
    regs->I2STXRT1 = (data) ;              // 16 bit left channel transmit audio data
}

/**
 *
 * \brief This function used to Enable and initalize the I2C module
 *         The I2C clk is set to run at 100 KHz
 *
 * \param    testArgs   [IN]   Test arguments
 *
 * \return
 * \n      TEST_PASS  - Test Passed
 * \n      TEST_FAIL  - Test Failed
 *
 */
TEST_STATUS initialise_i2c_interface(void *testArgs)
{
    CSL_Status         status;
    CSL_I2cConfig         i2cConfig;

    status = I2C_init(CSL_I2C0);

    /* Configure I2C module for write */
    i2cConfig.icoar  = CSL_I2C_ICOAR_DEFVAL;
    i2cConfig.icimr  = CSL_I2C_ICIMR_DEFVAL;
    i2cConfig.icclkl = 20;
    i2cConfig.icclkh = 20;
    i2cConfig.icsar  = CSL_I2C_ICSAR_DEFVAL;
    i2cConfig.icmdr  = CSL_I2C_ICMDR_WRITE_DEFVAL;
    i2cConfig.icemdr = CSL_I2C_ICEMDR_DEFVAL;
    i2cConfig.icpsc  = 20;

    status |= I2C_config(&i2cConfig);

    return 0;

}

/**
 *
 * \brief This function used to write into the audio codec registers
 *
 * \param testArgs  regnum - register number
 *                  regVal - register data
 *
 * \return
 * \n      TEST_PASS  - Test Passed
 * \n      TEST_FAIL  - Test Failed
 *
 */
TEST_STATUS AIC3206_write(Uint16 regnum, Uint16 regval)
{
    Int16 retVal;
    Uint16 startStop            = ((CSL_I2C_START) | (CSL_I2C_STOP));
    Uint16 cmd[2];
    cmd[0] = regnum & 0x007F;       // 7-bit Device Register
    cmd[1] = regval;                // 8-bit Register Data

    C55x_delay_msec(3);

    /* I2C Write */
    retVal = I2C_write(cmd, 2, AIC3206_I2C_ADDR,
             TRUE, startStop, CSL_I2C_MAX_TIMEOUT);
     if(retVal != 0)
     {
        C55x_msgWrite("I2C Write failed\n\r");
        //return -1;
     }

     return (0);
}

TEST_STATUS initialize_codec(void){
    Int16 retVal;

    /* Enable clocks to all peripherals */
    CSL_SYSCTRL_REGS->PCGCR1 = 0x0000;
    CSL_SYSCTRL_REGS->PCGCR2 = 0x0000;

    retVal =  SYS_setEBSR(CSL_EBSR_FIELD_PPMODE,
                          CSL_EBSR_PPMODE_1);    // Configure Parallel Port for I2S2
    retVal |= SYS_setEBSR(CSL_EBSR_FIELD_SP1MODE,
                             CSL_EBSR_SP1MODE_1);  // Serial Port mode 1 (I2S1 and GP[11:10]).
    retVal = initialise_i2c_interface(NULL);
    if(retVal != 0)
    {
        C55x_msgWrite("I2C initialisation failed\n\r");
        return (TEST_FAIL);
    }

    /* Configure AIC3206 */
    AIC3206_write( 0,  0x00 );  // Select page 0
    AIC3206_write( 1,  0x01 );  // Reset codec
    C55x_delay_msec(1);         // Wait 1ms after reset
    AIC3206_write( 0,  0x01 );  // Select page 1
    AIC3206_write( 1,  0x08 );  // Disable crude AVDD generation from DVDD
    AIC3206_write( 2,  0x01 );  // Enable Analog Blocks, use LDO power
    AIC3206_write( 123,0x05 );  // Force reference to power up in 40ms
    C55x_delay_msec(40);        // Wait at least 40ms
    AIC3206_write( 0,  0x00 );  // Select page 0

    /* PLL and Clocks config and Power Up  */
    AIC3206_write( 27, 0x0d );  // BCLK and WCLK are set as o/p; AIC3206(Master)
    AIC3206_write( 28, 0x00 );  // Data ofset = 0
    AIC3206_write( 4,  0x03 );  // PLL setting: PLLCLK <- MCLK, CODEC_CLKIN <-PLL CLK
    AIC3206_write( 6,  0x07 );  // PLL setting: J=7
    AIC3206_write( 7,  0x06 );  // PLL setting: HI_BYTE(D=1680)
    AIC3206_write( 8,  0x90 );  // PLL setting: LO_BYTE(D=1680)
    AIC3206_write( 30, 0x88 );  // For 32 bit clocks per frame in Master mode ONLY
                                // BCLK=DAC_CLK/N =(12288000/8) = 1.536MHz = 32*fs
    AIC3206_write( 5,  0x91 );  // PLL setting: Power up PLL, P=1 and R=1
    C55x_delay_msec(1);         // Wait for PLL to come up
    AIC3206_write( 13, 0x00 );  // Hi_Byte(DOSR) for DOSR = 128 decimal or 0x0080 DAC oversamppling
    AIC3206_write( 14, 0x80 );  // Lo_Byte(DOSR) for DOSR = 128 decimal or 0x0080
    AIC3206_write( 20, 0x80 );  // AOSR for AOSR = 128 decimal or 0x0080 for decimation filters 1 to 6
    AIC3206_write( 11, 0x82 );  // Power up NDAC and set NDAC value to 2
    AIC3206_write( 12, 0x87 );  // Power up MDAC and set MDAC value to 7
    AIC3206_write( 18, 0x87 );  // Power up NADC and set NADC value to 7
    AIC3206_write( 19, 0x82 );  // Power up MADC and set MADC value to 2

    /* DAC ROUTING and Power Up */
    AIC3206_write( 0,  0x01 );  // Select page 1
    AIC3206_write( 12, 0x08 );  // LDAC AFIR routed to HPL
    AIC3206_write( 13, 0x08 );  // RDAC AFIR routed to HPR
    AIC3206_write( 0,  0x00 );  // Select page 0
    AIC3206_write( 64, 0x02 );  // Left vol=right vol
    AIC3206_write( 65, 0x20 );  // Left DAC gain to 0dB VOL; Right tracks Left
    AIC3206_write( 63, 0xd4 );  // Power up left,right data paths and set channel
    AIC3206_write( 0,  0x01 );  // Select page 1
    AIC3206_write( 16, 0x00 );  // Unmute HPL , 0dB gain
    AIC3206_write( 17, 0x00 );  // Unmute HPR , 0dB gain
    AIC3206_write( 9 , 0x30 );  // Power up HPL,HPR
    C55x_delay_msec(1 );        // Wait 1 msec

    /* ADC ROUTING and Power Up */
    AIC3206_write( 0,  0x01 );  // Select page 1
    AIC3206_write( 52, 0x30 );  // STEREO 1 Jack
                                // IN2_L to LADC_P through 40 kohm
    AIC3206_write( 55, 0x33 );  // IN2_R to RADC_P through 40 kohmm
    AIC3206_write( 54, 0x03 );  // CM_1 (common mode) to LADC_M through 40 kohm
    AIC3206_write( 57, 0xc0 );  // CM_1 (common mode) to RADC_M through 40 kohm
    AIC3206_write( 59, 0x00 );  // MIC_PGA_L unmute
    AIC3206_write( 60, 0x00 );  // MIC_PGA_R unmute
    AIC3206_write( 0,  0x00 );  // Select page 0
    AIC3206_write( 81, 0xc0 );  // Powerup Left and Right ADC
    AIC3206_write( 82, 0x00 );  // Unmute Left and Right ADC
    AIC3206_write( 0,  0x00 );  // Select page 0
    C55x_delay_msec(1 );        // Wait 1 msec

    /* Initialize I2S */
    initialise_i2s_interface();

    //I2S_close(hI2s);    // Disble I2S
    //AIC3206_write( 1, 0x01 );  // Reset codec

    return 0;

}

TEST_STATUS initialize_codec_micro(void){
    Int16 retVal;

        /* Enable clocks to all peripherals */
        CSL_SYSCTRL_REGS->PCGCR1 = 0x0000;
        CSL_SYSCTRL_REGS->PCGCR2 = 0x0000;

        retVal =  SYS_setEBSR(CSL_EBSR_FIELD_PPMODE,
                              CSL_EBSR_PPMODE_1);    // Configure Parallel Port for I2S2
        retVal |= SYS_setEBSR(CSL_EBSR_FIELD_SP1MODE,
                                 CSL_EBSR_SP1MODE_1);  // Serial Port mode 1 (I2S1 and GP[11:10]).
        retVal = initialise_i2c_interface(NULL);
        if(retVal != 0)
        {
            C55x_msgWrite("I2C initialisation failed\n\r");
            return (TEST_FAIL);
        }

    /* Configure AIC3206 with microphone */
        AIC3206_write(  0, 0x00 );      // Select page 0
        AIC3206_write(  1, 0x01 );      // Reset codec
        C55x_delay_msec(1);         // Wait 1ms after reset
        AIC3206_write(  0, 0x01 );      // Select page 1
        AIC3206_write(  1, 0x08 );      // Disable crude AVDD generation from DVDD
        AIC3206_write(  2, 0x00 );      // Enable Analog Blocks

        //AIC3206_write( 123,0x05 );  // Force reference to power up in 40ms
        //C55x_delay_msec(40);        // Wait at least 40ms
        // PLL and Clocks config and Power Up
        AIC3206_write(  0, 0x00 );      // Select page 0
        AIC3206_write( 27, 0x00 );      // BCLK and WCLK is set as i/p to AIC3206(Slave)
        AIC3206_write(  4, 0x07 );      // PLL setting: PLLCLK <- BCLK and CODEC_CLKIN <-PLL CLK
        AIC3206_write(  6, 0x20 );      // PLL setting: J = 32
        AIC3206_write(  7, 0 );         // PLL setting: HI_BYTE(D)
        AIC3206_write(  8, 0 );         // PLL setting: LO_BYTE(D)
        // For 48 KHz sampling
        AIC3206_write(  5, 0x92 );      // PLL setting: Power up PLL, P=1 and R=2
        AIC3206_write( 13, 0x00 );      // Hi_Byte(DOSR) for DOSR = 128 decimal or 0x0080 DAC oversamppling
        AIC3206_write( 14, 0x80 );      // Lo_Byte(DOSR) for DOSR = 128 decimal or 0x0080
        AIC3206_write( 20, 0x80 );      // AOSR for AOSR = 128 decimal or 0x0080 for decimation filters 1 to 6
        AIC3206_write( 11, 0x84 );      // Power up NDAC and set NDAC value to 4
        AIC3206_write( 12, 0x82 );      // Power up MDAC and set MDAC value to 2
        AIC3206_write( 18, 0x84 );      // Power up NADC and set NADC value to 4
        AIC3206_write( 19, 0x82 );      // Power up MADC and set MADC value to 2
        // DAC ROUTING and Power Up
        AIC3206_write(  0, 0x01 );      // Select page 1
        AIC3206_write( 12, 0x08 );      // LDAC AFIR routed to HPL
        AIC3206_write( 13, 0x08 );      // RDAC AFIR routed to HPR
        AIC3206_write(  0, 0x00 );      // Select page 0
        AIC3206_write( 64, 0x02 );      // Left vol=right vol
        AIC3206_write( 65, 0x00 );      // Left DAC gain to 0dB VOL; Right tracks Left
        AIC3206_write( 63, 0xd4 );      // Power up left,right data paths and set channel
        AIC3206_write(  0, 0x01 );      // Select page 1
        AIC3206_write( 16, 0x06 );      // Unmute HPL , 6dB gain
        AIC3206_write( 17, 0x06 );      // Unmute HPR , 6dB gain
        AIC3206_write(  9, 0x30 );      // Power up HPL,HPR
        AIC3206_write(  0, 0x00 );      // Select page 0
        C55x_delay_msec( 5);            // Wait 5 msec
        // ADC ROUTING and Power Up
        AIC3206_write(  0, 0x01 );      // Select page 1
        AIC3206_write( 51, 0x40 );      // SetMICBIAS
        AIC3206_write( 52, 0x30 );      // STEREO 1 Jack
        // IN2_L to LADC_P through 40 kohm
        AIC3206_write( 55, 0x03 );      // IN2_R to RADC_P through 40 kohmm
        AIC3206_write( 54, 0x03 );      // CM_1 (common mode) to LADC_M through 40 kohm
        AIC3206_write( 57, 0xc0 );      // CM_1 (common mode) to RADC_M through 40 kohm
        AIC3206_write( 59, 0x5f );      // MIC_PGA_L unmute
        AIC3206_write( 60, 0x5f );      // MIC_PGA_R unmute
        AIC3206_write(  0, 0x00 );      // Select page 0
        AIC3206_write( 81, 0xc0 );      // Powerup Left and Right ADC
        AIC3206_write( 82, 0x00 );      // Unmute Left and Right ADC

        AIC3206_write( 0,  0x00 );
        C55x_delay_msec( 2 );           // Wait 2 msec

    /* Initialize I2S */
    //initialise_i2s_interface();
        I2S2_SRGR = 0x0015;
        I2S2_ICMR = 0x0028;    // Enable interrupts
        I2S2_CR   = 0x8012;    // 16-bit word, Master, enable I2C
    //I2S_close(hI2s);    // Disble I2S
    //AIC3206_write( 1, 0x01 );  // Reset codec

    return 0;

}


