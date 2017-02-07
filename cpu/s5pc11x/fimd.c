/*
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *              http://www.samsung.com/
 *
 * S5PC110 - LCD Driver for U-Boot
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <common.h>
#include <asm/io.h>
#include <asm/types.h>

#define LCD_BGCOLOR		0x1428A0

static unsigned int gFgColor = 0xFF;
static unsigned int gLeftColor = LCD_BGCOLOR;

#define Inp32(_addr)		readl(_addr)
#define Outp32(addr, data)	(*(volatile u32 *)(addr) = (data))
#define Delay(_a)		udelay(_a*1000)


#if defined(CFG_LCD_TL2796)
/*
 * TL2796
 */

#define LCD_WIDTH		480
#define LCD_HEIGHT		800

#define IIC_SDA_GPIO_ADDRESS		(0xE02000C0)	//GPB
#define IIC_SDA_GPIO_BIT			(0)		//GPB[7]
#define IIC_SCL_GPIO_ADDRESS		(0xE02000C0)	//GPB
#define IIC_SCL_GPIO_BIT			(1)		//GPB[4]
#define SDA_GPIO_ADDRESS			(0xE0200040)	//GPB
#define SDA_GPIO_BIT				(7)		//GPB[7]
#define SCL_GPIO_ADDRESS			(0xE0200040)	//GPB
#define SCL_GPIO_BIT				(4)		//GPB[4]
#define CS_GPIO_ADDRESS				(0xE0200040)	//GPB
#define CS_GPIO_BIT					(5)		//GPB[5]
#define BACKLIGHT_GPIO_ADDRESS		(0xE02000A0)	//GPD0
#define BACKLIGHT_GPIO_BIT			(3)		//GPD0[3]
#define RESET_GPIO_ADDRESS			(0xE0200C00)	//GPH0
#define RESET_GPIO_BIT				(6)		//GPH0.6
#define POWER_GPIO_ADDRESS			(0xE0200040)
#define POWER_GPIO_BIT				(6)
#define POWER_GPIO_ADDRESS2			(0xE0200040)
#define POWER_GPIO_BIT2				(7)
#define BACKLIGHT_ACTIVE 			HIGH_ACTIVE
#define RESET_ACTIVE 				LOW_ACTIVE
#define POWER_ACTIVE 				HIGH_ACTIVE

//do not modify
#define LCDM_SDA_HIGH			Outp32(SDA_GPIO_ADDRESS+4,(Inp32(SDA_GPIO_ADDRESS+4) |= (1<<SDA_GPIO_BIT)))
#define LCDM_SDA_LOW			Outp32(SDA_GPIO_ADDRESS+4,(Inp32(SDA_GPIO_ADDRESS+4) &= ~(1<<SDA_GPIO_BIT)))
#define LCDM_SCL_HIGH			Outp32(SCL_GPIO_ADDRESS+4,(Inp32(SCL_GPIO_ADDRESS+4) |= (1<<SCL_GPIO_BIT)))
#define LCDM_SCL_LOW			Outp32(SCL_GPIO_ADDRESS+4,(Inp32(SCL_GPIO_ADDRESS+4) &= ~(1<<SCL_GPIO_BIT)))
#define LCDM_CS_HIGH			Outp32(CS_GPIO_ADDRESS+4,(Inp32(CS_GPIO_ADDRESS+4) |=(1<<CS_GPIO_BIT)))
#define LCDM_CS_LOW				Outp32(CS_GPIO_ADDRESS+4,(Inp32(CS_GPIO_ADDRESS+4) &= ~(1<<CS_GPIO_BIT)))
#define LCDM_BACKLIGHT_HIGH 	Outp32(BACKLIGHT_GPIO_ADDRESS+4,(Inp32(BACKLIGHT_GPIO_ADDRESS+4) |= (1<<BACKLIGHT_GPIO_BIT)))
#define LCDM_BACKLIGHT_LOW	Outp32(BACKLIGHT_GPIO_ADDRESS+4,(Inp32(BACKLIGHT_GPIO_ADDRESS+4) &= ~(1<<BACKLIGHT_GPIO_BIT)))
#define LCDM_RESET_HIGH		 	Outp32(RESET_GPIO_ADDRESS+4,(Inp32(RESET_GPIO_ADDRESS+4) |= (1<<RESET_GPIO_BIT)))
#define LCDM_RESET_LOW			Outp32(RESET_GPIO_ADDRESS+4,(Inp32(RESET_GPIO_ADDRESS+4) &= ~(1<<RESET_GPIO_BIT)))
#define LCDM_POWER_HIGH		 	Outp32(POWER_GPIO_ADDRESS+4,(Inp32(POWER_GPIO_ADDRESS+4) |= (1<<POWER_GPIO_BIT)))
#define LCDM_POWER_LOW			Outp32(POWER_GPIO_ADDRESS+4,(Inp32(POWER_GPIO_ADDRESS+4) &= ~(1<<POWER_GPIO_BIT)))
#define LCDM_POWER2_HIGH	 	Outp32(POWER_GPIO_ADDRESS+4,(Inp32(POWER_GPIO_ADDRESS+4) |= (1<<POWER_GPIO_BIT)))
#define LCDM_POWER2_LOW	 		Outp32(POWER_GPIO_ADDRESS+4,(Inp32(POWER_GPIO_ADDRESS+4) &= ~(1<<POWER_GPIO_BIT)))
#define LCDM_IIC_SCL_HIGH		Outp32(SCL_GPIO_ADDRESS+4,(Inp32(SCL_GPIO_ADDRESS+4) |= (1<<SCL_GPIO_BIT)))
#define LCDM_IIC_SCL_LOW		Outp32(SCL_GPIO_ADDRESS+4,(Inp32(SCL_GPIO_ADDRESS+4) &= ~(1<<SCL_GPIO_BIT)))
#define LCDM_IIC_SDA_HIGH		Outp32(SDA_GPIO_ADDRESS+4,(Inp32(SDA_GPIO_ADDRESS+4) |= (1<<SDA_GPIO_BIT)))
#define LCDM_IIC_SDA_LOW		Outp32(SDA_GPIO_ADDRESS+4,(Inp32(SDA_GPIO_ADDRESS+4) &= ~(1<<SDA_GPIO_BIT)))


void Write_LDI_TL2796(u16 uAddr, u16 uData)
{
	s32 j;

	if(uAddr != 0x0000)
	{
		LCDM_CS_HIGH;
		LCDM_SCL_HIGH;
		LCDM_SDA_HIGH;

		LCDM_CS_LOW;
		LCDM_SCL_LOW;

	    // Command
	    for (j = 15 ; j >= 0; j--)
		{
			LCDM_SCL_LOW;

			if ((uAddr >> j) & 0x0001)
			{
				LCDM_SDA_HIGH;
			}
			else
			{
				LCDM_SDA_LOW;
			}
			LCDM_SCL_HIGH;
		}
	}

	if (uAddr == 0x0000)
	{
		LCDM_SDA_HIGH;
	}

	LCDM_CS_HIGH;
	LCDM_SCL_HIGH;
	LCDM_CS_LOW;
	LCDM_SCL_LOW;

	// parameter
	for (j = 15 ; j >= 0; j--)
	{
		LCDM_SCL_LOW;

		if ((uData >> j) & 0x0001)
		{
			LCDM_SDA_HIGH;
		}
		else
		{
			LCDM_SDA_LOW;
		}
		LCDM_SCL_HIGH;
	}
}

void LCDM_InitTL2796(void)
{
    Delay(50); // more than 20ms

	//[1] Panel Condition Set
	Write_LDI_TL2796(0x7031, 0x7208);
	Write_LDI_TL2796(0x7032, 0x7214);
	Write_LDI_TL2796(0x7030, 0x7202);

#if 1
	Write_LDI_TL2796(0x7027, 0x7203);
#else
	Write_LDI_TL2796(0x7027, 0x7201);
#endif

	//[2] Display Condition Set(RGB Data Interface)
	Write_LDI_TL2796(0x7012, 0x7208); //0x7208
	Write_LDI_TL2796(0x7013, 0x7208);
#if 1
	Write_LDI_TL2796(0x7015, 0x7210); //0x10
#else
	Write_LDI_TL2796(0x7015, 0x7200); //0x00
#endif
	Write_LDI_TL2796(0x7016, 0x7200);	//00 24bit 02 16bit RGB interface 
	Write_LDI_TL2796(0x70ef, 0x72d0);
	Write_LDI_TL2796(0x0000, 0x72e8);

	// set gamma control 230cd
	Write_LDI_TL2796(0x7039, 0x7244);
	Write_LDI_TL2796(0x7040, 0x7200);
	Write_LDI_TL2796(0x7041, 0x723F);
	Write_LDI_TL2796(0x7042, 0x722B);
	Write_LDI_TL2796(0x7043, 0x721f);
	Write_LDI_TL2796(0x7044, 0x7224);
	Write_LDI_TL2796(0x7045, 0x721b);
	Write_LDI_TL2796(0x7046, 0x7229);
	Write_LDI_TL2796(0x7050, 0x7200);
	Write_LDI_TL2796(0x7051, 0x7200);
	Write_LDI_TL2796(0x7052, 0x7200);
	Write_LDI_TL2796(0x7053, 0x721b);
	Write_LDI_TL2796(0x7054, 0x7222);
	Write_LDI_TL2796(0x7055, 0x721b);
	Write_LDI_TL2796(0x7056, 0x722a);
	Write_LDI_TL2796(0x7060, 0x7200);
	Write_LDI_TL2796(0x7061, 0x723F);
	Write_LDI_TL2796(0x7062, 0x7225);
	Write_LDI_TL2796(0x7063, 0x721c);
	Write_LDI_TL2796(0x7064, 0x7221);
	Write_LDI_TL2796(0x7065, 0x7218);
	Write_LDI_TL2796(0x7066, 0x723e);

	//Analog Power Condition Set
	Write_LDI_TL2796(0x7017, 0x7222);	//Boosting Freq
	Write_LDI_TL2796(0x7018, 0x7233);	//power AMP Medium
	Write_LDI_TL2796(0x7019, 0x7203);	//Gamma Amp Medium
	Write_LDI_TL2796(0x701a, 0x7201);	//Power Boosting
	Write_LDI_TL2796(0x7022, 0x72a4);	//Vinternal = 0.65*VCI
	Write_LDI_TL2796(0x7023, 0x7200);	//VLOUT1 Setting = 0.98*VCI
	Write_LDI_TL2796(0x7026, 0x72a0);	//Display Condition LTPS signal generation : Reference= DOTCLK

	//Stand-by Off Comman
	Write_LDI_TL2796(0x701d, 0x72a0);

	Delay(500); // more than 250ms

	//LCD ON
	Write_LDI_TL2796(0x7014, 0x7203);
}

void LCD_Initialize_TL2796(void)
{
	u32 uFbAddr = CFG_LCD_FBUFFER;

	u32 i,uTmpReg;
	u32* pBuffer = (u32*)uFbAddr;


	LCD_setprogress(0);


	uTmpReg = Inp32(0xE0107008);		// need to be changed later (09.01.23 WJ.Kim)	
	uTmpReg = (uTmpReg & ~(0x3)) | 0x2 ;
	Outp32(0xE0107008, uTmpReg);

	uTmpReg = Inp32(0xf800027c);
	uTmpReg = (uTmpReg & ~(0x3)) | 0 ;
	Outp32(0xf800027c,uTmpReg);


	Outp32(0xE0200120, 0x22222222);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	Outp32(0xE0200128,0x0);			//set pull-up,down disable
	Outp32(0xE0200140, 0x22222222);	//set GPF1 as VD[11:4]
	Outp32(0xE0200148,0x0);			//set pull-up,down disable
	Outp32(0xE0200160, 0x22222222);	//set GPF2 as VD[19:12]
	Outp32(0xE0200168,0x0);			//set pull-up,down disable
	Outp32(0xE0200180, 0x00002222);	//set GPF3 as VD[23:20]
	Outp32(0xE0200188,0x0);			//set pull-up,down disable

	Outp32(0xe02000A0, 0x11111111);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	Outp32(0xe02000A8,0x0);			//set pull-up,down disable
	Outp32(0xe02000A4,(1<<3));

	Outp32(0xE0200040, 0x11111111);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	Outp32(0xE0200048,0x0);			//set pull-up,down disable
	Outp32(0xE0200044,0);

	Outp32(0xE02000c0, 0x11111111);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	Outp32(0xE02000c8,0x0);			//set pull-up,down disable
	Outp32(0xE02000c4,0);

	Outp32(0xE0200C00, 0x11111111);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	Outp32(0xE0200C08,0x0);			//set pull-up,down disable
	Outp32(0xE0200C04,0);
	Delay(50);
	Outp32(0xE0200C04,0xffffffff);

	Outp32(SDA_GPIO_ADDRESS+4,(Inp32(SDA_GPIO_ADDRESS+4) |= (1<<SDA_GPIO_BIT)));
	Outp32(SCL_GPIO_ADDRESS+4,(Inp32(SCL_GPIO_ADDRESS+4) |= (1<<SCL_GPIO_BIT)));
	Outp32(CS_GPIO_ADDRESS+4,(Inp32(CS_GPIO_ADDRESS+4) |=(1<<CS_GPIO_BIT)));
	Outp32(SCL_GPIO_ADDRESS+4,(Inp32(SCL_GPIO_ADDRESS+4) |= (1<<SCL_GPIO_BIT)));
	Outp32(SDA_GPIO_ADDRESS+4,(Inp32(SDA_GPIO_ADDRESS+4) |= (1<<SDA_GPIO_BIT)));

	Outp32(0xf8000004, 0xf0);
	Outp32(0xf8000010, 0x50505);
	Outp32(0xf8000014, 0x50505);
	Outp32(0xf8000170, 0x0);
	Outp32(0xf8000018, 0x18f9df);
	Outp32(0xf8000000, 0x0);
	Outp32(0xf8000000, 0x190);
	Outp32(0xf8000130, 0x20);
	Outp32(0xf8000020, 0x0);
	Outp32(0xf8000024, 0x0);
	Outp32(0xf8000028, 0x0);
	Outp32(0xf800002c, 0x0);
	Outp32(0xf8000030, 0x0);
	Outp32(0xf8000034, 0x0);
	Outp32(0xf8000180, 0x0);
	Outp32(0xf8000184, 0x0);
	Outp32(0xf8000188, 0x0);
	Outp32(0xf800018c, 0x0);
	Outp32(0xf8000190, 0x0);
	Outp32(0xf8000140, 0x0);
	Outp32(0xf8000148, 0x0);
	Outp32(0xf8000150, 0x0);
	Outp32(0xf8000158, 0x0);
	Outp32(0xf8000058, 0x0);
	Outp32(0xf8000208, 0x0);
	Outp32(0xf800020c, 0x0);
	Outp32(0xf8000068, 0x0);
	Outp32(0xf8000210, 0x0);
	Outp32(0xf8000214, 0x0);
	Outp32(0xf8000078, 0x0);
	Outp32(0xf8000218, 0x0);
	Outp32(0xf800021c, 0x0);
	Outp32(0xf8000088, 0x0);
	Outp32(0xf8000220, 0x0);
	Outp32(0xf8000224, 0x0);
	Outp32(0xf8000260, 0x1);
	Outp32(0xf8000034, 0x0);
	Outp32(0xf80000a4, uFbAddr + 0x0);
	Outp32(0xf80000d4, uFbAddr + 0x177000);
	Outp32(0xf80000a0, uFbAddr + 0x0);
	Outp32(0xf80000d0, uFbAddr + 0x177000);
	Outp32(0xf80020a0, uFbAddr + 0x0);
	Outp32(0xf80020d0, uFbAddr + 0x177000);
	Outp32(0xf8000100, 0x780);
	Outp32(0xf8000020, 0x8000);
	Outp32(0xf8000020, 0x802c);
	Outp32(0xf8000040, 0x0);
	Outp32(0xf8000044, 0xefb1f);
	Outp32(0xf8000200, 0xffffff);
	Outp32(0xf8000204, 0xffffff);
	Outp32(0xf8000034, 0x400);
	Outp32(0xf8000020, 0x802d);
	Outp32(0xf8000034, 0x1);
	Outp32(0xf8000034, 0x1);
	Outp32(0xf8000034, 0x401);
	Outp32(0xf8000020, 0x802d);
	Outp32(0xf8000034, 0x1);
	Outp32(0xf8000034, 0x1);

	Outp32(0xf8000000, 0x193);

//	LCDM_InitLdi(LCD_TL2796, 0);
	LCDM_InitTL2796();
}
#elif defined(CFG_LCD_NONAME1)
/*
 *
 */

#define LCD_WIDTH		800 //1366
#define LCD_HEIGHT		480 //500	//766

void LCD_Initialize_NONAME1(void)
{

//bluesky
    //printf("LCD Initializing...... %s,%d\n",__func__,__LINE__);//bs
    u32 uFbAddr = CFG_LCD_FBUFFER;
    u32 i;
    u32* pBuffer = (u32*)uFbAddr;

    Outp32(0xe0200040, 0x10000000);  //GPBCON [31:28]:output [27:0]:input
    Outp32(0xe0200048, 0x1555);  //GPBPUD GPBPUD[7]:Pull-up/ down disabled GPBPUD[6:0]:Pull-down enabled
    Outp32(0xe020004c, 0xc000);  //GPBDRV GPBDRV[7]:4x  GPBDRV[6:0]:1x
    Outp32(0xe0200040, 0x10010000); //GPBCON [31:28],[19:16]:output [27:20],[15:0]:input
    Outp32(0xe0200048, 0x1455); //GPBPUD GPBPUD[7],[4]:Pull-up/ down disabled ,GPBPUD[6:5][3:0]:Pull-down enabled
    Outp32(0xe020004c, 0xc300);  //GPBDRV GPBDRV[7],[4]:4x  GPBDRV[6:5][3:0]:1x
    Outp32(0xe0200040, 0x10110000); //GPBCON [31:28],[23:20],[19:16]:output [27:24],[15:0]:input
    Outp32(0xe0200048, 0x1055);  //GPBPUD GPBPUD[7],[5][4]:Pull-up/ down disabled ,GPBPUD[6][3:0]:Pull-down enabled
    Outp32(0xe020004c, 0xcf00); //GPBDRV GPBDRV[7],[5],[4]:4x  GPBDRV[6][3:0]:1x
    Outp32(0xe02000c0, 0x1); //GPD1CON [23:4]:input [3:0]:output
    Outp32(0xe02000c8, 0x54); //GPD1PUD GPD1PUD[5:4],[0]:Pull-up/ down disabled ,GPBPUD[3:1]:Pull-down enabled
    Outp32(0xe02000cc, 0x3); //GPD1DRV GPD1DRV[0]:4x  GPBDRV[5:1]:1x
    Outp32(0xe02000c0, 0x11); //GPD1CON [23:8]:input [7:0]:output
    Outp32(0xe02000c8, 0x50); //GPD1PUD GPD1PUD[5:4],[1:0]:Pull-up/ down disabled ,GPBPUD[3:2]:Pull-down enabled
    Outp32(0xe02000cc, 0xf); //GPD1DRV GPD1DRV[1:0]:4x  GPBDRV[5:2]:1x
//gpd0 pwm 0
//    Outp32(0xe02000a0, 0x1001); //GPD0CON GPD0CON[3]，[0]:output GPD0CON[2:1]:input
//    Outp32(0xe02000a8, 0x15);  //GPD0PUD GPD0PUD[3]:Pull-up/ down disabled,GPD0PUD[2:0]:Pull-down enabled
//    Outp32(0xe02000ac, 0xc0); //GPD0DRV GPD0DRV[3]:4x,GPD0DRV[2:0]:1x
//gpd0
//gpd0_1 pwm1
    Outp32(0xe02000a0, 0x1010); //GPD0CON GPD0CON[3]，[0]:output GPD0CON[2:1]:input
    Outp32(0xe02000a8, 0x15);  //GPD0PUD GPD0PUD[3]:Pull-up/ down disabled,GPD0PUD[2:0]:Pull-down enabled
    Outp32(0xe02000ac, 0xc0); //GPD0DRV GPD0DRV[3]:4x,GPD0DRV[2:0]:1x
//gpd0_1
    Outp32(0xe0200c00, 0x1000010); // GPH0CON GPH0CON[6],[1]:output,GPH0CON[7],[5:2],[0]:input
    Outp32(0xe0200c08, 0x4455);  // GPH0PUD GPH0PUD[6],[4]:Pull-up/ down disabled GPH0PUD[7],[5],[4:0]:Pull-down enabled
    Outp32(0xe0200c0c, 0x3000);  // GPH0DRV GPH0DRV[6]:4x GPH0DRV[7],[5:0]:1x
    Outp32(0xe0200040, 0x11110000); //GPBCON [31:16]:output [15:0]:input
    Outp32(0xe0200048, 0x55);  //GPBPUD GPBPUD[7:4]:Pull-up/ down disabled GPBPUD[3:0]:Pull-down enabled
    Outp32(0xe020004c, 0xff00);  //GPBDRV GPBDRV[7:4]:4x  GPBDRV[3:0]:1x
    Outp32(0xe0200040, 0x11110100);  //GPBCON [31:16],[11:8]:output [15:12],[7:0]:input
    Outp32(0xe0200048, 0x55);  //GPBPUD GPBPUD[7:4]:Pull-up/ down disabled GPBPUD[3:0]:Pull-down enabled
    Outp32(0xe020004c, 0xff00);  //GPBDRV GPBDRV[7:4]:4x  GPBDRV[3:0]:1x
    Outp32(0xe0200044, 0x80);  //GPBDAT GPBDAT[7]=1,GPBDAT[6:0]=0
    Outp32(0xe0200044, 0x98);  //GPBDAT GPBDAT[7],[4:3]=1,GPBDAT[6:5],[2:0]=0
    Outp32(0xe0200044, 0xb9);  //GPBDAT GPBDAT[7],[5:3],[0]=1,GPBDAT[6],[2:1]=0
    Outp32(0xe0200044, 0xbb);  //GPBDAT GPBDAT[7],[5:3],[1:0]=1,GPBDAT[6],[2]=0
    Outp32(0xe0200044, 0xbb);  //GPBDAT GPBDAT[7],[5:3],[1:0]=1,GPBDAT[6],[2]=0
//pwm0
    //Outp32(0xe02000a4, 0xd); //GPD0DAT GPD0DAT[3:2],[0]=1,GPD0DAT[1]=0
//pwm0
//pwm1 gpd0_1
    Outp32(0xe02000a4, 0xe); //GPD0DAT GPD0DAT[3:2],[0]=1,GPD0DAT[1]=0
//pwm1 gpd0_1
    Outp32(0xe0200c04, 0xd1);  //GPH0DAT[7:6],[4],[0]=1,GPH0DAT[5],[3:1]=0
    Outp32(0xe0200044, 0xfb);  //GPBDAT GPBDAT[7:3],[1:0]=1,GPBDAT[2]=0
    Outp32(0xe0200044, 0xff);  //GPBDAT GPBDAT[7:0]=1
    Outp32(0xe0200c04, 0x91);  //GPH0DAT[7],[4],[0]=1,GPH0DAT[6:5],[3:1]=0
    Outp32(0xe0200c04, 0xd1);  //GPH0DAT[7:6],[4],[0]=1,GPH0DAT[5],[3:1]=0
    Outp32(0xe0200c04, 0xd3);  //GPH0DAT[7:6],[4],[1:0]=1,GPH0DAT[5],[3:2]=0

    Outp32(0xe0200120, 0x22222222);//set GPF0[0:7] as HSYNC,VSYNC,VDEN,VCLK,VD[0:3]
    Outp32(0xe0200128,0x0);//set pull-up,down disable
    Outp32(0xe0200140, 0x22222222);//set GPF1CON[7:0]  as VD[11:4]
    Outp32(0xe0200148,0x0);//set pull-up,down disable
    Outp32(0xe0200160, 0x22222222);//set GPF2CON[7:0] as VD[19:12]
    Outp32(0xe0200168,0x0);//set pull-up,down disable
    Outp32(0xe0200180, 0x00002222);//set GPF3CON[3:0]  as VD[23:20]
    Outp32(0xe0200188,0x0);//set pull-up,down disable

    //--------- S5PC110 EVT0 needs MAX drive strength---------//
    Outp32(0xe020012c,0xffffffff);//set GPF0 drive strength max by WJ.KIM(09.07.17)
    Outp32(0xe020014c,0xffffffff);//set GPF1 drive strength max by WJ.KIM(09.07.17)
    Outp32(0xe020016c,0xffffffff);//set GPF2 drive strength max by WJ.KIM(09.07.17)
    Outp32(0xe020018c,0x3ff);//set GPF3 drive strength max by WJ.KIM(09.07.17)

    Outp32(0xe0107008,0x2); //DISPLAY_CONTROL output path RGB=FIMD I80=FIMD ITU=FIMD
    Outp32(0xe0100204,0x700000); //CLK_SRC1 fimdclk = EPLL
    Outp32(0xf8000004, 0x60);  //VIDCON1  HSYNC pulse Inverted,VSYNC pulse Inverted
    // Outp32(0xf8000010, 0xe0e0305);  //VIDTCON0  VBPDE=14,VBPD=14,VFPD=3,VSPW=5
    Outp32(0xf8000010, 0x60400);  //VIDTCON0  VBPDE=0,VBPD=6,VFPD=4,VSPW=0
    Outp32(0xf8000014, 0x3103020);  //VIDTCON1  VFPDE=3,HBPD=16,HFPD=48,HSPW=32
    // Outp32(0xf8000170, 0x0);  //DITHMODE  Disables dithering
    Outp32(0xf8000018, 0xefb1f);  //VIDTCON2   0xefb1f:HOZVAL=799  LINEVAL=479,800*480;     0x17fd55:HOZVAL=1365  LINEVAL=767,1366*768;
    // Outp32(0xf8000000, 0x0);  //VIDCON0 RGB interface 0 + RGB parallel format + Normal: RGBORDER[2] @VIDCON3 + VCLK Free Run :Normal mode
                                      //video clock source:HCLK
    // Outp32(0xf8000000, 0x254); //VIDCON0
    // Outp32(0xf8000130, 0x20);  //VIDINTCON0 FIFO Interrupt Window 0 control = enables
    Outp32(0xf8000020, 0x0);  //WINCON0 Window 0 Buffer set 0
    Outp32(0xf8000024, 0x0);  //WINCON1 Window 1 Buffer set 0
    Outp32(0xf8000028, 0x0);  //WINCON2 Window 2 Buffer set 0
    Outp32(0xf800002c, 0x0);  //WINCON3 Window 3 Buffer set 0
    Outp32(0xf8000030, 0x0);  //WINCON4 Window 4 Buffer set 0
    Outp32(0xf8000034, 0x0);
    Outp32(0xf8000180, 0x0);
    Outp32(0xf8000184, 0x0);
    Outp32(0xf8000188, 0x0);
    Outp32(0xf800018c, 0x0);
    Outp32(0xf8000190, 0x0);
    Outp32(0xf8000140, 0x0);
    Outp32(0xf8000148, 0x0);
    Outp32(0xf8000150, 0x0);
    Outp32(0xf8000158, 0x0);
    Outp32(0xf8000058, 0x0);
    Outp32(0xf8000208, 0x0);
    Outp32(0xf800020c, 0x0);
    Outp32(0xf8000068, 0x0);
    Outp32(0xf8000210, 0x0);
    Outp32(0xf8000214, 0x0);
    Outp32(0xf8000078, 0x0);
    Outp32(0xf8000218, 0x0);
    Outp32(0xf800021c, 0x0);
    Outp32(0xf8000088, 0x0);
    Outp32(0xf8000220, 0x0);
    Outp32(0xf8000224, 0x0);  
    // Outp32(0xf8000260, 0x1);  //BLENDCON BLEND_NEW=8-bit alpha value
    // Outp32(0xf8000200, 0xffffff); //VIDW0ALPHA0  Window 0 Alpha0
    // Outp32(0xf8000204, 0xffffff);  //VIDW0ALPHA1  Window 0 Alpha1
    // Outp32(0xf8000034, 0x0);
    // Outp32(0xf8000020, 0x802c);
    Outp32(0xf80000a0, uFbAddr + 0x00000000);
    Outp32(0xf80000d0, uFbAddr + 0x00177000);  //buffer size  0x00177000:800*480*4   0x00400800:1366*768*4
    // Outp32(0xf80000a4, uFbAddr + 0x00000000);
    // Outp32(0xf80000d4, uFbAddr + 0x00400800);
    // Outp32(0xf80020a0, uFbAddr + 0x00000000);
    // Outp32(0xf80020d0, uFbAddr + 0x00400800);
    Outp32(0xf8000100, 0xc80); //VIDW00ADD2  0xc80:PAGEWIDTH_F=800*4 0x1558:PAGEWIDTH_F=1366*4 problem1 forlinx.bs
    Outp32(0xf8000040, 0x0); //VIDOSD0A 配置窗口位置寄存器A 和 B win0左上角的X、Y坐标(0,0)
    Outp32(0xf8000044, 0x18f9df);//VIDOSD0B win0右下角角的X、Y坐标(799,479) //problem3
    Outp32(0xf8000048, 0x5dc00);  //VIDOSD0C  0x5dc00:OSDSIZE=800*480    0x100200:OSDSIZE=1366*768 //problem2
    // Outp32(0xf8000020, 0x802d);
    // Outp32(0xf8000034, 0x1);
    Outp32(0xf8000020, 0x802d); //WINCON0 ENWIN_F=1,ALPHA_SEL_F=0,BPPMODE_F=Unpacked 24 bpp,WSWP_F =1
    Outp32(0xf8000034, 0x1);  //VIDOSD0C  C0_EN_F=Enables Channel 0
    // Outp32(0xf8000000, 0x257);
    // Outp32(0xf8000000, 0x57); //===> MPLL should be 667 !!!!
    Outp32(0xf8000000, 0x53); //ENVID_F=1,ENVID=1,CLKDIR=Divided by CLKVAL_F ,CLKVAL_F=1
    Outp32(0xf80001a4, 0x3);  //TRIGCON

//bluesky

	LCD_setprogress(0);

}
#else
// "No LCD Type is defined!"
#endif

/*
 */
void LCD_turnon(void)
{
#if defined(CFG_LCD_TL2796)
	LCD_Initialize_TL2796();
#elif defined(CFG_LCD_NONAME1)
	LCD_Initialize_NONAME1();
#endif
}

void LCD_setfgcolor(unsigned int color)
{
	gFgColor = color;
}

void LCD_setleftcolor(unsigned int color)
{
	gLeftColor = color;
}

void LCD_setprogress(int percentage)
{
#if defined(CFG_LCD_TL2796) || defined(CFG_LCD_NONAME1)
	u32 i, j;
	u32* pBuffer = (u32*)CFG_LCD_FBUFFER;

	for (i=0; i < (LCD_HEIGHT/100)*percentage; i++)
	{
		for (j=0; j < LCD_WIDTH; j++)
		{
			*pBuffer++ = gFgColor;
		}
	}

	for (; i < LCD_HEIGHT; i++)
	{
		for (j=0; j < (LCD_WIDTH >> 5); j++)
		{
			*pBuffer++ = gLeftColor;
		}
		for (; j < LCD_WIDTH; j++)
		{
			*pBuffer++ = LCD_BGCOLOR;
		}
	}
#endif
}
