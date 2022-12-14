#include "platform_internals.h"
#include "platform_test.h"
#include "evm5515.h"

#define AIC3206_I2C_ADDR 0x18

#define Rcv 0x08
#define Xmit 0x20

extern CSL_I2sHandle   hI2s;

TEST_STATUS initialize_codec(void);
TEST_STATUS initialize_codec_micro(void);
TEST_STATUS initialise_i2s_interface(void);
void I2S_readLeft(Int16* data);
void I2S_writeLeft(Int16 data);
void I2S_readRight(Int16* data);
void I2S_writeRight(Int16 data);
TEST_STATUS initialise_i2c_interface(void *testArgs);
TEST_STATUS AIC3206_write(Uint16 regnum, Uint16 regval);
