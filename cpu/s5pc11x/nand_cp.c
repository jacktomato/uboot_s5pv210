/*
 * $Id: nand_cp.c,v 1.1 2008/11/20 01:08:36 boyko Exp $
 *
 * (C) Copyright 2006 Samsung Electronics
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

/*
 * You must make sure that all functions in this file are designed
 * to load only U-Boot image.
 *
 * So, DO NOT USE in common read.
 *
 * By scsuh.
 */

/*
 * Large block only: Small block is not supported.
 * No ECC operation: Bit error is not corrected.
 */

#include <common.h>

#ifdef CONFIG_S5PC11X
#include <asm/io.h>
#include <linux/mtd/nand.h>
#include <regs.h>

#define NAND_CONTROL_ENABLE()	(NFCONT_REG |= (1 << 0))

static void nand_readpage (ulong col, ulong row, uchar* buf, int len)
{
    int i;

    NAND_ENABLE_CE();

    NFCMD_REG = NAND_CMD_READ0;

    /* Write Column Address */
    NFADDR_REG = (col) & 0xff;
    NFADDR_REG = (col >> 8) & 0xff;

    /* Write Row Address */
    NFADDR_REG = (row) & 0xff;
    NFADDR_REG = (row >> 8) & 0xff;
    NFADDR_REG = (row >> 16) & 0xff;

    NFCMD_REG = NAND_CMD_READSTART;

    NF_TRANSRnB();

    for (i = 0; i < len; i++) {
        buf[i] = NFDATA8_REG;
    }

    NAND_DISABLE_CE();
}

static int nand_isbad (ulong addr)
{
    int i;
    int page_size = 2048;
    uchar oob[2];

    if (addr == 0)
        return 0;

    nand_readpage(page_size, addr, oob, 2);

    if ((oob[0] == 0xFF) && (oob[1] == 0xFF))
        return 0;
    else
        return 1;
}

/*forlinx add */
static int nand_isbad_forlinx (ulong addr,int large_block)
{
    int i;
    int page_size = 2048;
    uchar oob[2];

    if (addr == 0)
        return 0;

    if(large_block==1)
        page_size = 2048;
    else if(large_block==3)
        page_size = 2048;	
    else
        page_size = 4096;

    nand_readpage(page_size, addr, oob, 2);

    if ((oob[0] == 0xFF) && (oob[1] == 0xFF))
        return 0;
    else
        return 1;
}


/*
 * address format
 *              17 16         9 8            0
 * --------------------------------------------
 * | block(12bit) | page(5bit) | offset(9bit) |
 * --------------------------------------------
 */

static int nandll_read_page (uchar *buf, ulong addr)
{
    int i;
    int page_size = 2048;

    nand_readpage(0, addr, buf, page_size);

    return 0;
}

/* Forlinx add.*/
static int nandll_read_page_forlinx (uchar *buf, ulong addr,int large_block)
{
    int i;
    int page_size = 0;

    if(large_block == 1)
        page_size = 2048;
    else if(large_block == 3)
	    page_size = 2048;
    else
        page_size = 4096;


    nand_readpage(0, addr, buf, page_size);

    return 0;
}


/*
 * Read data from NAND.
 */
static int nandll_read_blocks (ulong dst_addr, ulong size)
{
    uchar *buf = (uchar *)dst_addr;
    int i;
    int skipped_page = 0;
    uint page_shift = 11;

    /* Read pages */
    for (i = 0; i < (size>>page_shift); i++, buf+=(1<<page_shift))
    {
        if ((i & (64 - 1)) == 0)
        {
            while (nand_isbad(i + skipped_page))
            {
                skipped_page += 64;
            }
        }
        nandll_read_page(buf, i + skipped_page);
    }

    return 0;
}

/*
 * Read data from NAND.
   Forlinx add .
 */
static int nandll_read_blocks_forlinx (ulong dst_addr, ulong size,int large_block)
{
    uchar *buf = (uchar *)dst_addr;
    int i;
    int skipped_page = 0;
    uint page_shift = 0;
    uint pages=0;

    if(large_block==1) //K9G2G08U0A
    {
        page_shift=11;  //2K/Page
        pages=64;       //64 Page/Block

        /* Read pages */
        for (i = 0; i < (size>>page_shift); i++, buf+=(1<<page_shift))
        {
            if ((i & (pages - 1)) == 0)
            {
                while (nand_isbad(i + skipped_page))
                {
                    skipped_page += pages;
                }
            }
            nandll_read_page(buf, i + skipped_page);
        }

    }
    else if(large_block==3)////Samsung K9K8G08U0E      size=1GB  type=SLC  Page=2K OOB=64bytes
    {
        page_shift=11;  //2K/Page
        pages=64;      //128Page/Block
	/* Read pages */
	for (i = 0; i < (size>>page_shift); i++, buf+=(1<<(page_shift)))
        {
            if ((i & (pages - 1)) == 0)
            {
                while (nand_isbad_forlinx(i + skipped_page,large_block))
                {
                    skipped_page += pages;
                }
            }
            nandll_read_page_forlinx(buf, i + skipped_page,large_block);
        }
    }
    else//MT29F8G08ABABAWP   other flash go this flow too,because 4K is normal Page.
    {
        page_shift=12;  //4K/Page
        pages=128;      //128Page/Block

        for (i = 0; i < (size>>page_shift); i++, buf+=(1<<(page_shift)))
        {

            if ((i & (pages - 1)) == 0)
            {
                while (nand_isbad_forlinx(i + skipped_page,large_block))
                {
                    skipped_page += pages;
                }
            }
            nandll_read_page_forlinx(buf, i + skipped_page,large_block);


        }

     }

    return 0;
}



int copy_uboot_to_ram (void)
{
    int large_block = 0;
    int i;
    vu_char id;

    NAND_CONTROL_ENABLE();
    NAND_ENABLE_CE();
    NFCMD_REG = NAND_CMD_READID;
    NFADDR_REG =  0x00;

    /* wait for a while */
    for (i=0; i<200; i++);

    int factory = NFDATA8_REG;
    id = NFDATA8_REG;

    large_block = 3;//default


    if (factory==0xec && id == 0xda) //K9F2G08U0B
        large_block = 1;

    if(factory==0x2c && id == 0x38) //MT29F8G08ABABAWP
    {
        large_block = 2;
    }
    if(factory==0xec && id == 0xd3)//Samsung K9K8G08U0E
    {
//Samsung K9K8G08U0E      size=1GB  type=SLC  Page=2K OOB=64bytes
	large_block = 1;
    }
    if(factory==0xec && id == 0xdc)//Samsung Samsung K9F4G08U0D scb 0
    {
//Samsung K9F4G08U0D      size=512Bytes  type=SLC  Page=2K OOB=64bytes
	large_block = 1;
    }
    if(large_block==1)
    {
      /* read NAND blocks */
      return nandll_read_blocks(CFG_PHY_UBOOT_BASE, COPY_BL2_SIZE);
    }
    else //MT29F8G08ABABAWP and other flash go.
    {
        return nandll_read_blocks_forlinx(CFG_PHY_UBOOT_BASE,COPY_BL2_SIZE,large_block);
    }



}


#if 0
#define REG_GPD0CON      (0xE02000A0)
#define REG_GPD0DAT       (0xE02000A4)

void beep(void)
{

    uint reg_f_cfg=readl(REG_GPD0CON) & 0xFFFFFFF0 | (0x01);
    writel(reg_f_cfg,REG_GPD0CON);

    uint reg_f_on=readl(REG_GPD0DAT) & 0xFFFFFFFE | (0x01);
    uint reg_f_off=readl(REG_GPD0DAT)& 0xFFFFFFFE;

    writel(reg_f_on,REG_GPD0DAT);
}
#else
void  beep(void)
{

}
#endif

#endif

