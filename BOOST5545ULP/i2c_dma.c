#include "i2c_dma.h"

//--------parameters----------
//#define I2C_DMA_ADDRESS 0x78
CSL_DMA_Handle      dmaHandle_i2cRead;
CSL_DMA_Handle      dmaHandle_i2cWrite;
CSL_DMA_Config      dmaConfig_i2c;
CSL_DMA_ChannelObj  dmaObj_i2cWrite;
CSL_DMA_ChannelObj  dmaObj_i2cRead;
CSL_Status        status_dma;
CSL_I2cSetup     setup_i2c;
CSL_I2cConfig    config_i2c;

extern pI2cHandle    i2cHandle;

CSL_DMA_Handle i2c_dma_init_dma_write(Uint16 i2c_address, Uint16 data_size, Uint16 *data_in)
{
    //-------------initialize dma-----------------------
    status_dma = DMA_init();
    // configure dma channel for WRITE (DMA2 CH0)
    dmaConfig_i2c.pingPongMode = CSL_DMA_PING_PONG_DISABLE;
    dmaConfig_i2c.autoMode     = CSL_DMA_AUTORELOAD_ENABLE;
    dmaConfig_i2c.burstLen     = CSL_DMA_TXBURST_1WORD;
    dmaConfig_i2c.trigger      = CSL_DMA_EVENT_TRIGGER;
    dmaConfig_i2c.dmaEvt       = CSL_DMA_EVT_I2C_TX;
    dmaConfig_i2c.dmaInt       = CSL_DMA_INTERRUPT_DISABLE;
    dmaConfig_i2c.chanDir      = CSL_DMA_WRITE;
    dmaConfig_i2c.trfType      = CSL_DMA_TRANSFER_IO_MEMORY;
    dmaConfig_i2c.dataLen      = data_size;
    dmaConfig_i2c.srcAddr      = (Uint32)data_in;
    dmaConfig_i2c.destAddr     = (Uint32)&(i2cHandle->i2cRegs->ICDXR);

    dmaHandle_i2cWrite = DMA_open((CSL_DMAChanNum)8, &dmaObj_i2cWrite, &status_dma);
    status_dma = DMA_config(dmaHandle_i2cWrite, &dmaConfig_i2c);

    // start dma channel
    //dmaHandle_i2cWrite->dmaRegs->DMACH8TCR2 |= 0x8004;
    status_dma = DMA_start(dmaHandle_i2cWrite);

    //------------unlock clock for peripherals---------
    CSL_SYSCTRL_REGS->PCGCR1 = 0x0000;
    CSL_SYSCTRL_REGS->PCGCR2 = 0x0000;
    CSL_FINST(CSL_SYSCTRL_REGS->EBSR, SYS_EBSR_PPMODE, MODE6);
    //-------------initialize i2c-----------------------
    status_dma = I2C_init(CSL_I2C0);
    config_i2c.icoar  = CSL_I2C_ICOAR_DEFVAL;
    config_i2c.icimr  = 0x0000;
    config_i2c.icclkl = CSL_I2C_ICCLK_DEFVAL;
    config_i2c.icclkh = CSL_I2C_ICCLK_DEFVAL;
    config_i2c.iccnt  = data_size;
    config_i2c.icsar  = i2c_address;
    config_i2c.icmdr  = 0xE6A0;
    config_i2c.icemdr = CSL_I2C_ICEMDR_DEFVAL;
    config_i2c.icpsc  = CSL_I2C_ICPSC_DEFVAL;
    status_dma = I2C_setup(&setup_i2c);
    CSL_I2C_SETSTART();

    return (dmaHandle_i2cWrite);

}

CSL_DMA_Handle i2c_dma_init_dma_read(Uint16 i2c_address, Uint16 data_size, Uint16 *data_out)
{
    //-------------initialize dma-----------------------
    status_dma = DMA_init();
    // configure dma channel for READ (DMA2 CH1)
    dmaConfig_i2c.pingPongMode = CSL_DMA_PING_PONG_DISABLE;
    dmaConfig_i2c.autoMode     = CSL_DMA_AUTORELOAD_ENABLE;
    dmaConfig_i2c.burstLen     = CSL_DMA_TXBURST_1WORD;
    dmaConfig_i2c.trigger      = CSL_DMA_EVENT_TRIGGER;
    dmaConfig_i2c.dmaEvt       = CSL_DMA_EVT_I2C_RX;
    dmaConfig_i2c.dmaInt       = CSL_DMA_INTERRUPT_DISABLE;
    dmaConfig_i2c.chanDir      = CSL_DMA_READ;
    dmaConfig_i2c.trfType      = CSL_DMA_TRANSFER_IO_MEMORY;
    dmaConfig_i2c.dataLen      = data_size;
    dmaConfig_i2c.srcAddr      = (Uint32)&(i2cHandle->i2cRegs->ICDRR);
    dmaConfig_i2c.destAddr     = (Uint32)data_out;

    dmaHandle_i2cRead = DMA_open((CSL_DMAChanNum)9, &dmaObj_i2cRead, &status_dma);
    status_dma = DMA_config(dmaHandle_i2cRead, &dmaConfig_i2c);

    // start dma channel
    //dmaHandle_i2cRead->dmaRegs->DMACH9TCR2 |= 0x8004;
    status_dma = DMA_start(dmaHandle_i2cRead);
    return (dmaHandle_i2cRead);
}

TEST_STATUS i2c_dma_init_i2c(void)
{
    CSL_SYSCTRL_REGS->PCGCR1 = 0x0000;
    CSL_SYSCTRL_REGS->PCGCR2 = 0x0000;
    CSL_FINST(CSL_SYSCTRL_REGS->EBSR, SYS_EBSR_PPMODE, MODE6);

    //CSL_Status        status_dma;
    CSL_I2cSetup     i2cSetup;
    status_dma = I2C_init(CSL_I2C0);
    i2cSetup.addrMode    = CSL_I2C_ADDR_7BIT;
    i2cSetup.bitCount    = CSL_I2C_BC_8BITS;
    i2cSetup.loopBack    = CSL_I2C_LOOPBACK_DISABLE;
    i2cSetup.freeMode    = CSL_I2C_FREEMODE_DISABLE;
    i2cSetup.repeatMode  = CSL_I2C_REPEATMODE_DISABLE;
    i2cSetup.ownAddr     = CSL_I2C_ICOAR_DEFVAL;
    i2cSetup.sysInputClk = 100;
    i2cSetup.i2cBusFreq  = 100;
    status_dma = I2C_setup(&i2cSetup);
    CSL_I2C_SETSTART();
    return status_dma;
}

