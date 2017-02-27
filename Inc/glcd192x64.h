#ifndef		___GLCD192X64_H
#define		___GLCD192X64_H


/****************	LIBRARY FOR GRAPHIC LCD	********************************
*****************      USING CHIP KS0108    ********************************
***
*** Author			:	Xuan Thiep
*** Version			:	1.0 
*** Chip control	:	Samsung KS0108
*** MCU				:	ARM Cortex M from STM using HAL Library
*** Include:             			+ Define connection for GLCD
***									+ Instrucion Code
***									+ Define macro
***									+ Declare function prototype
*** Date			:   11/12/2016
****************************************************************************/


/******************** INCLUDE LIBRARY **************************************/

/*if use other stm32 chip, change this library*/
#include "stm32f1xx_hal.h"


/******************** DEFINE FOR CONNECTION ********************************/
/* NOTE	: edit if need	*/

//1.D0
#define		PORTD0		GPIOB
#define		PIND0		GPIO_PIN_10

//2.D1
#define		PORTD1		GPIOB
#define		PIND1		GPIO_PIN_11

//3.D2
#define		PORTD2		GPIOB
#define		PIND2		GPIO_PIN_13

//4.D3
#define		PORTD3		GPIOB
#define		PIND3		GPIO_PIN_15

//5.D4
#define		PORTD4		GPIOA
#define		PIND4		GPIO_PIN_8

//6.D5
#define		PORTD5		GPIOA
#define		PIND5		GPIO_PIN_10

//7.D6
#define		PORTD6		GPIOA
#define		PIND6		GPIO_PIN_9

//8.D7
#define		PORTD7		GPIOB
#define		PIND7		GPIO_PIN_5


//9.EN
#define		PORTEN		GPIOB
#define		PINEN		GPIO_PIN_2

//10.RS
#define		PORTRS		GPIOB
#define		PINRS		GPIO_PIN_1

//11.RW
#define		PORTRW		GPIOB
#define		PINRW		GPIO_PIN_0

//12.CS1
#define		PORTCS1		GPIOA
#define		PINCS1		GPIO_PIN_15

//13.CS2
#define		PORTCS2		GPIOB
#define		PINCS2		GPIO_PIN_3

//14.CS3
#define		PORTCS3		GPIOB
#define		PINCS3		GPIO_PIN_4

//Need delay between 2 time, (500 ns minimum)if too fast glcd may not operation, else too slow.
//STM32F103 is slow chip => don't delay, but higher speed chip is need.

#define 	pin_en_active(port,pin) {pin_high(port,pin);/* Insert Delay function here if need */ pin_low(port,pin);}



/************** DEFINE PRIVATE MACRO ***************************************/

#define 	pin_low(port,pin)	HAL_GPIO_WritePin(port,pin,GPIO_PIN_RESET);
#define     pin_high(port,pin)	HAL_GPIO_WritePin(port,pin,GPIO_PIN_SET);
#define 	pin_read(port,pin)	HAL_GPIO_ReadPin(port,pin);




/************** DEFINE PRIVATE INSTRUCTION CODE ****************************/

#define	GLCD_DISPLAY				0x3e//0011111x x=1 ON, x=0 OFF
#define	GLCD_YADD					0x40//0x40+y address (column from 0 to 63)
#define	GLCD_XADD					0xb8//0xB8+x address (page from 0 to 7)
#define	GLCD_STARTLINE				0xc0//0xc0+number row of scroll



/************** DECLARING FUNCTION PROTOTYPE ******************************/
/***************************************************************************
*** Description		: Set INPUT/OUTPUT for specific PORT & PIN
***	Input			: GPIOx:	 CHOOSE PORT. EX:GPIOA,GPIOB.....
***					: GPIO_PIN:  CHOOSE PIN. EX:GPIO_PIN_0,GPIO_PIN_1...
***					: GPIO_MODE: CHOOSE INPUT/OUTPUT. EX GPIO_MODE_OUTPUT....
*** Output			: NONE
****************************************************************************/
void GPIO_SET_OPERATION(GPIO_TypeDef *GPIOx,uint16_t GPIO_PIN_X,uint32_t GPIO_MODE);

/***************************************************************************
*** Description		: Set delay (unknown format), use with high speed chip (>72MHz)
***	Input			: time
*** Output			: NONE
****************************************************************************/
void mydelay(uint32_t time);

/***************************************************************************
*** Description		: Set DDR for  DATA pin
***	Input			: Direction:	0- IN, 1 -OUT
*** Output			: NONE
****************************************************************************/
void  set_ddr_data(uint8_t Direction);

/***************************************************************
*** Description		: Set PORT for  DATA pin
***	Input			: data
*** Output			: NONE
****************************************************************/
void set_port_data(uint8_t Data);

/***************************************************************
*** Description		: Set out put or input for pin, which is connect to glcd
***	Input			: Direction: 0-INPUT, 1-OUTPUT
*** Output			: NONE
****************************************************************/
void glcd_set_dir(uint8_t Direction);

/***************************************************************
*** Description		: Select chip ks0108 first,second or third
***	Input			: Chip select: 0-first, 1-second, 2-third
*** Output			: NONE	
****************************************************************/
void glcd_setside(uint8_t ChipSelect);

/***************************************************************
*** Description		: Wait lcd is free
***	Input			: NONE
*** Output			: NONE
****************************************************************/
void glcd_wait(void);

/***************************************************************
*** Description		: GLCD set display on/off
***	Input			: 0 off, 1 on
*** Output			: NONE
****************************************************************/
void glcd_set_display(uint8_t ON_OFF);

/***************************************************************
*** Description		: Set y Address in one chip KS0108
***	Input			: Column (value from 0 to 63)
*** Output			: NONE
****************************************************************/
void glcd_set_yadd(uint8_t Y_Add);

/***************************************************************
*** Description		: Set X Address in one chip KS0108
***	Input			: Page (Value from 0 to 7)
*** Output			: NONE
****************************************************************/
void glcd_set_xadd(uint8_t X_Add);

/***************************************************************
*** Description		: Set GLCD START LINE 
***	Input			: Row start line
*** Output			: NONE
****************************************************************/
void glcd_set_start_line(uint8_t Start_Line);

/***************************************************************
*** Description		: Write one byte to GLCD at current position x,y
***	Input			: Data 
*** Output			: NONE
****************************************************************/
void glcd_write_data(uint8_t Data);

/***************************************************************
*** Description		: Initialize GLCD
***	Input			: NONE 
*** Output			: NONE	
****************************************************************/
void glcd_init(void);

/***************************************************************
*** Description		: Set Cursor position 
***	Input			: x is row (from 0 to 7), y is column (from 0 to 191) 
*** Output			: NONE	
****************************************************************/
void glcd_goto_xy(uint8_t x,uint8_t y);

/***************************************************************
*** Description		: Clear the GLCD 
***	Input			: NONE  
*** Output			: NONE	
****************************************************************/
void glcd_clear(void);

/***************************************************************
*** Description		: Write a character 8x6(8bit row and 6 bit column) to GLCD at specific position, Important :)
***	Input			: Position to write: x is row(0-7),y is column (0-191), data to write 
*** Output			: NONE	
****************************************************************/
void glcd_putchar8x6(uint8_t x,uint8_t y,uint8_t data);

/***************************************************************
*** Description		: Write a string 8x6(8bit row and 6 bit column) to GLCD at specific position, Important :)
***	Input			: Position to write: x is row(0-7),y is column (0-191), String to write
*** Output			: NONE	
****************************************************************/
void glcd_puts8x6(uint8_t x,uint8_t y,uint8_t* string);

/***************************************************************
*** Description		: Write a character 16x10(16bit row and 10 bit column) to GLCD at specific position, Important :)
***	Input			: Position to write: x is row(0-7),y is column (0-191), data to write 
*** Output			: NONE
****************************************************************/
void glcd_putchar16x10(uint8_t x,uint8_t y,uint8_t data);

/***************************************************************
*** Description		: Write a string 16x10(16bit row and 10 bit column) to GLCD at specific position, Important :)
***	Input			: Position to write: x is row(0-7),y is column (0-191), String to write
*** Output			: NONE	
****************************************************************/
void glcd_puts16x10(uint8_t x, uint8_t y, uint8_t* String);

/***************************************************************
*** Description		: Write a character 24x14(24 bit row and 14 bit column) to GLCD at specific position, Important :)
***	Input			: Position to write: x is row(0-7),y is column (0-191), data to write 
*** Output			: NONE
****************************************************************/
void glcd_putchar24x14(uint8_t x,uint8_t y,uint8_t data);

/***************************************************************
*** Description		: Write a string 24x14(24 bit row and 14 bit column) to GLCD at specific position, Important :)
***	Input			: Position to write: x is row(0-7),y is column (0-191), String to write
*** Output			: NONE	
****************************************************************/
void glcd_puts24x14(uint8_t x,uint8_t y,uint8_t* string);

/***************************************************************
*** Description		: Write a character 40x24(40 bit row and 24 bit column) to GLCD at specific position, Important :)
***	Input			: Position to write: x is row(0-7),y is column (0-191), data to write 
*** Output			: NONE
****************************************************************/
void glcd_putchar40x23(uint8_t x,uint8_t y,uint8_t data);

/***************************************************************
*** Description		: Write a string 24x14(24 bit row and 14 bit column) to GLCD at specific position, Important :)
***	Input			: Position to write: x is row(0-7),y is column (0-191), String to write
*** Output			: NONE	
****************************************************************/
void glcd_puts40x23(uint8_t x,uint8_t y,uint8_t* string);

#endif



