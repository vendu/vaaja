#ifndef __V0_BSP_PAPILIO_PRO_H__
#define __V0_BSP_PAPILIO_PRO_H__

#include <zen/conf.h>

#if defined(V0_PAPILIO_PRO)

#include <stdint.h>

#define PAPILIO_PRO_PIX_MASK_BIT        (1 << 0)
#define PAPILIO_PRO_PIX_BUF_BIT  	    (1 << 1)
#define PAPILIO_PRO_PIX_SHIFT           4
#define PAPILIO_PRO_PIX_MASK            0x0f

#define PAPILIO_PRO_FPGA_RESET          37
#define PAPILIO_PRO_LED1_OUT            112

typedef int16_t                         v0pix12;
typedef v0pix12                         v0pixel;

#define v0getpixblue(ptr)                                               \
    ((uint8_t *)(ptr)[0] & PAPILIO_PRO_PIX_MASK)
#define v0getpixgreen(ptr)                                              \
    (((uint8_t *)(ptr)[0] >> PAPILIO_PRO_PIX_SHIFT) & PAPILIO_PRO_PIX_MASK)
#define v0getpixred(ptr)                                                \
    ((uint8_t *)(ptr)[1] & PAPILIO_PRO_PIX_MASK)
#define v0getpixflg(ptr)                                                \
    (((uint8_t *)(ptr)[1] >> PAPILIO_PRO_PIX_SHIFT) & PAPILIO_PRO_PIX_MASK)

#define PAPILIO_PRO_TIMER_PREC          32000000        // 32 MHz oscillator
#define PAPILIO_PRO_TIMER_MUL           2               // 64 MHz system bus
#define PAPILIO_PRO_TIMER_HZ            64000000.0      // timer frequency
#define PAPILIO_PRO_HZ                  125             // timer interrupt Hz
#define PAPILIO_PRO_HZ_MUL                                              \
    (long)(PAPILIO_PRO_TIMER_HZ / PAPILIO_PRO_HZ)       // timer Hz factor
#define PAPILIO_PRO_MIN_TICKS           1               // BVT-scheduler
#define PAPILIO_PRO_FLASH_SIZE          (8U * 1024 * 1024)      // size of SPI
#define PAPILIO_PRO_DRAM_SIZE           (8U * 1024 * 1024)     // size of SDRAM
#define PAPILIO_PRO_VIDEO_WIDTH         640                     // screen width
#define PAPILIO_PRO_VIDEO_HEIGHT        480                     // screen height
#define PAPILIO_PRO_VIDEO_BUFSZ  (PAPILIO_PRO_VIDEO_WIDTH * PAPILIO_PRO_VIDEO_HEIGHT * sizeof(v0pixel))
#define PAPILIO_PRO_VGA_DEPTH           4096
#define PAPILIO_PRO_VGA_BITS            12
#define PAPILIO_PRO_BRAM_FB             PAPILIO_PRO_RAM_SIZE    // above SDRAM
#define PAPILIO_PRO_FB_SIZE             460800                  // fb size
#define PAPILIO_PRO_CONF_SIZE           65536                   // BRAM-config
#define PAPILIO_PRO_CONF_BRAM                                           \
    (PAPILIO_PRO_BRAM_SIZE - PAPILIO_PRO_CONF_SIZE - PAPILIO_PRO_FB_SIZE)
#define PAPILIO_PRO_BRAM_UNIT           18000
#define PAPILIO_PRO_BRAM_UNITS          32
#define PAPILIO_PRO_BRAM_SIZE                                           \
    (PAPILIO_PRO_BRAM_UNIT * PAPILIO_PRO_BRAM_UNITS)
#define PAPILIO_PRO_SRAM_SIZE           65536

/* device flags */
#define PAPILIO_PRO_DAC_BIT      	(1 << 0)
#define PAPILIO_PRO_PS2_KBD      	(1 << 1)
#define PAPILIO_PRO_PS2_MOUSE    	(1 << 2)
#define PAPILIO_PRO_PS2_JSTICK1  	(1 << 3)
#define PAPILIO_PRO_PS2_JSTICK2  	(1 << 4)
#define PAPILIO_PRO_UP_BUTTON    	(1 << 5)
#define PAPILIO_PRO_DOWN_BUTTON  	(1 << 6)
#define PAPILIO_PRO_LEFT_BUTTON  	(1 << 7)
#define PAPILIO_PRO_RIGHT_BUTTON 	(1 << 8)
#define PAPILIO_PRO_LED1         	(1 << 9)
#define PAPILIO_PRO_LED2         	(1 << 10)
#define PAPILIO_PRO_LED3         	(1 << 11)
#define PAPILIO_PRO_LED4         	(1 << 12)

#define PAPILIO_PRO_IO_PINS             48
#define PAPILIO_PRO_JTAG_TMS_IN         107
#define PAPILIO_PRO_JTAG_TCK_IN         109
#define PAPILIO_PRO_JTAG_SI_IN          64
#define PAPILIO_PRO_JTAG_SO_IN          65

/* USB pins */
#define PAPILIO_PRO_SER_RX_IN           101     // serial receive
#define PAPILIO_PRO_SER_TX_OUT          105     // serial transmit
#define PAPILIO_PRO_FLASH_CS            38      // chip select
#define PAPILIO_PRO_FLASH_CLK           70      // clock
#define PAPILIO_PRO_FLASH_MOSI          64      // SPI master out slave in
#define PAPILIO_PRO_FLASH_MISO          65      // SPI master in slave out

/* oscillator pin */
#define PAPILIO_PRO_TIMER               94

/* VGA pinout */
#define PAPILIO_PRO_VGA_RED0            118
#define PAPILIO_PRO_VGA_RED2            119
#define PAPILIO_PRO_VGA_RED3            120
#define PAPILIO_PRO_VGA_RED4            121
#define PAPILIO_PRO_VGA_GREEN0          84
#define PAPILIO_PRO_VGA_GREEN1          83
#define PAPILIO_PRO_VGA_GREEN2          80
#define PAPILIO_PRO_VGA_GREEN3          78
#define PAPILIO_PRO_VGA_BLUE0           99
#define PAPILIO_PRO_VGA_BLUE1           97
#define PAPILIO_PRO_VGA_BLUE2           92
#define PAPILIO_PRO_VGA_BLUE3           87
#define PAPILIO_PRO_VGA_HSYNC           117
#define PAPILIO_PRO_VGA_VSYNC           116

/* audio pins */
#define PAPILIO_PRO_AUDIO_LEFT          98
#define PAPILIO_PRO_AUDIO_RIGHT         100

/* button pins */
#define PAPILIO_PRO_LEFT_PIN            74
#define PAPILIO_PRO_UP_PIN              95
#define PAPILIO_PRO_RIGHT_PIN           59
#define PAPILIO_PRO_DOWN_PIN            62

/* led pins */
#define PAPILIO_PRO_PLAYER1_LED         75
#define PAPILIO_PRO_PLAYER2_LED         67
#define PAPILIO_PRO_FLIPPED_LED         66
#define PAPILIO_PRO_LED4_PIN            61

/* joystick #1 */
#define PAPILIO_PRO_JOY1_UP      	123
#define PAPILIO_PRO_JOY1_DOWN    	126
#define PAPILIO_PRO_JOY1_LEFT    	127
#define PAPILIO_PRO_JOY1_RIGHT   	132
#define PAPILIO_PRO_JOY1_FIRE2   	134
#define PAPILIO_PRO_JOY1_FIRE1   	124
#define PAPILIO_PRO_JOY1_5VOLTS         0xff
#define PAPILIO_PRO_JOY1_GROUND         44

/* joystick #2 */
#define PAPILIO_PRO_JOY2_UP             57
#define PAPILIO_PRO_JOY2_DOWN           50
#define PAPILIO_PRO_JOY2_LEFT           47
#define PAPILIO_PRO_JOY2_RIGHT          51
#define PAPILIO_PRO_JOY2_FIRE2          58
#define PAPILIO_PRO_JOY2_FIRE1          55
#define PAPILIO_PRO_JOY2_5VOLTS         0xff
#define PAPILIO_PRO_JOY2_GROUND         48

/* keyboard and mouse */
#define PAPILIO_PRO_PS2_CLK1            115
#define PAPILIO_PRO_PS2_DATA1           114
#define PAPILIO_PRO_PS2_CLK2            93
#define PAPILIO_PRO_PS2_DATA2           88

#define PAPILIO_PRO_RESET_PIN           85

#endif /* V0_PAPILIO_PRO */

#endif /* __V0_BSP_PAPILIO_PRO_H__ */

