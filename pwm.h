#ifndef PWM_H
#define PWM_H

// functions
int8_t   pwm_init(void);
uint32_t pwm_control_get(void);
void     pwm_control_set(uint32_t value);
void     pwm_enable(unsigned int channel);
void     pwm_disable(unsigned int channel);
void     pwm_range(unsigned int channel, uint32_t value);
void     pwm_data(unsigned int channel, uint32_t value);
void     pwm_setclk(uint32_t divisor_i, uint32_t divisor_f);


// base offset for BCM2835 peripheral registers (Pi 3)
#define PERI_BASE           ((off_t)0x3F000000)

// PWM clock
// clock-manager registers offset
#define CM_PWM_REG_OFFSET   ((off_t)0x00101000)
extern volatile uint32_t *clk_base;

// PWM clock register pointers (not in BCM2835 datasheet...bad Broadcom!)
#define CM_PWM_REG_CTL     (*(volatile uint32_t *)(clk_base + 40))
#define CM_PWM_REG_DIV     (*(volatile uint32_t *)(clk_base + 41))

// PWM clock register values
#define CM_PWM_PSWD_MASK     ((uint32_t)0xFF000000)  // clock password (required for both CTL and DIV registers)
#define CM_PWM_PSWD          ((uint32_t)0x5A000000)  // password

#define CM_PWM_DIV_I_MASK    ((uint32_t)0x00FFF000)  // clock divisor integer mask
#define CM_PWM_DIV_F_MASK    ((uint32_t)0x00000FFF)  // clock divisor fraction mask

#define CM_PWM_CTL_MASH_MASK ((uint32_t)0x00000600) // MASH control
#define CM_PWM_CTL_MASH_INT  ((uint32_t)0x00000000) // MASH integer division (default)
#define CM_PWM_CTL_MASH_1    ((uint32_t)0x00000200) // MASH 1-stage
#define CM_PWM_CTL_MASH_2    ((uint32_t)0x00000300) // MASH 2-stage
#define CM_PWM_CTL_MASH_3    ((uint32_t)0x00000400) // MASH 3-stage

#define CM_PWM_CTL_FLIP_MASK ((uint32_t)0x00000100) // Invert clock generator output
#define CM_PWM_CTL_FLIP_DIS  ((uint32_t)0x00000000) // Disable
#define CM_PWM_CTL_FLIP_ENA  ((uint32_t)0x00000100) // Enable

#define CM_PWM_CTL_BUSY_MASK ((uint32_t)0x00000080) // Clock generator busy flag
#define CM_PWM_CTL_BUSY_FALS ((uint32_t)0x00000000) // Not busy
#define CM_PWM_CTL_BUSY_TRUE ((uint32_t)0x00000080) // Busy

#define CM_PWM_CTL_KILL_MASK ((uint32_t)0x00000020) // Clock generator kill
#define CM_PWM_CTL_KILL_DIS  ((uint32_t)0x00000000) // keep running
#define CM_PWM_CTL_KILL_ENA  ((uint32_t)0x00000020) // stop and reset

#define CM_PWM_CTL_ENAB_MASK ((uint32_t)0x00000010) // Clock generator enable
#define CM_PWM_CTL_ENAB_DIS  ((uint32_t)0x00000000) // disable
#define CM_PWM_CTL_ENAB_ENA  ((uint32_t)0x00000010) // enable

#define CM_PWM_CTL_SRCE_MASK ((uint32_t)0x0000000f) // Clock generator source
#define CM_PWM_CTL_SRCE_GND  ((uint32_t)0x00000000) // Ground
#define CM_PWM_CTL_SRCE_OSC  ((uint32_t)0x00000001) // Oscillator     (19.2 MHz)
#define CM_PWM_CTL_SRCE_TDB0 ((uint32_t)0x00000002) // Test debug 0   (0 MHz)
#define CM_PWM_CTL_SRCE_TBD1 ((uint32_t)0x00000003) // Test debug 1   (0 MHz)
#define CM_PWM_CTL_SRCE_PLLA ((uint32_t)0x00000004) // PLLA per       (0 MHz)
#define CM_PWM_CTL_SRCE_PLLC ((uint32_t)0x00000005) // PLLC per       (1000 MHz)
#define CM_PWM_CTL_SRCE_PLLD ((uint32_t)0x00000006) // PLLD per       (500 MHz)
#define CM_PWM_CTL_SRCE_HDMI ((uint32_t)0x00000007) // HDMI auxillary (216 MHz)


// PWM control
// PWM control registers offset
#define PWM_REG_OFFSET         ((off_t)0x0020C000)
extern volatile uint32_t *pwm_base;

// PWM control register pointers (divide datasheet byte-offset values by 4 to get word-offsets for uint32_t)
#define PWM_REG_CTL     (*(volatile uint32_t *)(pwm_base + (0x00/4)))
#define PWM_REG_STA     (*(volatile uint32_t *)(pwm_base + (0x04/4)))
#define PWM_REG_DMAC    (*(volatile uint32_t *)(pwm_base + (0x08/4)))
#define PWM_REG_RNG1    (*(volatile uint32_t *)(pwm_base + (0x10/4)))
#define PWM_REG_DAT1    (*(volatile uint32_t *)(pwm_base + (0x14/4)))
#define PWM_REG_FIF1    (*(volatile uint32_t *)(pwm_base + (0x18/4)))
#define PWM_REG_RNG2    (*(volatile uint32_t *)(pwm_base + (0x20/4)))
#define PWM_REG_DAT2    (*(volatile uint32_t *)(pwm_base + (0x24/4)))

// PWM CTL register values
#define PWM_CTL_MSEN2_MASK   ((uint32_t)0x00008000)  // Mark/space enable channel 2
#define PWM_CTL_MSEN2_PWM    ((uint32_t)0x00000000)  // Enable PWM (default)
#define PWM_CTL_MSEN2_MS     ((uint32_t)0x00008000)  // Enable M/S

#define PWM_CTL_USEF2_MASK   ((uint32_t)0x00002000)  // Use FIFO channel 2
#define PWM_CTL_USEF2_DATA   ((uint32_t)0x00000000)  // Use data register (default)
#define PWM_CTL_USEF2_FIFO   ((uint32_t)0x00002000)  // Use FIFO

#define PWM_CTL_POLA2_MASK   ((uint32_t)0x00001000)  // Polarity channel 2
#define PWM_CTL_POLA2_AHI    ((uint32_t)0x00000000)  // Active high (default)
#define PWM_CTL_POLA2_ALO    ((uint32_t)0x00001000)  // Active low

#define PWM_CTL_SBIT2_MASK   ((uint32_t)0x00000800)  // Silence bit channel 2
#define PWM_CTL_SBIT2_0      ((uint32_t)0x00000000)  // Silence bit = 0 (default)
#define PWM_CTL_SBIT2_1      ((uint32_t)0x00000800)  // Silence bit = 1

#define PWM_CTL_RPTL2_MASK   ((uint32_t)0x00000400)  // Repeat last data channel 2
#define PWM_CTL_RPTL2_INT    ((uint32_t)0x00000000)  // Interrupt when FIFO empty (default)
#define PWM_CTL_RPTL2_REP    ((uint32_t)0x00000400)  // Repeat last data until FIFO empty

#define PWM_CTL_MODE2_MASK   ((uint32_t)0x00000200)  // Mode channel 2
#define PWM_CTL_MODE2_PWM    ((uint32_t)0x00000000)  // PWM mode (default)
#define PWM_CTL_MODE2_SER    ((uint32_t)0x00000200)  // Serializer mode

#define PWM_CTL_PWEN2_MASK   ((uint32_t)0x00000100)  // Enable channel 2
#define PWM_CTL_PWEN2_DIS    ((uint32_t)0x00000000)  // Disable (default)
#define PWM_CTL_PWEN2_ENA    ((uint32_t)0x00000100)  // Enable

#define PWM_CTL_MSEN1_MASK   ((uint32_t)0x00000080)  // Mark/space enable channel 1
#define PWM_CTL_MSEN1_PWM    ((uint32_t)0x00000000)  // Enable PWM (default)
#define PWM_CTL_MSEN1_MS     ((uint32_t)0x00000080)  // Enable M/S

#define PWM_CTL_CLRF1_MASK   ((uint32_t)0x00000040)  // Clear FIFO
#define PWM_CTL_CLRF1_NOE    ((uint32_t)0x00000000)  // No effect
#define PWM_CTL_CLRF1_CLR    ((uint32_t)0x00000040)  // Clear FIFO

#define PWM_CTL_USEF1_MASK   ((uint32_t)0x00000020)  // Use FIFO channel 1
#define PWM_CTL_USEF1_DATA   ((uint32_t)0x00000000)  // Use data register (default)
#define PWM_CTL_USEF1_FIFO   ((uint32_t)0x00000020)  // Use FIFO

#define PWM_CTL_POLA1_MASK   ((uint32_t)0x00000010)  // Polarity channel 1
#define PWM_CTL_POLA1_AHI    ((uint32_t)0x00000000)  // Active high (default)
#define PWM_CTL_POLA1_ALO    ((uint32_t)0x00000010)  // Active low

#define PWM_CTL_SBIT1_MASK   ((uint32_t)0x00000008)  // Silence bit channel 1
#define PWM_CTL_SBIT1_0      ((uint32_t)0x00000000)  // Silence bit = 0 (default)
#define PWM_CTL_SBIT1_1      ((uint32_t)0x00000008)  // Silence bit = 1

#define PWM_CTL_RPTL1_MASK   ((uint32_t)0x00000004)  // Repeat last data channel 1
#define PWM_CTL_RPTL1_INT    ((uint32_t)0x00000000)  // Interrupt when FIFO empty (default)
#define PWM_CTL_RPTL1_REP    ((uint32_t)0x00000004)  // Repeat last data until FIFO empty

#define PWM_CTL_MODE1_MASK   ((uint32_t)0x00000002)  // Mode channel 1
#define PWM_CTL_MODE1_PWM    ((uint32_t)0x00000000)  // PWM mode (default)
#define PWM_CTL_MODE1_SER    ((uint32_t)0x00000002)  // Serializer mode

#define PWM_CTL_PWEN1_MASK   ((uint32_t)0x00000001)  // Enable channel 1
#define PWM_CTL_PWEN1_DIS    ((uint32_t)0x00000000)  // Disable (default)
#define PWM_CTL_PWEN1_ENA    ((uint32_t)0x00000001)  // Enable


// PWM STA register values
#define PWM_STA_STA4_MASK    ((uint32_t)0x00001000)  // Channel 4 State
#define PWM_STA_STA3_MASK    ((uint32_t)0x00000800)  // Channel 3 State
#define PWM_STA_STA2_MASK    ((uint32_t)0x00000400)  // Channel 2 State
#define PWM_STA_STA1_MASK    ((uint32_t)0x00000200)  // Channel 1 State
#define PWM_STA_BERR_MASK    ((uint32_t)0x00000100)  // Bus error flag
#define PWM_STA_GAP04_MASK   ((uint32_t)0x00000080)  // Channel 4 gap error occurred
#define PWM_STA_GAP03_MASK   ((uint32_t)0x00000040)  // Channel 2 gap error occurred
#define PWM_STA_GAP02_MASK   ((uint32_t)0x00000020)  // Channel 3 gap error occurred
#define PWM_STA_GAP01_MASK   ((uint32_t)0x00000010)  // Channel 1 gap error occurred
#define PWM_STA_RERR1_MASK   ((uint32_t)0x00000008)  // FIFO read error
#define PWM_STA_WERR1_MASK   ((uint32_t)0x00000004)  // FIFO write error
#define PWM_STA_EMPT1_MASK   ((uint32_t)0x00000002)  // FIFO empty
#define PWM_STA_FULL1_MASK   ((uint32_t)0x00000002)  // FIFO full

// PWM DMAC register values
#define PWM_DMAC_ENAB_MASK    ((uint32_t)0x80000000)  // DMA Enable
#define PWM_DMAC_ENAB_DIS     ((uint32_t)0x00000000)  // Disable
#define PWM_DMAC_ENAB_ENA     ((uint32_t)0x80000000)  // Enable

#define PWM_DMAC_PANIC_MASK   ((uint32_t)0x0000FF00)  // DMA PANIC signal threshold
#define PWM_DMAC_DREQ_MASK    ((uint32_t)0x000000FF)  // DMA DREQ signal threshold

#endif // PWM_H
