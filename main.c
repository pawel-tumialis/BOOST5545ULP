#include "codec_3206.h"
#include "button.h"

#define SAMPLE_NUMBER 1024
CSL_DMA_Handle      dmaHandleRxL;
CSL_DMA_Handle      dmaHandleRxR;
CSL_DMA_Handle      dmaHandleTxL;
CSL_DMA_Handle      dmaHandleTxR;
CSL_DMA_Config      dmaConfig;
CSL_DMA_Config      getdmaConfig;
CSL_DMA_ChannelObj  dmaObj;
CSL_Status          status;

Uint16 samples_left[SAMPLE_NUMBER];
Uint16 samples_right[SAMPLE_NUMBER];
#pragma DATA_ALIGN(samples_left, 4);
#pragma DATA_ALIGN(samples_right, 4);

extern void VECSTART(void);
interrupt void dma_isr(void);
TEST_STATUS initialize_dma(void);

Uint16 RxCounter = 0;
Uint16 TxCounter = 0;

int main(void)
{
    // Initialize the platform
    status = initPlatform();    //delete for
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
    Uint16 i = 0;
    while(1){/*
                I2S_readLeft(&data1);
                I2S_readRight(&data2);
                data1 &= 0xFFEB;
                data2 &= 0xFFEB;
                I2S_writeLeft(data1);
                I2S_writeRight(data2);*/
        if(!RxCounter)
            {
                dmaHandleRxL->dmaRegs->DMACH2TCR2 |= 0x8004;
                dmaHandleRxR->dmaRegs->DMACH3TCR2 |= 0x8004;
            }
            if (RxCounter > 1)
            {
                RxCounter = 0;
                dmaHandleRxL->dmaRegs->DMACH2TCR2 &= 0x7FFF;
                dmaHandleRxR->dmaRegs->DMACH3TCR2 &= 0x7FFF;
                //-----------------------STUDENT ZONE------------------------
                for (i=0; i<SAMPLE_NUMBER; i++)
                {
                    // Insert algorithm here
                    samples_left[i]  = samples_left[i];
                    samples_right[i] = samples_right[i];
                }
            }
            if(!TxCounter)
            {
                dmaHandleTxR->dmaRegs->DMACH1TCR2 |= 0x8004;
                dmaHandleTxL->dmaRegs->DMACH0TCR2 |= 0x8004;
            }
            if(TxCounter > 1)
            {
                TxCounter = 0;
                dmaHandleTxR->dmaRegs->DMACH1TCR2 &= 0x7FFF;
                dmaHandleTxL->dmaRegs->DMACH0TCR2 &= 0x7FFF;
            }
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
        dmaConfig.destAddr       = (Uint32)(samples_left);

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
        dmaConfig.srcAddr      = (Uint32)samples_left;
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
        dmaConfig.destAddr       = (Uint32)samples_right;

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
        dmaConfig.srcAddr      = (Uint32)samples_right;
        dmaConfig.destAddr       = (Uint32)&CSL_I2S2_REGS->I2STXRT0;

        dmaHandleTxR = DMA_open((CSL_DMAChanNum)5,&dmaObj, &status);
        status = DMA_config(dmaHandleTxR, &dmaConfig);

    //start RX channels with sync
       // dmaHandleRxL->dmaRegs->DMACH2TCR2 |= 0x8004;
        //dmaHandleRxR->dmaRegs->DMACH3TCR2 |= 0x8004;
    return (status);
}

interrupt void dma_isr(void)
{
    int ifrValue;

    ifrValue = CSL_SYSCTRL_REGS->DMAIFR;
    CSL_SYSCTRL_REGS->DMAIFR |= ifrValue;

    // Check for DMA1 TXL transfer completion
    if (ifrValue & (1<<4))
    {
        TxCounter++;
    }
    // Check for DMA1 TXR transfer completion
    if (ifrValue & (1<<5))
    {
        TxCounter++;
    }

    // Check for DMA1 RXL transfer completion
    if (ifrValue & (1<<6))
    {
        RxCounter++;
    }

    // Check for DMA1 RXR transfer completion
    if (ifrValue & (1<<7))
    {
        RxCounter++;
    }
}
