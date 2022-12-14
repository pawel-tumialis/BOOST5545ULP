/* HWAFFT Routines ROM Addresses */
_hwafft_br = 0x00fefe9c;
_hwafft_8pts = 0x00fefeb0;
_hwafft_16pts = 0x00feff9f;
_hwafft_32pts = 0x00ff00f5;
_hwafft_64pts = 0x00ff03fe;
_hwafft_128pts = 0x00ff0593;
_hwafft_256pts = 0x00ff07a4;
_hwafft_512pts = 0x00ff09a2;
_hwafft_1024pts = 0x00ff0c1c;

-stack 		0x1000   /* PRIMARY STACK SIZE    */
-sysstack	0x1000   /* SECONDARY STACK SIZE  */
-heap       0x3F80   /* HEAP AREA SIZE        */  

MEMORY
{
    MMR     (RW) : origin = 0000000h length = 0000C0h /* MMRs */
    VEC     (RX) : origin = 00000C0h length = 000300h /* on-chip ROM vectors */
    DARAM   (RW) : origin = 0000400h length = 00FBFFh /* on-chip DARAM  */
    SARAM   (RW) : origin = 0010000h length = 03E000h /* on-chip SARAM  */
}
 
SECTIONS
{
     vectors(NOLOAD)
     vector     : > VEC    ALIGN = 256
    .text       : > SARAM  ALIGN = 4
    .stack      : > SARAM  ALIGN = 4
    .sysstack   : > SARAM  ALIGN = 4
    .data       : > SARAM
    .bss        : > SARAM, fill = 0
	.cinit 		: > SARAM
	.const 		: > SARAM
	.sysmem 	: > SARAM
	.cio    	: > SARAM
	.switch     : > SARAM
	.buffer1   	: > SARAM
	.buffer2   	: > SARAM
	.data_br_buf : > SARAM   			  /* ADDR = 0x0040000, Aligned to addr with 12 least-sig zeros */
}
