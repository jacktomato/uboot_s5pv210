/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, <d.mueller@elsoft.ch>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <regs.h>
#include <asm/io.h>

//forinx.bs update
//#include <s5pc110.h>
#include <video_fb.h>
#include <asm/arch/regs-fb.h>



/* ------------------------------------------------------------------------- */
#define SMC9115_Tacs	(0x0)	// 0clk		address set-up
#define SMC9115_Tcos	(0x4)	// 4clk		chip selection set-up
#define SMC9115_Tacc	(0xe)	// 14clk	access cycle
#define SMC9115_Tcoh	(0x1)	// 1clk		chip selection hold
#define SMC9115_Tah	(0x4)	// 4clk		address holding time
#define SMC9115_Tacp	(0x6)	// 6clk		page mode access cycle
#define SMC9115_PMC	(0x0)	// normal(1data)page mode configuration

#define SROM_DATA16_WIDTH(x)	(1<<((x*4)+0))
#define SROM_WAIT_ENABLE(x)	(1<<((x*4)+1))
#define SROM_BYTE_ENABLE(x)	(1<<((x*4)+2))

/* ------------------------------------------------------------------------- */

#define DM9000_Tacs	(0x0)	// 0clk		address set-up
#define DM9000_Tcos	(0x4)	// 4clk		chip selection set-up
#define DM9000_Tacc	(0xE)	// 14clk	access cycle
#define DM9000_Tcoh	(0x1)	// 1clk		chip selection hold
#define DM9000_Tah	(0x4)	// 4clk		address holding time
#define DM9000_Tacp	(0x6)	// 6clk		page mode access cycle
#define DM9000_PMC	(0x0)	// normal(1data)page mode configuration


#define CS8900_Tacs	(0x0)	// 0clk		address set-up
#define CS8900_Tcos	(0x3)	// 4clk		chip selection set-up
#define CS8900_Tacc	(0x7)	// 14clk	access cycle
#define CS8900_Tcoh	(0x1)	// 1clk		chip selection hold
#define CS8900_Tah	(0x3)	// 4clk		address holding time
#define CS8900_Tacp	(0x6)	// 6clk		page mode access cycle
#define CS8900_PMC	(0x0)	// normal(1data)page mode configuration
static inline void delay(unsigned long loops)
{
	__asm__ volatile ("1:\n" "subs %0, %1, #1\n" "bne 1b":"=r" (loops):"0"(loops));
}

/*
 * Miscellaneous platform dependent initialisations
 */

static void dm9000_pre_init(void)
{
	unsigned int tmp;

#if defined(DM9000_16BIT_DATA)
	SROM_BW_REG &= ~(0xf << 4);
//	SROM_BW_REG |= (0<<23) | (0<<22) | (0<<21) | (1<<20);
	SROM_BW_REG |= (0<<7) | (0<<6) | (1<<5) | (1<<4);
#else	
	SROM_BW_REG &= ~(0xf << 20);
	SROM_BW_REG |= (0<<19) | (0<<18) | (0<<16);
#endif

//	SROM_BC1_REG = ((0<<28)|(1<<24)|(5<<16)|(1<<12)|(4<<8)|(6<<4)|(0<<0));
//	SROM_BC1_REG = ((DM9000_Tacs<<28)|(DM9000_Tcos<<24)|(DM9000_Tacc<<16)|(DM9000_Tcoh<<12)|(DM9000_Tah<<8)|(DM9000_Tacp<<4)|(DM9000_PMC));
	SROM_BC1_REG = ((CS8900_Tacs<<28)+(CS8900_Tcos<<24)+(CS8900_Tacc<<16)+(CS8900_Tcoh<<12)+(CS8900_Tah<<8)+(CS8900_Tacp<<4)+(CS8900_PMC));
	
	tmp = MP01CON_REG;
	tmp &=~(0xf<<4);
	tmp |=(2<<4);
	MP01CON_REG = tmp;
}

//forlinx.bs
void board_lcd_init()
{
#if 0////debug //spi.forlinx.bs
	writel(0x10000000, GPBCON);     //GPBCON [31:28]:output [27:0]:input
	writel(0x1555,     GPBPUD);//GPBPUD GPBPUD[7]:Pull-up/ down disabled GPBPUD[6:0]:Pull-down enabled
	writel(0xc000,     GPBDRV_SR);//GPBDRV GPBDRV[7]:4x  GPBDRV[6:0]:1x
	writel(0x10010000,GPBCON);//GPBCON [31:28],[19:16]:output [27:20],[15:0]:input
	writel(0x1455,    GPBPUD);//GPBPUD GPBPUD[7],[4]:Pull-up/ down disabled ,GPBPUD[6:5][3:0]:Pull-down enabled
	writel(0xc300,    GPBDRV_SR);//GPBDRV GPBDRV[7],[4]:4x  GPBDRV[6:5][3:0]:1x
	writel(0x10110000,GPBCON);//GPBCON [31:28],[23:20],[19:16]:output [27:24],[15:0]:input
	writel(0x1055, GPBPUD);//GPBPUD GPBPUD[7],[5][4]:Pull-up/ down disabled ,GPBPUD[6][3:0]:Pull-down enabled
	writel(0xcf00, GPBDRV_SR);//GPBDRV GPBDRV[7],[5],[4]:4x  GPBDRV[6][3:0]:1x
#endif //debug
#if 0//i2s.forlinx.bs
	writel(0x1,  GPD1CON);//GPD1CON [23:4]:input [3:0]:output
	writel(0x54, GPD1PUD);//GPD1PUD GPD1PUD[5:4],[0]:Pull-up/ down disabled ,GPBPUD[3:1]:Pull-down enabled
	writel(0x3, GPD1DRV);//GPD1DRV GPD1DRV[0]:4x  GPBDRV[5:1]:1x
	writel(0x11, GPD1CON);//GPD1CON [23:8]:input [7:0]:output
	writel(0x50, GPD1PUD);//GPD1PUD GPD1PUD[5:4],[1:0]:Pull-up/ down disabled ,GPBPUD[3:2]:Pull-down enabled
	writel(0xf, GPD1DRV);//GPD1DRV GPD1DRV[1:0]:4x  GPBDRV[5:2]:1x
#endif//debug//i2c
////gpd0 pwm 0 gpd0_0.forlinx.bs
	//writel(0x1001, GPD0CON);//GPD0CON GPD0CON[3]，[0]:output GPD0CON[2:1]:input  
	//writel(0x15, GPD0PUD);//GPD0PUD GPD0PUD[3]:Pull-up/ down disabled,GPD0PUD[2:0]:Pull-down enabled
	//writel(0xc0, GPD0DRV);//GPD0DRV GPD0DRV[3]:4x,GPD0DRV[2:0]:1x
////gpd0 pwm 0 gad0_0
//gpd0_1 pwm1.forlinx.bs
	writel(0x1010, GPD0CON);//GPD0CON GPD0CON[3]，[0]:output GPD0CON[2:1]:input  
	writel(0x15, GPD0PUD);//GPD0PUD GPD0PUD[3]:Pull-up/ down disabled,GPD0PUD[2:0]:Pull-down enabled
	writel(0xc0, GPD0DRV);//GPD0DRV GPD0DRV[3]:4x,GPD0DRV[2:0]:1x
//gpd0_1 pwm1
#if 0 ////debug XEINT0 .... XEINT7.forlinx.bs
	writel(0x1000010, GPH0CON);// GPH0CON GPH0CON[6],[1]:output,GPH0CON[7],[5:2],[0]:input  
	writel(0x4455, GPH0PUD);// GPH0PUD GPH0PUD[6],[4]:Pull-up/ down disabled GPH0PUD[7],[5],[4:0]:Pull-down enabled
	writel(0x3000, GPH0DRV);// GPH0DRV GPH0DRV[6]:4x GPH0DRV[7],[5:0]:1x
#endif //debug XEINT0 .... XEINT7
#if 0//debug spi.forlinx.bs
	writel(0x11110000, GPBCON);//GPBCON [31:16]:output [15:0]:input
	writel(0x55, GPBPUD);//GPBPUD GPBPUD[7:4]:Pull-up/ down disabled GPBPUD[3:0]:Pull-down enabled
	writel(0xff00, GPBDRV_SR);//GPBDRV GPBDRV[7:4]:4x  GPBDRV[3:0]:1x
	writel(0x11110100, GPBCON);//GPBCON [31:16],[11:8]:output [15:12],[7:0]:input
	writel(0x55, GPBPUD);//GPBPUD GPBPUD[7:4]:Pull-up/ down disabled GPBPUD[3:0]:Pull-down enabled
	writel(0xff00, GPBDRV_SR);//GPBDRV GPBDRV[7:4]:4x  GPBDRV[3:0]:1x
	writel(0x80, GPBDAT);//GPBDAT GPBDAT[7]=1,GPBDAT[6:0]=0  
	writel(0x98, GPBDAT);//GPBDAT GPBDAT[7],[4:3]=1,GPBDAT[6:5],[2:0]=0
	writel(0xb9, GPBDAT);//GPBDAT GPBDAT[7],[5:3],[0]=1,GPBDAT[6],[2:1]=0
	writel(0xbb, GPBDAT);//GPBDAT GPBDAT[7],[5:3],[1:0]=1,GPBDAT[6],[2]=0
	writel(0xbb, GPBDAT);//GPBDAT GPBDAT[7],[5:3],[1:0]=1,GPBDAT[6],[2]=0
#endif//debug spi
	//writel(0xd, GPD0DAT);//GPD0DAT GPD0DAT[3:2],[0]=1,GPD0DAT[1]=0  
////gpd0 pwm 0
////gpd0 pwm 1 gpd0_1
       writel(0xc, GPD0DAT);//GPD0DAT GPD0DAT[3:2],[0]=1,GPD0DAT[1]=0
  ////gpd0 pwm 1 gpd0_1
#if 0//debug.forlinx.bs
	writel(0xd1, GPH0DAT);//GPH0DAT[7:6],[4],[0]=1,GPH0DAT[5],[3:1]=0  
	writel(0xfb, GPBDAT);//GPBDAT GPBDAT[7:3],[1:0]=1,GPBDAT[2]=0
	writel(0xff, GPBDAT);//GPBDAT GPBDAT[7:0]=1
	writel(0x91, GPH0DAT);//GPH0DAT[7],[4],[0]=1,GPH0DAT[6:5],[3:1]=0
	writel(0xd1, GPH0DAT);//GPH0DAT[7:6],[4],[0]=1,GPH0DAT[5],[3:1]=0
	writel(0xd3, GPH0DAT);//GPH0DAT[7:6],[4],[1:0]=1,GPH0DAT[5],[3:2]=0
#endif//debug
	writel(0x22222222, GPF0CON);    //GPF0CON set GPF0[0:7] as HSYNC,VSYNC,VDEN,VCLK,VD[0:3]
	writel(0x0, GPF0PUD);      //GPF0PUD set pull-up,down disable
	writel(0x22222222, GPF1CON);    //set GPF1CON[7:0] as VD[11:4]
	writel(0x0, GPF1PUD);      //GPF1PUD set pull-up,down disable
	writel(0x22222222, GPF2CON);    //set GPF2CON[7:0] as VD[19:12]
	writel(0x0, GPF2PUD);      //GPF2PUD set pull-up,down disable
	writel(0x00002222, GPF3CON);    //set GPF3CON[3:0] as VD[23:20]
	writel(0x0, GPF3PUD);      //GPF3PUD set pull-up,down disable
	//--------- S5PC110 EVT0 needs MAX drive strength---------//
	writel(0xffffffff, GPF0DRV);    //set GPF0DRV drive strength max by WJ.KIM(09.07.17)
	writel(0xffffffff, GPF1DRV);    //set GPF1DRV drive strength max by WJ.KIM(09.07.17)
	writel(0xffffffff, GPF2DRV);    //set GPF2DRV drive strength max by WJ.KIM(09.07.17)
	writel(0x3ff, GPF3DRV);     //set GPF3DRV drive strength max by WJ.KIM(09.07.17)
}
//forlinx.bs
int board_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;
#ifdef CONFIG_DRIVER_SMC911X
	smc9115_pre_init();
#endif

#ifdef CONFIG_DRIVER_DM9000
	dm9000_pre_init();
#endif
#if defined(CONFIG_VIDEO) || defined(CONFIG_CFB_CONSOLE)//forlinx.bs
    board_lcd_init();//forlinx.bs
#endif//forlinx.bs
	gd->bd->bi_arch_number = MACH_TYPE;
	gd->bd->bi_boot_params = (PHYS_SDRAM_1+0x100);

	return 0;
}
//forlinx.bs
void board_video_init(GraphicDevice *pGD) 
{
	//DISPLAY_CONTROL_REG = 0x2; //DISPLAY_CONTROL output path RGB=FIMD I80=FIMD ITU=FIMD
	*(volatile unsigned long *)(0xE0107008) = 0x2;
	
	//CLK_SRC1_REG = 0x700000;  //CLK_SRC1 fimdclk = EPLL
	*(volatile unsigned long *)(0xE0100204) = 0x700000;
} 
void board_video_reset()
{
	S5PC11X_FB * const fb = S5PC11X_GetBase_FB();
	fb->WINCON1 &= ~(S3C_WINCON_BPPMODE_16BPP_565 | S3C_WINCON_ENWIN_ENABLE |
	S3C_WINCON_HAWSWP_ENABLE);
} 
//forlinx.bs

int dram_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;

#if defined(PHYS_SDRAM_2)
	gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
	gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
#endif
	
	return 0;
}


#ifdef BOARD_LATE_INIT
#if defined(CONFIG_BOOT_NAND)
int board_late_init (void)
{
	uint *magic = (uint*)(PHYS_SDRAM_1);
	char boot_cmd[100];
	
	if ((0x24564236 == magic[0]) && (0x20764316 == magic[1])) {
		sprintf(boot_cmd, "nand erase 0 40000;nand write %08x 0 40000", PHYS_SDRAM_1 + 0x8000);
		magic[0] = 0;
		magic[1] = 0;
		printf("\nready for self-burning U-Boot image\n\n");
		setenv("bootdelay", "0");
		setenv("bootcmd", boot_cmd);
	}

	return 0;
}
#elif defined(CONFIG_BOOT_MOVINAND)
int board_late_init (void)
{
	uint *magic = (uint*)(PHYS_SDRAM_1);
	char boot_cmd[100];
	int hc;

	
	hc = (magic[2] & 0x1) ? 1 : 0;

	if ((0x24564236 == magic[0]) && (0x20764316 == magic[1])) {
		sprintf(boot_cmd, "movi init %d %d;movi write u-boot %08x", magic[3], hc, PHYS_SDRAM_1 + 0x8000);
		magic[0] = 0;
		magic[1] = 0;
		printf("\nready for self-burning U-Boot image\n\n");
		setenv("bootdelay", "0");
		setenv("bootcmd", boot_cmd);
	}

	return 0;
}
#else
int board_late_init (void)
{
	return 0;
}
#endif
#endif

#ifdef CONFIG_DISPLAY_BOARDINFO
int checkboard(void)
{
#ifdef CONFIG_MCP_SINGLE
#if defined(CONFIG_VOGUES)
	printf("\nBoard:   VOGUESV210\n");
#else
	printf("\nBoard:   SMDKV210\n");
#endif //CONFIG_VOGUES
#else
	printf("\nBoard:   SMDKC110\n");
#endif
	return (0);
}
#endif

#ifdef CONFIG_ENABLE_MMU

#ifdef CONFIG_MCP_SINGLE
ulong virt_to_phy_smdkc110(ulong addr)
{
	if ((0xc0000000 <= addr) && (addr < 0xd0000000))
		return (addr - 0xc0000000 + 0x20000000);
	else
		printf("The input address don't need "\
			"a virtual-to-physical translation : %08lx\n", addr);

	return addr;
}
#else
ulong virt_to_phy_smdkc110(ulong addr)
{
	if ((0xc0000000 <= addr) && (addr < 0xd0000000))
		return (addr - 0xc0000000 + 0x30000000);
	else if ((0x30000000 <= addr) && (addr < 0x50000000))
		return addr;
	else
		printf("The input address don't need "\
			"a virtual-to-physical translation : %08lx\n", addr);

	return addr;
}
#endif

#endif

#if defined(CONFIG_CMD_NAND) && defined(CFG_NAND_LEGACY)
#include <linux/mtd/nand.h>
extern struct nand_chip nand_dev_desc[CFG_MAX_NAND_DEVICE];
void nand_init(void)
{
	nand_probe(CFG_NAND_BASE);
        if (nand_dev_desc[0].ChipID != NAND_ChipID_UNKNOWN) {
                print_size(nand_dev_desc[0].totlen, "\n");
        }
}
#endif
