#include "codec_3206.h"
#include "button.h"
#include "oled.h"

#define SAMPLE_NUMBER 1024
CSL_DMA_Handle      dmaHandleRxL;
CSL_DMA_Handle      dmaHandleRxR;
CSL_DMA_Handle      dmaHandleTxL;
CSL_DMA_Handle      dmaHandleTxR;
CSL_DMA_Config      dmaConfig;
CSL_DMA_Config      getdmaConfig;
CSL_DMA_ChannelObj  dmaObj;
CSL_Status          status;

Int16 samples_left[SAMPLE_NUMBER];
#pragma DATA_ALIGN(samples_left, 4);
Uint16   startFFT = 0;

/* FFT VARIABLES */
typedef long        Int32_;

#define FFT_FLAG ( 0 ) /* HWAFFT to perform FFT */
#define IFFT_FLAG ( 1 ) /* HWAFFT to perform IFFT */
#define SCALE_FLAG ( 0 ) /* HWAFFT to scale butterfly output */
#define NOSCALE_FLAG ( 1 ) /* HWAFFT not to scale butterfly output */
#define OUT_SEL_DATA ( 0 ) /* Indicates HWAFFT output located in input data vector */
#define OUT_SEL_SCRATCH ( 1 ) /* Indicates HWAFFT output located in scratch vector */
#define REAL 0
#define IMAG 1

Uint32 tab_fft[1024];
Int32 kk = 0, jj = 0;

#pragma DATA_SECTION(tab_br_buf, ".data_br_buf");  // tablica przypisana do bloku pamieci ktorego aders poczatkowy ma przynajmniej 12 zer najmniej znaczacych.
Uint32 tab_br_buf[1024];                           // Pamiec ta wydzielono w pliku linkera lnkx.cmd

Uint32 tab_scratch_buf[1024];

typedef union { Int32_ x_i32;  Uint32 x_ui32; Int16 tab_i16[2]; } data_type;
data_type dat;

Int32_ *data = (Int32_ *)tab_fft;
Int32_ *data_br = (Int32_ *)tab_br_buf;
Uint16 fft_flag;
Uint16 scale_flag;
Int32_ *scratch = (Int32_ *)tab_scratch_buf;
Uint16 out_sel;
Int32_ *result;
Int16 *result_16;
Int32 pow[512];
Int16 pow_fl[512];

Int16 real, imag;

void hwafft_br(Int32_ *data, Int32_ *data_br, Uint16 data_len);                            // funkcje przypisane do adresow w ROM
Uint16 hwafft_1024pts(Int32_ *data, Int32_ *scratch, Uint16 fft_flag, Uint16 scale_flag);  // na koncu pliku linkera lnkx.cmd

extern void VECSTART(void);
interrupt void dma_isr(void);
TEST_STATUS initialize_dma(void);

int main(void)
{
    // Initialize the platform
    status = initPlatform();
    // Initialize DMA
    status = initialize_dma();
    // Initialize the codec
    status = initialize_codec();
    /* Initialize GPIO */
    CSL_GpioObj           GpioObj;
    GPIO_Handle           hGPIO;
    hGPIO = GPIO_open(&GpioObj,&status);
    GPIO_reset(hGPIO);
    /* Initialize BUTTONs */
    status = initialize_button(hGPIO);
     /* Initialize OLED */
    status = initialize_oled(hGPIO);
    status = setline(0);
    status = setOrientation(1);
    status = printstr("HARDWARE FFT");
    status = setline(1);
    status = setOrientation(1);
    status = printstr("MIDDLE BUTTON");

    Uint16   readButton;
    while(1){
        GPIO_read(hGPIO,BUTTON1,&readButton);
        if(readButton == 0){
            if(startFFT == 0){
                startFFT = 1;
            }
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
        dmaConfig.srcAddr      = (Uint32)samples_left;
        dmaConfig.destAddr       = (Uint32)&CSL_I2S2_REGS->I2STXRT0;

        dmaHandleTxR = DMA_open((CSL_DMAChanNum)5,&dmaObj, &status);
        status = DMA_config(dmaHandleTxR, &dmaConfig);

        //start channel with sync
        dmaHandleRxL->dmaRegs->DMACH2TCR2 |= 0x8004;
        dmaHandleTxL->dmaRegs->DMACH0TCR2 |= 0x8004;
        dmaHandleTxR->dmaRegs->DMACH1TCR2 |= 0x8004;
    return (status);
}

interrupt void dma_isr(void)
{
    int ifrValue;

    ifrValue = CSL_SYSCTRL_REGS->DMAIFR;

    // Check for DMA1 TXL transfer completion
    if (ifrValue & (1<<4))
    {
        if(startFFT == 1){
            // HARDWARE FFT
            for (jj=0; jj<1024; jj++)   {   //  kopiowanie danych wejsciowych
                        tab_fft[jj] = 0;  // wyzerowanie czesci realis i imaginalis
                        tab_fft[jj] = (Uint32)samples_left[jj] << 16; // wczytanie probki do czesci realis
                    }
                    fft_flag = FFT_FLAG;
                     scale_flag = NOSCALE_FLAG;   // niebezpieczne!!!, ale konieczne dla sygnalow o malym poziomie!!!
                    //scale_flag = SCALE_FLAG;    // zawsze bezpieczne, ale dla sygnalow o malym poziomie amplitudy prazkow moga byc male i ginac w szumie szerokopasmowym

                    /* Bit-Reverse 1024-point data, Store into data_br, data_br aligned to 12-least significant binary zeros*/
                    hwafft_br(data, data_br, 1024); /* bit-reverse input data,

                    Destination buffer aligned */
                    data = data_br;

                    /* Compute 1024-point FFT, scaling enabled. */
                    out_sel = hwafft_1024pts(data, scratch, fft_flag, scale_flag);

                    if (out_sel == OUT_SEL_DATA) {  result = data;  }  // przypisanie do wskaznika result wskaznika bufora wyjsciowego zawierajacego wyniki obliczen
                    else {  result = scratch;  }

                    for (jj=0; jj<512; jj++)   {  // obliczenie kwadratu modulu sygnalu zespolonego widma
                        real = (Int16)(result[jj]>>16);  imag = (Int16)(result[jj] & 0x0000FFFF);
                        pow_fl[jj] = (real)*(real) + (imag)*(imag);
                    }

            // FFT READY
            status = clear();
            writeFFT(pow_fl, 512);
            startFFT = 0;
        }
    }
    CSL_SYSCTRL_REGS->DMAIFR |= ifrValue;
}
