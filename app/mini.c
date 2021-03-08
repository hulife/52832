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
 /*miniLZO��ʹ�÷ǳ��򵥣���ѹ���ͽ�ѹ��֮ǰ�ȵ���lzo_init�������г�ʼ��������ú�������LZO_E_OK�ͱ���û��������Լ�����������Ҫѹ�����ݵ���lzo1x_1_compress��������Ҫ��ѹ���ݾ͵���lzo1x_decompress������lzo1x_decompress_safe������������������������lzo1x_decompress_safe�������Ҫ��ѹ�����ݵ���Ч�Խ�����֤�������֤ͨ���Ż���н�ѹ��������lzo1x_decompress�����򲻻���ô����������ݲ�����Ч�����������δ��󡱡�����ʹ��lzo1x_decompress_safe��������������Щ��������ϸ����˵����

lzo1x_1_compress��������ѹ�����ݲ���������Ҫ5�������ֱ��ǣ�Ҫѹ�������ݡ�Ҫѹ�������ݵĴ�С����λΪ�ֽڣ���ѹ�������ݵĻ�������ѹ���󻺳����Ĵ�С��ֵ������������óɹ�֮��洢ʵ�ʵ�ѹ��������ݴ�С����ѹ��������������ѹ�����ݳɹ�֮��᷵��LZO_E_OK��
lzo1x_decompress��lzo1x_decompress_safe�����������ݵĽ�ѹ����������Ҳ��Ҫ5�������ֱ��ǣ�Ҫ��ѹ�������ݡ�Ҫ��ѹ�����ݵĴ�С����λΪ�ֽڣ�����ѹ�������ݵĴ�Ż�������ԭʼ���ݣ�δѹ�����ݣ���С��ֵ���������ִ�гɹ�֮�󷵻ؽ�ѹ�������ݵ�ʵ�ʴ�С������ѹ������Ҫ��������������ΪNULL��ִ�гɹ�����LZO_E_OK�����4������Ҫ��ԭʼ���ݴ�Сһ�£����������ָ�뷽ʽ���룬�����ֵ����ȷҲ���ѹʧ�ܣ�ʵ������з���ʹ��lzo1x_decompress�������ֵ����������lzo1x_decompress_safe�Ͳ��У���
ѹ��������Ҫһ�������ڴ�wrkmem���������о�����һϵ�к���Է�����������Ϊ��һ��64KB��С�����飬��Ҫע���һ��������ڴ�Ҫ4�ֽڶ��룬���������������ôһ��ע�������ѡ�
ע�⣺

��1��lzo1x_1_compress�����ĵ�4��������ֵ�������������ȥ��ֵ������ָʾ���ѹ�������ݵĻ�������С��ִ�гɹ�֮��ͨ��ָ�뷵�صĽ����ѹ���������ʵ��ʹ�õĻ�������С����ѹ��������ݴ�С��ѹ������Ҫ�����ݵĻ������Ĵ�С�����ǿ��Ը���δѹ�����ݴ�С���м���Ĺ�ʽ��output_block_size = input_block_size + (input_block_size / 16) + 64 + 3��

��2��lzo1x_1_compress�����ĵ�5��������ѹ����ʱ����Ҫʹ�õĹ����������������������miniLZO����ṩ�������̣�testmini.c��������صĺ����ɸû�����������ѹ��ʱ��Ͳ���Ҫ��������

��3��lzo1x_decompress��lzo1x_decompress_safe��ѹ��ʱ����Ҫ�ĵ�4��������ֵ�������������ȥ��ֵ��ԭʼ��δѹ�����ݵĴ�С��ִ�гɹ�֮��ͨ��ָ�뷵�ص���ʵ�ʽ�ѹ��������ݵĴ�С������ѹ��֮��������ڴ����ʱ����Ҫ��ԭʼ���ݵĴ�С��ѹ��������һ���䣬����Է��ڽ�ѹ����ʱ���޷���ѹ��
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

