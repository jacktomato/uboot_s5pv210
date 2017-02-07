/*
 * Common LCD routines for supported CPUs
 *
 * (C) Copyright 2001-2002
 * Wolfgang Denk, forlinx Software Engineering -- zhangdongguang@forlinx.com
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * HeBei Baoding China
 */

#include <common.h>
#include <s3c6410.h>
#include <common.h>
//#include <malloc.h>//bluesky add
#include <nand.h>//bluesky add
#include <lcd_file.h>//bluesky

extern nand_info_t nand_info[];       /* info for NAND chips */

int read_file_from_nand(ulong v_addr,ulong v_size,ulong v_off)
{
		//nand read
		//
		int i, dev, ret;
		ulong addr, off, size;
		char *cmd, *s;
		nand_info_t *nand;
		int quiet = 1;
		ret=0;
		//
		addr=v_addr;////
		off=v_off;//////
		size=v_size;////
		//nand read
		/* the following commands operate on the current device */
		if (nand_curr_device < 0 || nand_curr_device >= CFG_MAX_NAND_DEVICE ||
				!nand_info[nand_curr_device].name) {
			puts("\nno devices available2\n");
			ret=-1;
			return ret;
		}
		nand = &nand_info[nand_curr_device];
		/* read */
        nand_read_options_t opts;
        memset(&opts, 0, sizeof(opts));
        opts.buffer	= (u_char*) addr;
        opts.length	= size;
        opts.offset	= off;
        opts.quiet      = quiet;
        ret = nand_read_opts(nand, &opts);
        //printf("%s %s :%d\n",__func__,__FILE__,__LINE__);
		
		return ret;
}
int read_bmp()
{
	int i=0;
	int ret=0;
	ulong mem_addr=0xc0f00000;
	ulong bmp_size=0x100000;
	ulong nand_part_off=0x600000;
	ret=read_file_from_nand(mem_addr,bmp_size,nand_part_off);
	u_char* paddr=(u_char*)mem_addr;
	//printf("the bmp file:\n");
	//for(i=0;i<54;i++)
	//	printf("%x ",(*(paddr+i)));
	//printf("%s %s %d\n",__FILE__,__func__,__LINE__);
	return ret;
}
