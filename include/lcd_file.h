/*
 * include/lcd_file.h
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

#ifndef _LCD_FILE_H_
#define _LCD_FILE_H_

#include <linux/mtd/compat.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <nand.h>
/******************************************************************/
//int read_file_from_nand(ulong v_addr,ulong v_size,ulong v_off);
/******************************************************************/
int read_bmp();
#endif /* _LCD_FILE_H_ */
