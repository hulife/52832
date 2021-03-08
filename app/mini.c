/* testmini.c -- very simple test program for the miniLZO library
   This file is part of the LZO real-time data compression library.
   Copyright (C) 1996-2017 Markus Franz Xaver Johannes Oberhumer
   All Rights Reserved.
   The LZO library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.
   The LZO library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with the LZO library; see the file COPYING.
   If not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
   Markus F.X.J. Oberhumer
   <markus@oberhumer.com>
   http://www.oberhumer.com/opensource/lzo/
 */
 /*miniLZO库使用非常简单，在压缩和解压缩之前先调用lzo_init函数进行初始化，如果该函数返回LZO_E_OK就表明没有问题可以继续操作。需要压缩数据调用lzo1x_1_compress函数，需要解压数据就调用lzo1x_decompress函数或lzo1x_decompress_safe函数。这两个函数的区别是lzo1x_decompress_safe函数会对要解压缩数据的有效性进行验证，如果验证通过才会进行解压缩操作而lzo1x_decompress函数则不会这么做，如果数据不是有效的则会产生“段错误”。建议使用lzo1x_decompress_safe函数。以下是这些函数的详细操作说明：

lzo1x_1_compress函数进行压缩数据操作，其需要5个参数分别是：要压缩的数据、要压缩的数据的大小（单位为字节）、压缩后数据的缓冲区、压缩后缓冲区的大小（值结果参数，调用成功之后存储实际的压缩后的数据大小）、压缩工作缓冲区。压缩数据成功之后会返回LZO_E_OK；
lzo1x_decompress和lzo1x_decompress_safe函数进行数据的解压缩操作，其也需要5个参数分别是：要解压缩的数据、要解压缩数据的大小（单位为字节）、解压缩后数据的存放缓冲区、原始数据（未压缩数据）大小（值结果参数，执行成功之后返回解压缩后数据的实际大小）、解压缩不需要工作缓冲区可以为NULL。执行成功返回LZO_E_OK且其第4个参数要和原始数据大小一致（这个参数以指针方式传入，传入的值不正确也会解压失败，实验过程中发现使用lzo1x_decompress函数这个值可以随便给，lzo1x_decompress_safe就不行）。
压缩过程需要一个工作内存wrkmem，在例子中经过了一系列宏可以发现他被定义为了一个64KB大小的数组，需要注意的一点是这个内存要4字节对齐，例子上特意加了这么一条注释来提醒。
注意：

（1）lzo1x_1_compress函数的第4个参数是值结果参数，传进去的值是用来指示存放压缩后数据的缓冲区大小，执行成功之后通过指针返回的结果是压缩后的数据实际使用的缓冲区大小，即压缩后的数据大小。压缩后需要的数据的缓冲区的大小上限是可以根据未压缩数据大小进行计算的公式：output_block_size = input_block_size + (input_block_size / 16) + 64 + 3；

（2）lzo1x_1_compress函数的第5个参数是压缩的时候需要使用的工作缓冲区，缓冲的生成在miniLZO库的提供测试例程（testmini.c）中有相关的宏生成该缓冲区，而解压的时候就不需要缓冲区；

（3）lzo1x_decompress和lzo1x_decompress_safe解压的时候需要的第4个参数是值结果参数，传进去的值是原始的未压缩数据的大小，执行成功之后通过指针返回的是实际解压缩后的数据的大小。所以压缩之后的数据在传输的时候需要将原始数据的大小和压缩后数据一起传输，否则对方在解压缩的时候将无法解压。
 */
#include <stdio.h>
#include <stdlib.h>
 
 
/*************************************************************************
// This program shows the basic usage of the LZO library.
// We will compress a block of data and decompress again.
//
// For more information, documentation, example programs and other support
// files (like Makefiles and build scripts) please download the full LZO
// package from
//    http://www.oberhumer.com/opensource/lzo/
**************************************************************************/
 
/* First let's include "minizo.h". */
 
#include "minilzo.h"
 
 
/* We want to compress the data block at 'in' with length 'IN_LEN' to
 * the block at 'out'. Because the input block may be incompressible,
 * we must provide a little more output space in case that compression
 * is not possible.
 */
 
#define IN_LEN      (128*1024ul)
#define OUT_LEN     (IN_LEN + IN_LEN / 16 + 64 + 3)
 
static unsigned char __LZO_MMODEL in  [ IN_LEN ];
static unsigned char __LZO_MMODEL out [ OUT_LEN ];
 
 
/* Work-memory needed for compression. Allocate memory in units
 * of 'lzo_align_t' (instead of 'char') to make sure it is properly aligned.
 */
 
#define HEAP_ALLOC(var,size) \
    lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]
 
static HEAP_ALLOC(wrkmem, LZO1X_1_MEM_COMPRESS);
 
 
/*************************************************************************
//
**************************************************************************/
 
int main_mini(int argc, char *argv[])
{
    int r;
    lzo_uint in_len;
    lzo_uint out_len;
    lzo_uint new_len;
 
    if (argc < 0 && argv == NULL)   /* avoid warning about unused args */
        return 0;
 
    printf("\nLZO real-time data compression library (v%s, %s).\n",
           lzo_version_string(), lzo_version_date());
    printf("Copyright (C) 1996-2017 Markus Franz Xaver Johannes Oberhumer\nAll Rights Reserved.\n\n");
 
 
/*
 * Step 1: initialize the LZO library
 */
    if (lzo_init() != LZO_E_OK)
    {
        printf("internal error - lzo_init() failed !!!\n");
        printf("(this usually indicates a compiler bug - try recompiling\nwithout optimizations, and enable '-DLZO_DEBUG' for diagnostics)\n");
        return 3;
    }
 
/*
 * Step 2: prepare the input block that will get compressed.
 *         We just fill it with zeros in this example program,
 *         but you would use your real-world data here.
 */
    in_len = IN_LEN;
    lzo_memset(in,0,in_len);
 
/*
 * Step 3: compress from 'in' to 'out' with LZO1X-1
 */
    r = lzo1x_1_compress(in,in_len,out,&out_len,wrkmem);
    if (r == LZO_E_OK)
        printf("compressed %lu bytes into %lu bytes\n",
            (unsigned long) in_len, (unsigned long) out_len);
    else
    {
        /* this should NEVER happen */
        printf("internal error - compression failed: %d\n", r);
        return 2;
    }
    /* check for an incompressible block */
    if (out_len >= in_len)
    {
        printf("This block contains incompressible data.\n");
        return 0;
    }
 
/*
 * Step 4: decompress again, now going from 'out' to 'in'
 */
    new_len = in_len;
    r = lzo1x_decompress(out,out_len,in,&new_len,NULL);
    if (r == LZO_E_OK && new_len == in_len)
        printf("decompressed %lu bytes back into %lu bytes\n",
            (unsigned long) out_len, (unsigned long) in_len);
    else
    {
        /* this should NEVER happen */
        printf("internal error - decompression failed: %d\n", r);
        return 1;
    }
 
    printf("\nminiLZO simple compression test passed.\n");
    return 0;
}
 
 
/* vim:set ts=4 sw=4 et: */

