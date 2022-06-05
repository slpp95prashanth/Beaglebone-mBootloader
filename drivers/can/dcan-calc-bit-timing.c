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

#define DCAN1_CTL		(SOC_DCAN_1_REGS + 0x0)
#define DCAN1_BTR		(SOC_DCAN_1_REGS + 0xc)

#define DCAN_CTL_SWR   (0x00008000u)
#define DCAN_CTL_INIT   (0x00000001u)
#define DCAN_CTL_CCE   (0x00000040u)

#define DCAN_BITRATE	(1000000u)
#define DCAN_IN_CLK     (24000000u)

/* Macros used by CANBitTimeCalculator */
#define CAN_CALC_MAX_ERROR              (50u)
#define BIT_RATE_ERR_WARN               (2u)
#define BIT_RATE_ERR_MAX                (1u)
#define EXTRACT_BRPE_VAL                (0x3C0u)
#define NO_BIT_RATE_ERR                 (3u)
#define BRPE_SHIFT                      (10u)

/* BTR */
#define DCAN_BTR_BRP   (0x0000003Fu)
#define DCAN_BTR_BRP_SHIFT   (0x00000000u)

#define DCAN_BTR_BRPE   (0x000F0000u)
#define DCAN_BTR_BRPE_SHIFT   (0x00000010u)

#define DCAN_BTR_DCAN_BTR   (0xFFFFFFFFu)
#define DCAN_BTR_DCAN_BTR_SHIFT   (0x00000000u)

#define DCAN_BTR_SJW   (0x000000C0u)
#define DCAN_BTR_SJW_SHIFT   (0x00000006u)

#define DCAN_BTR_TSEG1   (0x00000F00u)
#define DCAN_BTR_TSEG1_SHIFT   (0x00000008u)

#define DCAN_BTR_TSEG2   (0x00007000u)
#define DCAN_BTR_TSEG2_SHIFT   (0x0000000Cu)

#define read(x) 		(*(volatile unsigned int *)(x))
#define write(x, y)	((*(volatile unsigned int *)(x)) = y)

/*  
** Structure used by the CAN Bit time calculator. Will hold values related to 
** bit-time configuration.
*/  
struct _dcan_bittiming{
    unsigned int bitRate;          /* Bit-rate in bits/second */
    unsigned int samplePnt;        /* Sample point in one-tenth of a percent */
    unsigned long tq;              /* Time quanta (tq) in nanoseconds */
    unsigned int propSeg;          /* Propagation segment in tq */
    unsigned int phaseSeg1;        /* Phase buffer segment 1 in tq */
    unsigned int phaseSeg2;        /* Phase buffer segment 2 in tq */
    unsigned int sjw;              /* Synchronisation jump width in tq */
    unsigned int brp;              /* Bit-rate prescaler */
};

/*
** Structure used by the CAN Bit time calculator. Will hold values related to 
** bit-time configuration.
*/
struct _dcan_hw_params{
    unsigned int tseg1Min;        /* Time segement 1 = prop_seg + phase_seg1 */
    unsigned int tseg1Max;
    unsigned int tseg2Min;        /* Time segement 2 = phase_seg2 */
    unsigned int tseg2Max;
    unsigned int sjwMax;          /* Synchronisation jump width */
    unsigned int brpMin;          /* Bit-rate prescaler */
    unsigned int brpMax;
    unsigned int brpInc;
};

/**
 * \brief   This function will update the sampling point based on time 
 *          segment values \n
 *
 * \return  Updated sample point value \n
 *
 **/
int canUpdatSamPt(const struct _dcan_hw_params *ptr,
                  int sampl_pt, int tseg, int *tseg1, int *tseg2)
{
    *tseg2 = tseg + 1 - (sampl_pt * (tseg + 1)) / 1000;

    if(*tseg2 < ptr->tseg2Min)
    {
        *tseg2 = ptr->tseg2Min;
    }

    if(*tseg2 > ptr->tseg2Max)
    {
        *tseg2 = ptr->tseg2Max;
    }

    *tseg1 = tseg - *tseg2;

    if (*tseg1 > ptr->tseg1Max)
    {
        *tseg1 = ptr->tseg1Max;
        *tseg2 = tseg - *tseg1;
    }

    return(1000 * (tseg + 1 - *tseg2) / (tseg + 1));
}

unsigned int CANbitTimeCalculator(struct _dcan_hw_params *btc,
                               struct _dcan_bittiming *bt,
                               unsigned int clkFreq)
{
    int sampl_pt, spt_error = 1000, tsegall, tseg = 0, tseg1 = 0, tseg2 = 0;
    int brp = 0, spt = 0, best_tseg = 0, best_brp = 0;
    long error = 0, best_error = 1000000000;
    unsigned int errVal = NO_BIT_RATE_ERR;
    unsigned long rate, timeQuanta;

    if(bt->bitRate > 800000)
    {
        sampl_pt = 750;
    }

    else if(bt->bitRate > 500000)
    {
        sampl_pt = 800;
    }

    else
    {
        sampl_pt = 875;
    }

    for(tseg = (btc->tseg1Max + btc->tseg2Max) * 2 + 1;
        tseg >= (btc->tseg1Min + btc->tseg2Min) * 2; tseg--)
    {
        tsegall = 1 + tseg / 2;
        /* Compute all possible tseg choices (tseg=tseg1+tseg2) */
        brp = clkFreq / (tsegall * bt->bitRate) + tseg % 2;
        /* chose brp step which is possible in system */
        brp = (brp / btc->brpInc) * btc->brpInc;
        if((brp < btc->brpMin) || (brp > btc->brpMax))
            continue;
        rate = clkFreq / (brp * tsegall);
        error = bt->bitRate - rate;
        /* tseg brp biterror */
        if(error < 0)
            error = -error;
        if(error > best_error)
            continue;
        best_error = error;
        if(error == 0)
        {
            spt = canUpdatSamPt(btc, sampl_pt, tseg / 2,
                                 &tseg1, &tseg2);
            error = sampl_pt - spt;
            if(error < 0)
                error = -error;
            if(error > spt_error)
                continue;
            spt_error = error;
        }
        best_tseg = tseg / 2;
        best_brp = brp;
        if(error == 0)
            break;
    }

    if(best_error)
    {
        /* Error in one-tenth of a percent */
        error = (best_error * 1000) / bt->bitRate;
        if(error > CAN_CALC_MAX_ERROR)
        {
            errVal = BIT_RATE_ERR_MAX;
        }
        else
        {
            errVal = BIT_RATE_ERR_WARN;
        }
    }

    /* real sample point */
    bt->samplePnt = canUpdatSamPt(btc, sampl_pt, best_tseg,
                                      &tseg1, &tseg2);

    timeQuanta = best_brp * 1000000000UL;

    bt->tq = timeQuanta;
    bt->propSeg = tseg1 / 2;
    bt->phaseSeg1 = tseg1 - bt->propSeg;
    bt->phaseSeg2 = tseg2;
    bt->sjw = 1;
    bt->brp = best_brp;
    /* real bit-rate */
    bt->bitRate = clkFreq / (bt->brp * (tseg1 + tseg2 + 1));

    return errVal;
}

void dcan_set_bit_timing(unsigned int bitRate)
{
    unsigned int errVal = 0, btrValue = 0, tSeg1 = 0, tSeg2 = 0;
    struct _dcan_bittiming bit_time_values;
    struct _dcan_hw_params *btc;
    struct _dcan_bittiming *bt;

    static struct _dcan_hw_params dcan_hw_params = {
    /* tseg1Min = */ 1,
    /* tseg1Max = */ 16,
    /* tseg2Min = */ 1,
    /* tseg2Max = */ 8,
    /* sjwMax   = */ 4,
    /* brpMin   = */ 1,
    /* brpMax   = */ 1024,
    /* brpInc   = */ 1,
    };

    bt = &bit_time_values;
    btc = &dcan_hw_params;

    bt->bitRate = bitRate;

    errVal = CANbitTimeCalculator(btc, bt, DCAN_IN_CLK);

    /* Calculate Time Segment2 value */
    tSeg2 = (bt->phaseSeg2 - 1);

    /* Calculate Time Segment1 value */
    tSeg1 = (bt->phaseSeg1 + bt->propSeg - 1);

    /* Write the BRP value */
    btrValue |= ((bt->brp - 1) & DCAN_BTR_BRP);

    /* Write the BRPE value */
    btrValue |= (((bt->brp - 1) & EXTRACT_BRPE_VAL) << BRPE_SHIFT);

    /* Write the Time Segment2 value */
    btrValue |= ((tSeg2 << DCAN_BTR_TSEG2_SHIFT) & DCAN_BTR_TSEG2);

    /* Write the Time Segment1 value */
    btrValue |= ((tSeg1 << DCAN_BTR_TSEG1_SHIFT) & DCAN_BTR_TSEG1);

    write(DCAN1_BTR, btrValue);

    return errVal;
}
