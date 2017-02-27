#include "glcd192x64.h"
#include "font8x6.h"
#include "font16x10.h"
#include "font24x14.h"
#include "font40x23.h"

#include "stm32f1xx_hal.h"


GPIO_TypeDef *	PORT_NAME_ARR[]={PORTD0,PORTD1,PORTD2,PORTD3,PORTD4,PORTD5,PORTD6,PORTD7};
uint16_t		PIN_NAME_ARR[]={PIND0,PIND1,PIND2,PIND3,PIND4,PIND5,PIND6,PIND7};


void mydelay(uint32_t time)
{
	while(time--);
}


void GPIO_SET_OPERATION(GPIO_TypeDef *GPIOx,uint16_t GPIO_PIN_X,uint32_t GPIO_MODE)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_X;
	GPIO_InitStruct.Mode = GPIO_MODE;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	if((GPIO_MODE==GPIO_MODE_OUTPUT_PP))
	{
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	}
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}



/***************************************************************************
 *** Description		: Set DDR for  DATA pin
 *** Input				: Direction:	0- IN, 1 -OUT
 *** Output				: NONE
 ****************************************************************************/
void  set_ddr_data(uint8_t Direction)
{
	if(Direction)//Output
	{
		GPIO_SET_OPERATION(PORTD7,PIND7,GPIO_MODE_OUTPUT_PP);
	}
	else	//Input
	{
		GPIO_SET_OPERATION(PORTD7,PIND7,GPIO_MODE_INPUT);
	}
}


/***************************************************************
 *** Description		: Set PORT for  DATA pin
 *** Input				: data
 *** Output				: NONE
 ****************************************************************/
void set_port_data(uint8_t Data)
{
	for(uint8_t i=0;i<8;i++)
	{
		if(Data&(0x01<<i))
		{
			pin_high(PORT_NAME_ARR[i],PIN_NAME_ARR[i]);
		}
		else
		{
			pin_low(PORT_NAME_ARR[i],PIN_NAME_ARR[i]);
		}
	}
}


/***************************************************************
 *** Description		: Set out put or input for pin, which is connect to glcd
 *** Input				: Direction: 0-INPUT, 1-OUTPUT
 *** Output				: NONE
 ****************************************************************/
void glcd_set_dir(uint8_t Direction)
{

	//Disable GLCD
	pin_low(PORTEN,PINEN);
	//Set Direction for data
	set_ddr_data(Direction);
}


/***************************************************************
 *** Description		: Select chip ks0108 first,second or third
 *** Input				: Chip select: 0-first, 1-second, 2-third
 *** Output				: NONE
 ****************************************************************/
void glcd_setside(uint8_t ChipSelect)
{
	//GLCD SET OUT
	glcd_set_dir(1);

	switch(ChipSelect)
	{
	case 0:
		pin_high(PORTCS2,PINCS2);
		pin_high(PORTCS3,PINCS3);
		pin_low(PORTCS1,PINCS1);
		break;
	case 1:
		pin_high(PORTCS1,PINCS1);
		pin_high(PORTCS3,PINCS3);
		pin_low(PORTCS2,PINCS2);
		break;
	case 2:
		pin_high(PORTCS1,PINCS1);
		pin_high(PORTCS2,PINCS2);
		pin_low(PORTCS3,PINCS3);
		break;
	}

	//	May be need a bit of delay
	//	HAL_Delay(1);
}

/***************************************************************
 *** Description		: Wait lcd is free
 *** Input				: NONE
 *** Output				: NONE
 ****************************************************************/
void glcd_wait(void)
{
	//GLCD SET IN
	glcd_set_dir(0);
	pin_high(PORTRW,PINRW);
	pin_low(PORTRS,PINRS);
	pin_en_active(PORTEN,PINEN);
	GPIO_PinState state= pin_read(PORTD7,PIND7);
	while(state)
	{
		pin_en_active(PORTEN,PINEN);
		state= pin_read(PORTD7,PIND7);
	}
}


/***************************************************************
 *** Description		: GLCD set display on/off
 *** Input				: 0 off, 1 on
 *** Output				: NONE
 ****************************************************************/
void glcd_set_display(uint8_t ON_OFF)
{
	glcd_wait();
	//Set out GLCD
	glcd_set_dir(1);
	pin_low(PORTRW,PINRW);
	pin_low(PORTRS,PINRS);
	set_port_data(GLCD_DISPLAY+ON_OFF);
	pin_en_active(PORTEN,PINEN);
}

/***************************************************************
 *** Description		: Set y Address
 *** Input				: Column (value from 0 to 63)
 *** Output				: NONE
 ****************************************************************/
void glcd_set_yadd(uint8_t Y_Add)
{
	glcd_wait();
	//GLCD SET OUTPUT
	glcd_set_dir(1);
	pin_low(PORTRW,PINRW);
	pin_low(PORTRS,PINRS);
	set_port_data(GLCD_YADD+Y_Add);
	pin_en_active(PORTEN,PINEN);
}

/***************************************************************
 *** Description		: Set X Address
 *** Input				: Page (Value from 0 to 7)
 *** Output				: NONE
 ****************************************************************/
void glcd_set_xadd(uint8_t X_Add)
{
	glcd_wait();
	//GLCD SET OUTPUT
	glcd_set_dir(1);
	pin_low(PORTRW,PINRW);
	pin_low(PORTRS,PINRS);
	set_port_data(GLCD_XADD+X_Add);
	pin_en_active(PORTEN,PINEN);
}

/***************************************************************
 *** Description		: Set GLCD START LINE
 *** Input				: Row start line
 *** Output				: NONE
 ****************************************************************/
void glcd_set_start_line(uint8_t Start_Line)
{
	glcd_wait();
	//GLCD SET OUTPUT
	glcd_set_dir(1);
	pin_low(PORTRW,PINRW);
	pin_low(PORTRS,PINRS);
	set_port_data(GLCD_STARTLINE+Start_Line);
	pin_en_active(PORTEN,PINEN);
}

/***************************************************************
 *** Description		: Write one byte to GLCD at current position x,y
 *** Input				: Data
 *** Output				: NONE
 ****************************************************************/
void glcd_write_data(uint8_t Data)
{
	glcd_wait();
	glcd_set_dir(1);
	pin_low(PORTRW,PINRW);
	pin_high(PORTRS,PINRS);
	set_port_data(Data);
	pin_en_active(PORTEN,PINEN);
}

/***************************************************************
 *** Description		: Initialize GLCD
 *** Input				: NONE
 *** Output				: NONE
 ****************************************************************/
void glcd_init(void)
{
	glcd_setside(0);//Select First Chip KS0108
	glcd_set_display(1);//Turn On display
	glcd_set_xadd(0);
	glcd_set_yadd(0);
	glcd_set_start_line(0);

	glcd_setside(1);//Select Second Chip KS0108
	glcd_set_display(1);//Turn On display
	glcd_set_xadd(0);
	glcd_set_yadd(0);
	glcd_set_start_line(0);

	glcd_setside(2);//Select First Chip KS0108
	glcd_set_display(1);//Turn On display
	glcd_set_xadd(0);
	glcd_set_yadd(0);
	glcd_set_start_line(0);

	glcd_clear();
}

/***************************************************************
 *** Description		: Set Cursor position
 *** Input				: x is row (from 0 to 7), y is column (from 0 to 191)
 *** Output				: NONE
 ****************************************************************/
void glcd_goto_xy(uint8_t x,uint8_t y)
{
	uint8_t side;
	side=y/64;//select chip ks0108
	y=y-(64*side);//Get the correct column
	glcd_setside(side);
	glcd_set_xadd(x);
	glcd_set_yadd(y);
}

/***************************************************************
 *** Description		: Clear the GLCD
 *** Input				: NONE
 *** Output				: NONE
 ****************************************************************/
void glcd_clear()
{
	uint8_t i,j;
	for(j=0;j<8;j++)
	{
		for(i=0;i<192;i++)
		{
			glcd_goto_xy(j,i);
			glcd_write_data(0x00);
		}
	}
}

/***************************************************************
 *** Description		: Write a character 8x6(8bit row and 6 bit column) to GLCD at specific position, Important :)
 *** Input				: Position to write: x is row(0-7),y is column (0-191), data to write
 *** Output				: NONE
 ****************************************************************/
void glcd_putchar8x6(uint8_t x,uint8_t y,uint8_t data)
{
	uint8_t i;
	for(i=0;i<6;i++)
	{
		glcd_goto_xy(x,y++);
		glcd_write_data(font8x6[(data-32)*6+i]);
	}
}

/***************************************************************
 *** Description		: Write a string 8x6(8bit row and 6 bit column) to GLCD at specific position, Important :)
 *** Input				: Position to write: x is row(0-7),y is column (0-191), String to write
 *** Output				: NONE
 ****************************************************************/
void glcd_puts8x6(uint8_t x,uint8_t y,uint8_t* string)
{
	uint8_t i=0;
	while(*(string+i)&&(i<=32))
	{
		glcd_putchar8x6(x,y+6*i,*(string+i));
		i++;
	}
}

/***************************************************************
 *** Description	:   Write a character 16x10(16bit row and 10 bit column) to GLCD at specific position, Important :)
 *** Input			: 	Position to write: x is row(0-7),y is column (0-191), data to write
 *** Output			:	NONE
 ****************************************************************/
void glcd_putchar16x10(uint8_t x,uint8_t y,uint8_t data)
{
	uint8_t i,j;
	for(j=0;j<10;j++)//10 column
	{
		for(i=0;i<2;i++)// 2 page
		{
			glcd_goto_xy(x+i,y);
			glcd_write_data(font16x10[(data-32)*20+j*2+i]);
		}
		y++;
	}
}

/***************************************************************
 *** Description		: Write a string 16x10(16bit row and 10 bit column) to GLCD at specific position, Important :)
 *** Input				: Position to write: x is row(0-7),y is column (0-191), String to write
 *** Output				: NONE
 ****************************************************************/
void glcd_puts16x10(uint8_t x, uint8_t y, uint8_t* string)
{
	uint8_t i=0;
	while(*(string+i)&&(i<=19))
	{
		glcd_putchar16x10(x,y+10*i,*(string+i));
		i++;
	}
}

/***************************************************************
 *** Description	:   Write a character 24x14(24 bit row and 14 bit column) to GLCD at specific position, Important :)
 *** Input			: 	Position to write: x is row(0-7),y is column (0-191), data to write
 *** Output			:	NONE
 ****************************************************************/
void glcd_putchar24x14(uint8_t x,uint8_t y,uint8_t data)
{
	uint8_t i,j;
	for(j=0;j<14;j++)
	{
		for(i=0;i<3;i++)
		{
			glcd_goto_xy(x+i,y+j);
			glcd_write_data(font24x14[(data-32)*42+j*3+i]);
		}
	}
}

/***************************************************************
 *** Description		: Write a string 24x14(24 bit row and 14 bit column) to GLCD at specific position, Important :)
 *** Input				: Position to write: x is row(0-7),y is column (0-191), String to write
 *** Output				: NONE
 ****************************************************************/
void glcd_puts24x14(uint8_t x,uint8_t y,uint8_t* string)
{
	uint8_t i=0;
	while(*(string+i)&&(i<=12))
	{
		glcd_putchar24x14(x,y+i*14,*(string+i));
		i++;
	}
}

/***************************************************************
 *** Description	:   Write a character 40x24(40 bit row and 24 bit column) to GLCD at specific position, Important :)
 *** Input			: 	Position to write: x is row(0-7),y is column (0-191), data to write
 *** Output			:	NONE
 ****************************************************************/
void glcd_putchar40x23(uint8_t x,uint8_t y,uint8_t data)
{
	uint8_t i,j;
	for(j=0;j<24;j++)
	{
		for(i=0;i<5;i++)
		{
			glcd_goto_xy(x+i,y+j);
			glcd_write_data(font40x23[(data-32)*115+j*5+i]);
		}
	}
}


/***************************************************************
 *** Description		: Write a string 24x14(24 bit row and 14 bit column) to GLCD at specific position, Important :)
 *** Input				: Position to write: x is row(0-7),y is column (0-191), String to write
 *** Output				: NONE
 ****************************************************************/
void glcd_puts40x23(uint8_t x,uint8_t y,uint8_t* string)
{
	uint8_t i=0;
	while(*(string+i)&&(i<=8))
	{
		glcd_putchar40x23(x,y+i*23,*(string+i));
		i++;
	}
}

