#include "codec_3206.h"

#define SAMPLE_NUMBER 1024
CSL_DMA_Handle      dmaHandleRxL;
CSL_DMA_Handle      dmaHandleRxR;
CSL_DMA_Handle      dmaHandleTxL;
CSL_DMA_Handle      dmaHandleTxR;
CSL_DMA_Config      dmaConfig;
CSL_DMA_Config      getdmaConfig;
CSL_DMA_ChannelObj  dmaObj;
CSL_Status          status;

Uint16 i2sDmaReadBufLeft[SAMPLE_NUMBER];
Uint16 i2sDmaReadBufRight[SAMPLE_NUMBER];
#pragma DATA_ALIGN(i2sDmaReadBufLeft, 4);
#pragma DATA_ALIGN(i2sDmaReadBufRight, 4);

extern void VECSTART(void);
interrupt void dma_isr(void);
TEST_STATUS initialize_dma(void);

int main(void)
{
    // Initialize the platform
    status = initPlatform();
    if(status != Platform_EOK)
    {
        printf("Systen_init Failed\n\r");
        return (-1);
    }
    // Initialize DMA
    status = initialize_dma();
    if(status != 0)
    {
        printf("DMA_init Failed\n\r");
        return (-1);
    }
    // Initialize the codec
    status = initialize_codec();
    if(status != 0)
    {
        printf("Codec_init Failed\n\r");
        return (-1);
    }

    //Int16  data1, data2;  //for testing stereo
    while(1){

                //I2S_readLeft(&data1);
                //I2S_readRight(&data2);
                //I2S_writeLeft(data1);
                //I2S_writeRight(data2);
    }

    return (0);

}

TEST_STATUS initialize_dma(void){
    // Interrupt configuration
    IRQ_globalDisable();
    IRQ_clearAll();
    IRQ_disableAll();
    IRQ_setVecs((Uint32)&VECSTART);
    IRQ_clear(DMA_EVENT);
    IRQ_plug (DMA_EVENT, &dma_isr);
    IRQ_enable(DMA_EVENT);
    IRQ_globalEnable();

    status = DMA_init();
    //Config left read
    dmaConfig.pingPongMode = CSL_DMA_PING_PONG_DISABLE;
    dmaConfig.autoMode     = CSL_DMA_AUTORELOAD_ENABLE;
    dmaConfig.burstLen     = CSL_DMA_TXBURST_1WORD;
    dmaConfig.trigger      = CSL_DMA_EVENT_TRIGGER;
    dmaConfig.dmaEvt       = CSL_DMA_EVT_I2S2_RX;
    dmaConfig.dmaInt       = CSL_DMA_INTERRUPT_ENABLE;
    dmaConfig.chanDir      = CSL_DMA_READ;
    dmaConfig.trfType      = CSL_DMA_TRANSFER_IO_MEMORY;
    dmaConfig.dataLen      = 1024;
    dmaConfig.srcAddr      = (Uint32)&CSL_I2S2_REGS->I2SRXLT0;
    dmaConfig.destAddr       = (Uint32)(i2sDmaReadBufLeft);

    dmaHandleRxL = DMA_open((CSL_DMAChanNum)6,&dmaObj, &status);
    status = DMA_config(dmaHandleRxL, &dmaConfig);

    //Config left write
    dmaConfig.pingPongMode = CSL_DMA_PING_PONG_DISABLE;
    dmaConfig.autoMode     = CSL_DMA_AUTORELOAD_ENABLE;
    dmaConfig.burstLen     = CSL_DMA_TXBURST_1WORD;
    dmaConfig.trigger      = CSL_DMA_EVENT_TRIGGER;
    dmaConfig.dmaEvt       = CSL_DMA_EVT_I2S2_TX;
    dmaConfig.dmaInt       = CSL_DMA_INTERRUPT_ENABLE;
    dmaConfig.chanDir      = CSL_DMA_WRITE;
    dmaConfig.trfType      = CSL_DMA_TRANSFER_IO_MEMORY;
    dmaConfig.dataLen      = 1024;
    dmaConfig.srcAddr      = (Uint32)i2sDmaReadBufLeft;
    dmaConfig.destAddr       = (Uint32)&CSL_I2S2_REGS->I2STXLT0;

    dmaHandleTxL = DMA_open((CSL_DMAChanNum)4,&dmaObj, &status);
    status = DMA_config(dmaHandleTxL, &dmaConfig);

    //Config right read
    dmaConfig.pingPongMode = CSL_DMA_PING_PONG_DISABLE;
    dmaConfig.autoMode     = CSL_DMA_AUTORELOAD_ENABLE;
    dmaConfig.burstLen     = CSL_DMA_TXBURST_1WORD;
    dmaConfig.trigger      = CSL_DMA_EVENT_TRIGGER;
    dmaConfig.dmaEvt       = CSL_DMA_EVT_I2S2_RX;
    dmaConfig.dmaInt       = CSL_DMA_INTERRUPT_ENABLE;
    dmaConfig.chanDir      = CSL_DMA_READ;
    dmaConfig.trfType      = CSL_DMA_TRANSFER_IO_MEMORY;
    dmaConfig.dataLen      = 1024;
    dmaConfig.srcAddr      = (Uint32)&CSL_I2S2_REGS->I2SRXRT0;
    dmaConfig.destAddr       = (Uint32)i2sDmaReadBufRight;

    dmaHandleRxR = DMA_open((CSL_DMAChanNum)7,&dmaObj, &status);
    status = DMA_config(dmaHandleRxR, &dmaConfig);

    //Config right write
    dmaConfig.pingPongMode = CSL_DMA_PING_PONG_DISABLE;
    dmaConfig.autoMode     = CSL_DMA_AUTORELOAD_ENABLE;
    dmaConfig.burstLen     = CSL_DMA_TXBURST_1WORD;
    dmaConfig.trigger      = CSL_DMA_EVENT_TRIGGER;
    dmaConfig.dmaEvt       = CSL_DMA_EVT_I2S2_TX;
    dmaConfig.dmaInt       = CSL_DMA_INTERRUPT_ENABLE;
    dmaConfig.chanDir      = CSL_DMA_WRITE;
    dmaConfig.trfType      = CSL_DMA_TRANSFER_IO_MEMORY;
    dmaConfig.dataLen      = 1024;
    dmaConfig.srcAddr      = (Uint32)i2sDmaReadBufRight;
    dmaConfig.destAddr       = (Uint32)&CSL_I2S2_REGS->I2STXRT0;

    dmaHandleTxR = DMA_open((CSL_DMAChanNum)5,&dmaObj, &status);
    status = DMA_config(dmaHandleTxR, &dmaConfig);

    //start channel with sync
    dmaHandleRxL->dmaRegs->DMACH2TCR2 |= 0x8004;
    dmaHandleTxL->dmaRegs->DMACH0TCR2 |= 0x8004;
    dmaHandleRxR->dmaRegs->DMACH3TCR2 |= 0x8004;
    dmaHandleTxR->dmaRegs->DMACH1TCR2 |= 0x8004;
    return (status);
}


interrupt void dma_isr(void)
{
    int ifrValue;

    ifrValue = CSL_SYSCTRL_REGS->DMAIFR;
    CSL_SYSCTRL_REGS->DMAIFR |= ifrValue;
}
