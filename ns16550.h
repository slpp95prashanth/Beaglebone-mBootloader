#define UART_FCR_FIFO_EN        0x01 /* Fifo enable */
#define UART_FCR_TRIGGER_MASK   0xC0 /* Mask for the FIFO trigger range */
#define UART_FCR_TRIGGER_1      0x00 /* Mask for trigger set at 1 */
#define UART_FCR_TRIGGER_4      0x40 /* Mask for trigger set at 4 */
#define UART_FCR_TRIGGER_8      0x80 /* Mask for trigger set at 8 */
#define UART_FCR_TRIGGER_14     0xC0 /* Mask for trigger set at 14 */

#define UART_FCR_RXSR           0x02 /* Receiver soft reset */
#define UART_FCR_TXSR           0x04 /* Transmitter soft reset */

/*
 *  * These are the definitions for the Modem Control Register
 *   */
#define UART_MCR_DTR    0x01            /* DTR   */
#define UART_MCR_RTS    0x02            /* RTS   */
#define UART_MCR_OUT1   0x04            /* Out 1 */
#define UART_MCR_OUT2   0x08            /* Out 2 */
#define UART_MCR_LOOP   0x10            /* Enable loopback test mode */

#define UART_MCR_DMA_EN 0x04
#define UART_MCR_TX_DFR 0x08
#define UART_LCR_WLS_MSK 0x03           /* character length select mask */
#define UART_LCR_WLS_5  0x00            /* 5 bit character length */
#define UART_LCR_WLS_6  0x01            /* 6 bit character length */
#define UART_LCR_WLS_7  0x02            /* 7 bit character length */
#define UART_LCR_WLS_8  0x03            /* 8 bit character length */
#define UART_LCR_STB    0x04            /* # stop Bits, off=1, on=1.5 or 2) */
#define UART_LCR_PEN    0x08            /* Parity eneble */
#define UART_LCR_EPS    0x10            /* Even Parity Select */
#define UART_LCR_STKP   0x20            /* Stick Parity */
#define UART_LCR_SBRK   0x40            /* Set Break */
#define UART_LCR_BKSE   0x80            /* Bank select enable */
#define UART_LCR_DLAB   0x80            /* Divisor latch access bit */

/*
 *  * These are the definitions for the Line Status Register
 *   */
#define UART_LSR_DR     0x01            /* Data ready */
#define UART_LSR_OE     0x02            /* Overrun */
#define UART_LSR_PE     0x04            /* Parity error */
#define UART_LSR_FE     0x08            /* Framing error */
#define UART_LSR_BI     0x10            /* Break */
#define UART_LSR_THRE   0x20            /* Xmit holding register empty */
#define UART_LSR_TEMT   0x40            /* Xmitter empty */
#define UART_LSR_ERR    0x80            /* Error */

#define UART_MSR_DCD    0x80            /* Data Carrier Detect */
#define UART_MSR_RI     0x40            /* Ring Indicator */
#define UART_MSR_DSR    0x20            /* Data Set Ready */
#define UART_MSR_CTS    0x10            /* Clear to Send */
#define UART_MSR_DDCD   0x08            /* Delta DCD */
#define UART_MSR_TERI   0x04            /* Trailing edge ring indicator */
#define UART_MSR_DDSR   0x02            /* Delta DSR */
#define UART_MSR_DCTS   0x01            /* Delta CTS */
#define UART_LCR_8N1    0x03

#define UART_LCRVAL UART_LCR_8N1                /* 8 data, 1 stop, no parity */
#define UART_MCRVAL (UART_MCR_DTR | \
		UART_MCR_RTS)              /* RTS/DTR */
#define UART_FCRVAL (UART_FCR_FIFO_EN | \
		UART_FCR_RXSR |    \
		UART_FCR_TXSR)             /* Clear & enable FIFOs */

#define CONFIG_SYS_NS16550_IER  0x00

struct ns16550 {
	unsigned int rbr;          /* 0 */
	unsigned int ier;          /* 1 */
	unsigned int fcr;          /* 2 */
	unsigned int lcr;          /* 3 */
	unsigned int mcr;          /* 4 */
	unsigned int lsr;          /* 5 */
	unsigned int msr;          /* 6 */
	unsigned int spr;          /* 7 */
	unsigned int mdr1;         /* 8 */
	unsigned int reg9;         /* 9 */
	unsigned int regA;         /* A */
	unsigned int regB;         /* B */
	unsigned int regC;         /* C */
	unsigned int regD;         /* D */
	unsigned int regE;         /* E */
	unsigned int uasr;         /* F */
	unsigned int scr;          /* 10*/
	unsigned int ssr;          /* 11*/
	unsigned int reg12;        /* 12*/
	unsigned int osc_12m_sel;  /* 13*/
};

#define thr rbr
#define iir fcr
#define dll rbr
#define dlm ier


