#include "platform_internals.h"
#include "platform_test.h"

TEST_STATUS i2c_dma_init_i2c(void);
CSL_DMA_Handle i2c_dma_init_dma_read(Uint16 i2c_address, Uint16 data_size, Uint16 *data_out);
CSL_DMA_Handle i2c_dma_init_dma_write(Uint16 i2c_address, Uint16 data_size, Uint16 *data_in);
