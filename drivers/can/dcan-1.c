#include <stdio.h>

#define SOC_CM_PER_REGS (0x44E00000)

#define CM_PER_DCAN0_CLKCTRL (SOC_CM_PER_REGS + 0xc0)
#define CM_PER_DCAN1_CLKCTRL (SOC_CM_PER_REGS + 0xc4)

#define CM_PER_DCAN0_CLKCTRL_MODULEMODE_ENABLE (0x2)
#define CM_PER_DCAN1_CLKCTRL_MODULEMODE_ENABLE (0x2)

#define SOC_CONTROL_REGS                     (0x44E10000)

#define CONTROL_CONF_UART_CTSN (SOC_CONTROL_REGS + 0x968)
#define CONTROL_CONF_UART_RTSN (SOC_CONTROL_REGS + 0x96c)

#define CONTROL_DCAN_RAMINIT (SOC_CONTROL_REGS + 0x644)

#define CONTROL_DCAN_RAMINIT_DCAN0_RAMINIT_START   (0x00000001u)

#define SOC_DCAN_0_REGS                      (0x481CC000) 
#define SOC_DCAN_1_REGS                      (0x481D0000) 

#define DCAN1_CTL   	(SOC_DCAN_1_REGS + 0x0)
#define DCAN1_TEST   	(SOC_DCAN_1_REGS + 0x14)
#define DCAN1_INT   	(SOC_DCAN_1_REGS + 0x10)
#define DCAN1_IF1CMD	(SOC_DCAN_1_REGS + 0x100)
#define DCAN1_IF2CMD	(SOC_DCAN_1_REGS + 0x120)
#define DCAN1_IF1ARB	(SOC_DCAN_1_REGS + 0x108)
#define DCAN1_IF2ARB	(SOC_DCAN_1_REGS + 0x128)
#define DCAN1_IF1MCTL	(SOC_DCAN_1_REGS + 0x10c)
#define DCAN1_IF2MCTL	(SOC_DCAN_1_REGS + 0x12c)
#define DCAN1_IF1DATA	(SOC_DCAN_1_REGS + 0X110)
#define DCAN1_IF2DATA	(SOC_DCAN_1_REGS + 0X130)
#define DCAN1_IF1DATB	(SOC_DCAN_1_REGS + 0X114)
#define DCAN1_IF2DATB	(SOC_DCAN_1_REGS + 0X134)

#define DCAN1_TXRQ(n)   (SOC_DCAN_1_REGS + 0x88 + (n * 4))
#define DCAN1_MSGVAL(n)   (SOC_DCAN_1_REGS + 0xC4 + (n * 4))            

#define DCAN1_IFARB(n)	(n == 1 ? DCAN1_IF1ARB : DCAN1_IF2ARB)
#define DCAN1_IFCMD(n)	(n == 1 ? DCAN1_IF1CMD : DCAN1_IF2CMD)
#define DCAN1_IFMCTL(n)	(n == 1 ? DCAN1_IF1MCTL : DCAN1_IF2MCTL)
#define DCAN1_IFDATA(n)	(n == 1 ? DCAN1_IF1DATA : DCAN1_IF2DATA)
#define DCAN1_IFDATB(n)	(n == 1 ? DCAN1_IF1DATB : DCAN1_IF2DATB)

#define DCAN_CTL_SWR   (0x00008000u)
#define DCAN_CTL_INIT   (0x00000001u)
#define DCAN_CTL_CCE   (0x00000040u)
#define DCAN_CTL_IE0   (0x00000002u)
#define DCAN_CTL_IE1   (0x00020000u)
#define DCAN_CTL_EIE   (0x00000008u)
#define DCAN_CTL_ABO   (0x00000200u)
#define DCAN_CTL_SIE   (0x00000004u)
#define DCAN_CTL_TEST   (0x00000080u)

#define DCAN_BITRATE	(1000000u)
#define DCAN_IN_CLK     (24000000u)

#define DCAN_IFCMD_BUSY   (0x00008000u) 
#define DCAN_IFCMD_CLRINTPND   (0x00080000u)
#define DCAN_IFCMD_CONTROL   (0x00100000u)
#define DCAN_IFCMD_DATAA   (0x00020000u)
#define DCAN_IFCMD_DATAB   (0x00010000u)
#define DCAN_IFCMD_DMAACTIVE   (0x00004000u)
#define DCAN_IFCMD_MASK   (0x00400000u)
#define DCAN_IFCMD_MESSAGENUMBER   (0x000000FFu)
#define DCAN_IFCMD_TXRQST_NEWDAT   (0x00040000u)
#define DCAN_IFCMD_WR_RD   (0x00800000u)
#define DCAN_IFCMD_ARB   (0x00200000u)

#define DCAN_IFARB_MSGVAL   (0x80000000u)

#define DCAN_TEST_EXL   (0x00000100u)
#define DCAN_TEST_LBACK   (0x00000010u)
#define DCAN_TEST_RDA   (0x00000200u)
#define DCAN_TEST_RX   (0x00000080u)
#define DCAN_TEST_TX   (0x00000060u)
#define DCAN_TEST_SILENT   (0x00000008u) 

#define DCAN_INT_INT0ID   (0x0000FFFFu)
#define DCAN_INT_INT1ID   (0x00FF0000u)

#define DCAN_NO_INT_PENDING               (0x00000000u)
#define DCAN_ERROR_OCCURED                (0x8000u)

#define CAN_NUM_OF_MSG_OBJS	(64u)
#define CAN_TX_MSG_STD_ID	(0x02u)

#define read(x) 		(*(volatile unsigned int *)(x))
#define write(x, y)	((*(volatile unsigned int *)(x)) = y)

void dcan_clk_config(void)
{
	write(CM_PER_DCAN1_CLKCTRL , CM_PER_DCAN1_CLKCTRL_MODULEMODE_ENABLE);
	while ((read(CM_PER_DCAN1_CLKCTRL) & 0x3) != CM_PER_DCAN1_CLKCTRL_MODULEMODE_ENABLE);

	write(CM_PER_DCAN0_CLKCTRL , CM_PER_DCAN0_CLKCTRL_MODULEMODE_ENABLE);
	while ((read(CM_PER_DCAN0_CLKCTRL) & 0x3) != CM_PER_DCAN0_CLKCTRL_MODULEMODE_ENABLE);
}

void dcan_control_module(void)
{
	write(CONTROL_CONF_UART_CTSN, 0x2);
	write(CONTROL_CONF_UART_RTSN, 0x32);
}

void dcan_reset(void)
{
	write(DCAN1_CTL, read(DCAN1_CTL) | DCAN_CTL_SWR);

	while(read(DCAN1_CTL) & DCAN_CTL_SWR);
}

void dcan_init_mode_set(void)
{
	write(DCAN1_CTL, read(DCAN1_CTL) | DCAN_CTL_INIT);
printf("%s %d\n", __FILE__, __LINE__);

	while((read(DCAN1_CTL) & DCAN_CTL_INIT) == 0);
printf("%s %d\n", __FILE__, __LINE__);
}

void dcan_init_mode_clear(void)
{
	write(DCAN1_CTL, read(DCAN1_CTL) & (~DCAN_CTL_INIT));

	while(read(DCAN1_CTL) & DCAN_CTL_INIT);
}

void dcan_config_reg_enable_access_control(void)
{
	write(DCAN1_CTL, read(DCAN1_CTL) | DCAN_CTL_CCE);
}

void dcan_config_reg_disable_access_control(void)
{
	write(DCAN1_CTL, read(DCAN1_CTL) & (~DCAN_CTL_CCE));
}

void dcan_config_bit_timing(void)
{
	dcan_config_reg_enable_access_control();
printf("config bit timing %s %d\n", __FILE__, __LINE__);

	dcan_set_bit_timing(DCAN_BITRATE);
printf("set bit timing %s %d\n", __FILE__, __LINE__);

	dcan_config_reg_disable_access_control();
printf("disable access control %s %d\n", __FILE__, __LINE__);
}

bool dcan_ifreg_busy_status(unsigned int ifreg_num)
{
	return (read(DCAN1_IFCMD(ifreg_num)) & DCAN_IFCMD_BUSY);
}

void dcan_set_cmd_reg(unsigned int flags, unsigned int msgobj_num, unsigned int ifreg_num)
{
	while (dcan_ifreg_busy_status(ifreg_num) == 1);

	write(DCAN1_IFCMD(ifreg_num), read(DCAN1_IFCMD(ifreg_num)) & (~(DCAN_IFCMD_DMAACTIVE |
                                             DCAN_IFCMD_DATAA |
                                             DCAN_IFCMD_DATAB |
                                             DCAN_IFCMD_TXRQST_NEWDAT |
                                             DCAN_IFCMD_CLRINTPND |
                                             DCAN_IFCMD_CONTROL |
                                             DCAN_IFCMD_ARB |
                                             DCAN_IFCMD_MASK |
                                             DCAN_IFCMD_MESSAGENUMBER |
                                             DCAN_IFCMD_WR_RD)));

	while (dcan_ifreg_busy_status(ifreg_num) == 1);

	write(DCAN1_IFCMD(ifreg_num), read(DCAN1_IFCMD(ifreg_num)) | ((flags & (DCAN_IFCMD_DMAACTIVE |
                                             DCAN_IFCMD_DATAA |
                                             DCAN_IFCMD_DATAB |
                                             DCAN_IFCMD_TXRQST_NEWDAT |
                                             DCAN_IFCMD_CLRINTPND |
                                             DCAN_IFCMD_CONTROL |
                                             DCAN_IFCMD_ARB |
                                             DCAN_IFCMD_MASK |
                                             DCAN_IFCMD_WR_RD)) |
							   (msgobj_num & DCAN_IFCMD_MESSAGENUMBER)));
}

void dcan_invalidate_message_obj(unsigned int ifreg_num)
{
	while (dcan_ifreg_busy_status(ifreg_num) == 1);

	write(DCAN1_IFARB(ifreg_num), read(DCAN1_IFARB(ifreg_num)) & ~DCAN_IFARB_MSGVAL);
}

void dcan_validate_message_obj(unsigned int ifreg_num)
{
	while (dcan_ifreg_busy_status(ifreg_num) == 1);

	write(DCAN1_IFARB(ifreg_num), read(DCAN1_IFARB(ifreg_num)) | DCAN_IFARB_MSGVAL);
}

/* Identifier of 11 bit size is used */
#define DCAN_11_BIT_ID                      (0)
/* Identifier of 29 bit size is used */
#define DCAN_29_BIT_ID                      (1)

#define DCAN_IFMCTL_EOB   (0x00000080u)
#define DCAN_IFMCTL_INTPND   (0x00002000u)
#define DCAN_IFMCTL_MSGLST   (0x00004000u)
#define DCAN_IFMCTL_NEWDAT   (0x00008000u)
#define DCAN_IFMCTL_RMTEN   (0x00000200u)
#define DCAN_IFMCTL_RXIE   (0x00000400u)
#define DCAN_IFMCTL_TXIE   (0x00000800u)
#define DCAN_IFMCTL_TXRQST   (0x00000100u)
#define DCAN_IFMCTL_UMASK   (0x00001000u)

#define DCAN_STD_ID_SHIFT			  (18u)

#define DCAN_IFARB_MSK   (0x1FFFFFFFu)
#define DCAN_IFARB_XTD   (0x40000000u)
#define DCAN_IFARB_DIR   (0x20000000u)

#define DCAN_IFMCTL_DATALENGTHCODE   (0x0000000Fu)

void dcan_set_id(unsigned int id, unsigned int id_len, unsigned int ifreg_num)
{
	if (id_len == DCAN_11_BIT_ID) {
		id <<= DCAN_STD_ID_SHIFT;
	}

	while (dcan_ifreg_busy_status(ifreg_num) == 1);

	/* Clear the Msk field of DCAN_IFARB register */
	write(DCAN1_IFARB(ifreg_num), read(DCAN1_IFARB(ifreg_num)) & (~(DCAN_IFARB_MSK | DCAN_IFARB_XTD)));

	while (dcan_ifreg_busy_status(ifreg_num) == 1);

	/* Set the Msk field with the ID value */
	write(DCAN1_IFARB(ifreg_num), read(DCAN1_IFARB(ifreg_num)) | ((id & DCAN_IFARB_MSK) | (id_len | DCAN_IFARB_XTD)));
}

#define DCAN_IFARB_TX_DIR	(0x20000000u)
#define DCAN_IFARB_RX_DIR	(0x0u)

#define DCAN_TX_DIR		(1)
#define DCAN_RX_DIR		(0)

void dcan_set_dir_tx(unsigned int ifreg_num)
{
      while (dcan_ifreg_busy_status(ifreg_num) == 1);

	write(DCAN1_IFARB(ifreg_num), read(DCAN1_IFARB(ifreg_num)) | DCAN_IFARB_TX_DIR);
}

void dcan_set_dir_rx(unsigned int ifreg_num)
{
      while (dcan_ifreg_busy_status(ifreg_num) == 1);

	write(DCAN1_IFARB(ifreg_num), read(DCAN1_IFARB(ifreg_num)) & ~DCAN_IFARB_TX_DIR);
}

void dcan_set_dlc(unsigned int dlc, unsigned int ifreg_num)
{
      while (dcan_ifreg_busy_status(ifreg_num) == 1);

	/* Clear the DLC field of DCAN_IFMCTL register */
	write(DCAN1_IFMCTL(ifreg_num), read(DCAN1_IFMCTL(ifreg_num)) & (~DCAN_IFMCTL_DATALENGTHCODE));

      while (dcan_ifreg_busy_status(ifreg_num) == 1);

	/* Set the DLC field with the user sent value */
	write(DCAN1_IFMCTL(ifreg_num), read(DCAN1_IFMCTL(ifreg_num)) | (dlc & DCAN_IFMCTL_DATALENGTHCODE));
}

void dcan_set_data(unsigned int *data, unsigned int ifreg_num)
{
      while (dcan_ifreg_busy_status(ifreg_num) == 1);

	write(DCAN1_IFDATA(ifreg_num), data[0]);

      while (dcan_ifreg_busy_status(ifreg_num) == 1);

	write(DCAN1_IFDATB(ifreg_num), data[1]);
}

void dcan_set_tx_int(unsigned int ifreg_num)
{
      while (dcan_ifreg_busy_status(ifreg_num) == 1);

	write(DCAN1_IFMCTL(ifreg_num), read(DCAN1_IFMCTL(ifreg_num)) | DCAN_IFMCTL_TXIE);
}

void dcan_set_rx_int(unsigned int ifreg_num)
{
      while (dcan_ifreg_busy_status(ifreg_num) == 1);

	write(DCAN1_IFMCTL(ifreg_num), read(DCAN1_IFMCTL(ifreg_num)) | DCAN_IFMCTL_RXIE);
}

void dcan_set_eob(unsigned int ifreg_num)
{
      while (dcan_ifreg_busy_status(ifreg_num) == 1);

	write(DCAN1_IFMCTL(ifreg_num), read(DCAN1_IFMCTL(ifreg_num)) | DCAN_IFMCTL_EOB);
}

void dcan_clear_eob(unsigned int ifreg_num)
{
      while (dcan_ifreg_busy_status(ifreg_num) == 1);

	write(DCAN1_IFMCTL(ifreg_num), read(DCAN1_IFMCTL(ifreg_num)) & (~DCAN_IFMCTL_EOB));
}

unsigned int dcan_get_next_msgobj_txrq_cleared(void)
{
    unsigned int index = 1;
    unsigned int regNum;
    unsigned int offSet;
    
    while(index < 128)
    {                           
        regNum = (index - 1) / 32;
        offSet = (index - 1) % 32;
    
        if(!(read(DCAN1_TXRQ(regNum)) & (1 << offSet)))
        {
            break;
        }
        index++;
    }

    return(index);
}

void dcan_config_msgobj_tx(void *data, unsigned int dlc, unsigned int id, unsigned int id_len)
{
	unsigned int msgobj_num;

	dcan_validate_message_obj(1);

	dcan_set_id(id, id_len, 1);

	dcan_set_dir_tx(1);

	dcan_set_dlc(dlc, 1);

	dcan_set_data(data, 1);

	dcan_set_tx_int(1);

	dcan_set_eob(1);

	msgobj_num = dcan_get_next_msgobj_txrq_cleared();

	printf("tx msgnum = %d\n", msgobj_num);

	dcan_set_cmd_reg(DCAN_IFCMD_DATAA | DCAN_IFCMD_WR_RD | DCAN_IFCMD_TXRQST_NEWDAT |
                                DCAN_IFCMD_DATAB | DCAN_IFCMD_CONTROL |
                                DCAN_IFCMD_ARB, msgobj_num, 1);
}

void dcan_set_accept_filter(unsigned int ifreg_num)
{
	/* Use Acceptance mask. */
	write(DCAN1_IFMCTL(ifreg_num), read(DCAN1_IFMCTL(ifreg_num)) | DCAN_IFMCTL_UMASK);
}

unsigned int dcan_get_msgobj_valid_status(unsigned int msgobj_num)
{
	unsigned int regNum;
	unsigned int offSet;
	
	regNum = (msgobj_num - 1) / 32;
	offSet = (msgobj_num - 1) % 32;
	
	/* Return the status from DCAN1_MSGVAL register */
	return(read(DCAN1_MSGVAL(regNum)) & (1 << offSet));
}

void dcan_config_msgobj_rx(unsigned int id, unsigned int id_len)
{
	unsigned int msgobj_num = (64 / 2);

	printf("rx msgnum = %x\n", msgobj_num);

	dcan_validate_message_obj(2);

	dcan_set_id(id, id_len, 2);

	dcan_set_dir_rx(2);

	dcan_set_rx_int(2);

	dcan_set_eob(2);

	/* Check for the message valid status for receive objects */
	while((msgobj_num <= (CAN_NUM_OF_MSG_OBJS - 1))) {
		if (dcan_get_msgobj_valid_status(msgobj_num)) {
			dcan_set_cmd_reg(DCAN_IFCMD_DATAA | DCAN_IFCMD_WR_RD |
                               DCAN_IFCMD_MASK | DCAN_IFCMD_CONTROL | DCAN_IFCMD_ARB, msgobj_num, 2);

			printf("rx msgnum = %x\n", msgobj_num);
		} else {
			printf("rx msgobj_num skip = %x\n", msgobj_num);
		}
		msgobj_num++;
	}

}

void dcan_set_lpbk(void)
{
	write(DCAN1_TEST, read(DCAN1_TEST) | DCAN_TEST_LBACK);
}

void dcan_set_err_int(void)
{
	write(DCAN1_CTL, read(DCAN1_CTL) | DCAN_CTL_EIE);
}

void dcan_set_int_line(unsigned int line)
{
	write(DCAN1_CTL, read(DCAN1_CTL) | line);
}

unsigned int dcan_get_int_line_status(unsigned int line)
{
	return (read(DCAN1_INT) & line);
}

void dcan_clear_intpnd(unsigned int ifreg_num)
{
      while (dcan_ifreg_busy_status(ifreg_num) == 1);

	write(DCAN1_IFMCTL(ifreg_num), read(DCAN1_IFMCTL(ifreg_num)) & (~(DCAN_IFMCTL_INTPND)));
}

void dcan_clear_int_status_msgobj(unsigned int msgobj_num, unsigned int ifreg_num)
{
	dcan_clear_intpnd(ifreg_num);

	dcan_set_cmd_reg(DCAN_IFCMD_CLRINTPND, msgobj_num, ifreg_num);
}

void dcan_intr(void)
{
	unsigned int status;

	printf("intr\n");

	while (dcan_get_int_line_status(DCAN_INT_INT0ID) == 1) {

		status = dcan_get_int_line_status(DCAN_INT_INT0ID);

		if (status == DCAN_ERROR_OCCURED) {
			printf("err \n");
		} else if (status != DCAN_NO_INT_PENDING && status != DCAN_ERROR_OCCURED) {
			unsigned int msgobj_num = status;

			if (msgobj_num < (CAN_NUM_OF_MSG_OBJS / 2)) {
				printf("tx intr for msgobj_num = %d\n", msgobj_num);

				/* Clear the Interrupt pending status */
				dcan_clear_int_status_msgobj(msgobj_num, 1);
			} else {
				printf("rx intr for msgobj_num = %d\n", msgobj_num);

                        /* Clear the Interrupt pending status */
                        dcan_clear_int_status_msgobj(msgobj_num, 1);
			}
		}
	}
}

void dcan_set_auto_bus_on(void)
{
	write(DCAN1_CTL, read(DCAN1_CTL) | DCAN_CTL_ABO);
}

void dcan_set_test_mode(void)
{
	write(DCAN1_CTL, read(DCAN1_CTL) | DCAN_CTL_TEST);
}

int dcan_init(void)
{
	unsigned int index;

	dcan_clk_config();

	dcan_control_module();

	write(CONTROL_DCAN_RAMINIT, CONTROL_DCAN_RAMINIT_DCAN0_RAMINIT_START);

	request_irq(55, dcan_intr, NULL);

	dcan_reset();

	dcan_init_mode_set();

	dcan_config_bit_timing();

	index = CAN_NUM_OF_MSG_OBJS;

	while (index--) {
		dcan_invalidate_message_obj(2);

		dcan_set_cmd_reg((DCAN_IFCMD_ARB | DCAN_IFCMD_WR_RD), index, 2);
	}

	dcan_config_msgobj_rx(0, DCAN_11_BIT_ID);

	dcan_set_auto_bus_on();

	dcan_set_test_mode();

	dcan_init_mode_clear();

	dcan_set_lpbk();

	dcan_set_err_int();

	dcan_set_int_line(DCAN_CTL_IE0);

	unsigned int data[2] = {(unsigned int)('a' | 'b' << 8), (unsigned int)'c'};

	dcan_config_msgobj_tx(data, 2, CAN_TX_MSG_STD_ID, DCAN_11_BIT_ID);
}
