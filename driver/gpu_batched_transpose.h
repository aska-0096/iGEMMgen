/*******************************************************************************
 *
 * MIT License
 *
 * Copyright (c) 2020-2021 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 *all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *******************************************************************************/
#ifndef __GPU_BATCHED_TRANSPOSE_H
#define __GPU_BATCHED_TRANSPOSE_H

#include <hip/hip_ext.h>
#include <hip/hip_runtime.h>
#include <assert.h>
#include "magic_div.h"

#ifndef BATCHED_TRANSPOSE_PERSISTENT
#define BATCHED_TRANSPOSE_PERSISTENT 0
#endif

#ifndef HIP_CALL
#define HIP_CALL(call)                                                         \
    do {                                                                       \
        hipError_t err = call;                                                 \
        if (err != hipSuccess) {                                               \
            printf("[hiperror](%d) fail to call %s,(%s)\n", (int)err, #call,     \
                   hipGetErrorString(err));                                    \
            exit(1);                                                           \
        }                                                                      \
    } while (0)
#endif

static struct {
    hipModule_t     module;

    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2ncwh;
    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2nhcw;
    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2nhwc;
    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2nwch;
    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2nwhc;
    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2cnhw;
    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2cnwh;
    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2chnw;
    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2chwn;
    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2cwnh;
    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2cwhn;
    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2hncw;
    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2hnwc;
    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2hcnw;
    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2hcwn;
    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2hwnc;
    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2hwcn;
    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2wnch;
    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2wnhc;
    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2wcnh;
    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2wchn;
    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2whnc;
    hipFunction_t   kernel_general_4d_reorder_1p_dword_nchw2whcn;

    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2ncwh;
    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2nhcw;
    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2nhwc;
    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2nwch;
    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2nwhc;
    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2cnhw;
    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2cnwh;
    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2chnw;
    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2chwn;
    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2cwnh;
    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2cwhn;
    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2hncw;
    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2hnwc;
    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2hcnw;
    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2hcwn;
    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2hwnc;
    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2hwcn;
    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2wnch;
    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2wnhc;
    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2wcnh;
    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2wchn;
    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2whnc;
    hipFunction_t   kernel_general_4d_reorder_2p_dword_nchw2whcn;

    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2ncwh;
    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2nhcw;
    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2nhwc;
    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2nwch;
    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2nwhc;
    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2cnhw;
    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2cnwh;
    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2chnw;
    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2chwn;
    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2cwnh;
    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2cwhn;
    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2hncw;
    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2hnwc;
    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2hcnw;
    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2hcwn;
    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2hwnc;
    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2hwcn;
    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2wnch;
    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2wnhc;
    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2wcnh;
    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2wchn;
    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2whnc;
    hipFunction_t   kernel_general_4d_reorder_4p_dword_nchw2whcn;

    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2ncwh;
    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2nhcw;
    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2nhwc;
    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2nwch;
    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2nwhc;
    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2cnhw;
    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2cnwh;
    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2chnw;
    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2chwn;
    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2cwnh;
    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2cwhn;
    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2hncw;
    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2hnwc;
    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2hcnw;
    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2hcwn;
    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2hwnc;
    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2hwcn;
    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2wnch;
    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2wnhc;
    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2wcnh;
    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2wchn;
    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2whnc;
    hipFunction_t   kernel_general_4d_reorder_8p_dword_nchw2whcn;

    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2ncwh;
    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2nhcw;
    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2nhwc;
    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2nwch;
    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2nwhc;
    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2cnhw;
    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2cnwh;
    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2chnw;
    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2chwn;
    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2cwnh;
    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2cwhn;
    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2hncw;
    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2hnwc;
    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2hcnw;
    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2hcwn;
    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2hwnc;
    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2hwcn;
    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2wnch;
    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2wnhc;
    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2wcnh;
    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2wchn;
    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2whnc;
    hipFunction_t   kernel_general_4d_reorder_16p_dword_nchw2whcn;

} the_reorder_gpu_handle;

static struct {
    hipModule_t     module;

    hipFunction_t   kernel_batched_transpose_16x16_dword;
    hipFunction_t   kernel_batched_transpose_16x16_half;
    hipFunction_t   kernel_batched_transpose_16x16_byte;

    hipFunction_t   kernel_batched_transpose_32x16_dword;
    hipFunction_t   kernel_batched_transpose_32x16_half;
    hipFunction_t   kernel_batched_transpose_32x16_byte;

    hipFunction_t   kernel_batched_transpose_16x32_dword;
    hipFunction_t   kernel_batched_transpose_16x32_half;
    hipFunction_t   kernel_batched_transpose_16x32_byte;

    hipFunction_t   kernel_batched_transpose_32x32_dword;
    hipFunction_t   kernel_batched_transpose_32x32_half;
    hipFunction_t   kernel_batched_transpose_32x32_byte;

    hipFunction_t   kernel_batched_transpose_4x64_dword;
    hipFunction_t   kernel_batched_transpose_4x64_half;
    hipFunction_t   kernel_batched_transpose_4x64_byte;

    hipFunction_t   kernel_batched_transpose_64x4_dword;
    hipFunction_t   kernel_batched_transpose_64x4_half;
    hipFunction_t   kernel_batched_transpose_64x4_byte;

    hipFunction_t   kernel_batched_transpose_4x128_dword;
    hipFunction_t   kernel_batched_transpose_4x128_half;
    hipFunction_t   kernel_batched_transpose_4x128_byte;

    hipFunction_t   kernel_batched_transpose_128x4_dword;
    hipFunction_t   kernel_batched_transpose_128x4_half;
    hipFunction_t   kernel_batched_transpose_128x4_byte;

    hipFunction_t   kernel_batched_transpose_4x256_dword;
    hipFunction_t   kernel_batched_transpose_4x256_half;
    hipFunction_t   kernel_batched_transpose_4x256_byte;

    hipFunction_t   kernel_batched_transpose_256x4_dword;
    hipFunction_t   kernel_batched_transpose_256x4_half;
    hipFunction_t   kernel_batched_transpose_256x4_byte;

    hipFunction_t   kernel_batched_transpose_32x32_pack_2x2_ediv_2x2_half;
    hipFunction_t   kernel_batched_transpose_32x32_pack_2x2_ediv_1x2_half;
    hipFunction_t   kernel_batched_transpose_32x32_pack_2x2_ediv_2x1_half;
    hipFunction_t   kernel_batched_transpose_32x32_pack_2x2_ediv_1x1_half;

    hipFunction_t   kernel_batched_transpose_64x32_pack_4x2_ediv_4x2_half;
    hipFunction_t   kernel_batched_transpose_64x32_pack_4x2_ediv_2x2_half;
    hipFunction_t   kernel_batched_transpose_64x32_pack_4x2_ediv_2x1_half;

    hipFunction_t   kernel_batched_transpose_32x64_pack_2x4_ediv_2x4_half;
    hipFunction_t   kernel_batched_transpose_32x64_pack_2x4_ediv_2x2_half;
    hipFunction_t   kernel_batched_transpose_32x64_pack_2x4_ediv_1x2_half;

    hipFunction_t   kernel_batched_transpose_16x64_pack_1x4_ediv_1x2_half;
    hipFunction_t   kernel_batched_transpose_64x16_pack_4x1_ediv_2x1_half;

    hipFunction_t   kernel_batched_transpose_64x64_pack_4x4_ediv_4x4_half;
    hipFunction_t   kernel_batched_transpose_64x64_pack_4x4_ediv_2x2_half;
} the_transpose_gpu_handle;

static inline void gpu_nhwc_nchw_transpose_init(const char * hsaco){
    static int inited = 0;
    if(!inited){
        HIP_CALL(hipModuleLoad(&the_transpose_gpu_handle.module, hsaco));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_16x16_dword,  the_transpose_gpu_handle.module, "batched_transpose_16x16_dword"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_16x16_half,   the_transpose_gpu_handle.module, "batched_transpose_16x16_half"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_16x16_byte,   the_transpose_gpu_handle.module, "batched_transpose_16x16_byte"));

        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_32x16_dword,  the_transpose_gpu_handle.module, "batched_transpose_32x16_dword"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_32x16_half,   the_transpose_gpu_handle.module, "batched_transpose_32x16_half"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_32x16_byte,   the_transpose_gpu_handle.module, "batched_transpose_32x16_byte"));

        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_16x32_dword,  the_transpose_gpu_handle.module, "batched_transpose_16x32_dword"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_16x32_half,   the_transpose_gpu_handle.module, "batched_transpose_16x32_half"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_16x32_byte,   the_transpose_gpu_handle.module, "batched_transpose_16x32_byte"));

        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_32x32_dword,  the_transpose_gpu_handle.module, "batched_transpose_32x32_dword"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_32x32_half,   the_transpose_gpu_handle.module, "batched_transpose_32x32_half"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_32x32_byte,   the_transpose_gpu_handle.module, "batched_transpose_32x32_byte"));

        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_4x64_dword,  the_transpose_gpu_handle.module, "batched_transpose_4x64_dword"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_4x64_half,   the_transpose_gpu_handle.module, "batched_transpose_4x64_half"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_4x64_byte,   the_transpose_gpu_handle.module, "batched_transpose_4x64_byte"));

        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_64x4_dword,  the_transpose_gpu_handle.module, "batched_transpose_64x4_dword"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_64x4_half,   the_transpose_gpu_handle.module, "batched_transpose_64x4_half"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_64x4_byte,   the_transpose_gpu_handle.module, "batched_transpose_64x4_byte"));

        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_4x128_dword,  the_transpose_gpu_handle.module, "batched_transpose_4x128_dword"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_4x128_half,   the_transpose_gpu_handle.module, "batched_transpose_4x128_half"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_4x128_byte,   the_transpose_gpu_handle.module, "batched_transpose_4x128_byte"));

        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_128x4_dword,  the_transpose_gpu_handle.module, "batched_transpose_128x4_dword"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_128x4_half,   the_transpose_gpu_handle.module, "batched_transpose_128x4_half"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_128x4_byte,   the_transpose_gpu_handle.module, "batched_transpose_128x4_byte"));

        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_4x256_dword,  the_transpose_gpu_handle.module, "batched_transpose_4x256_dword"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_4x256_half,   the_transpose_gpu_handle.module, "batched_transpose_4x256_half"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_4x256_byte,   the_transpose_gpu_handle.module, "batched_transpose_4x256_byte"));

        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_256x4_dword,  the_transpose_gpu_handle.module, "batched_transpose_256x4_dword"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_256x4_half,   the_transpose_gpu_handle.module, "batched_transpose_256x4_half"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_256x4_byte,   the_transpose_gpu_handle.module, "batched_transpose_256x4_byte"));

        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_32x32_pack_2x2_ediv_2x2_half,   the_transpose_gpu_handle.module, "batched_transpose_32x32_pack_2x2_ediv_2x2_half"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_32x32_pack_2x2_ediv_1x2_half,   the_transpose_gpu_handle.module, "batched_transpose_32x32_pack_2x2_ediv_1x2_half"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_32x32_pack_2x2_ediv_2x1_half,   the_transpose_gpu_handle.module, "batched_transpose_32x32_pack_2x2_ediv_2x1_half"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_32x32_pack_2x2_ediv_1x1_half,   the_transpose_gpu_handle.module, "batched_transpose_32x32_pack_2x2_ediv_1x1_half"));

        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_64x32_pack_4x2_ediv_4x2_half,   the_transpose_gpu_handle.module, "batched_transpose_64x32_pack_4x2_ediv_4x2_half"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_64x32_pack_4x2_ediv_2x2_half,   the_transpose_gpu_handle.module, "batched_transpose_64x32_pack_4x2_ediv_2x2_half"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_64x32_pack_4x2_ediv_2x1_half,   the_transpose_gpu_handle.module, "batched_transpose_64x32_pack_4x2_ediv_2x1_half"));

        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_32x64_pack_2x4_ediv_2x4_half,   the_transpose_gpu_handle.module, "batched_transpose_32x64_pack_2x4_ediv_2x4_half"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_32x64_pack_2x4_ediv_2x2_half,   the_transpose_gpu_handle.module, "batched_transpose_32x64_pack_2x4_ediv_2x2_half"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_32x64_pack_2x4_ediv_1x2_half,   the_transpose_gpu_handle.module, "batched_transpose_32x64_pack_2x4_ediv_1x2_half"));

        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_16x64_pack_1x4_ediv_1x2_half,   the_transpose_gpu_handle.module, "batched_transpose_16x64_pack_1x4_ediv_1x2_half"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_64x16_pack_4x1_ediv_2x1_half,   the_transpose_gpu_handle.module, "batched_transpose_64x16_pack_4x1_ediv_2x1_half"));

        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_64x64_pack_4x4_ediv_4x4_half,   the_transpose_gpu_handle.module, "batched_transpose_64x64_pack_4x4_ediv_4x4_half"));
        HIP_CALL(hipModuleGetFunction(&the_transpose_gpu_handle.kernel_batched_transpose_64x64_pack_4x4_ediv_2x2_half,   the_transpose_gpu_handle.module, "batched_transpose_64x64_pack_4x4_ediv_2x2_half"));
        inited = 1;
    }
}
//reorder kernel
static inline void gpu_tensor_reorder_init(const char * hsaco){
    static int inited = 0;
    if(!inited){
        HIP_CALL(hipModuleLoad(&the_reorder_gpu_handle.module, hsaco));

        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2ncwh, the_reorder_gpu_handle.module,  "general_4d_reorder_1p_dword_nchw2ncwh"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2nhcw, the_reorder_gpu_handle.module,  "general_4d_reorder_1p_dword_nchw2nhcw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2nhwc,  the_reorder_gpu_handle.module, "general_4d_reorder_1p_dword_nchw2nhwc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2nwch,  the_reorder_gpu_handle.module, "general_4d_reorder_1p_dword_nchw2nwch"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2nwhc, the_reorder_gpu_handle.module,  "general_4d_reorder_1p_dword_nchw2nwhc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2cnhw, the_reorder_gpu_handle.module,  "general_4d_reorder_1p_dword_nchw2cnhw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2cnwh,  the_reorder_gpu_handle.module, "general_4d_reorder_1p_dword_nchw2cnwh"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2chnw,  the_reorder_gpu_handle.module, "general_4d_reorder_1p_dword_nchw2chnw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2chwn, the_reorder_gpu_handle.module,  "general_4d_reorder_1p_dword_nchw2chwn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2cwnh, the_reorder_gpu_handle.module,  "general_4d_reorder_1p_dword_nchw2cwnh"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2cwhn,  the_reorder_gpu_handle.module, "general_4d_reorder_1p_dword_nchw2cwhn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2hncw,  the_reorder_gpu_handle.module, "general_4d_reorder_1p_dword_nchw2hncw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2hnwc, the_reorder_gpu_handle.module,  "general_4d_reorder_1p_dword_nchw2hnwc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2hcnw, the_reorder_gpu_handle.module,  "general_4d_reorder_1p_dword_nchw2hcnw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2hcwn,  the_reorder_gpu_handle.module, "general_4d_reorder_1p_dword_nchw2hcwn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2hwnc,  the_reorder_gpu_handle.module, "general_4d_reorder_1p_dword_nchw2hwnc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2hwcn, the_reorder_gpu_handle.module,  "general_4d_reorder_1p_dword_nchw2hwcn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2wnch, the_reorder_gpu_handle.module,  "general_4d_reorder_1p_dword_nchw2wnch"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2wnhc,  the_reorder_gpu_handle.module, "general_4d_reorder_1p_dword_nchw2wnhc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2wcnh,  the_reorder_gpu_handle.module, "general_4d_reorder_1p_dword_nchw2wcnh"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2wchn, the_reorder_gpu_handle.module,  "general_4d_reorder_1p_dword_nchw2wchn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2whnc, the_reorder_gpu_handle.module,  "general_4d_reorder_1p_dword_nchw2whnc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2whcn,  the_reorder_gpu_handle.module, "general_4d_reorder_1p_dword_nchw2whcn"));

        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2ncwh, the_reorder_gpu_handle.module,  "general_4d_reorder_2p_dword_nchw2ncwh"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2nhcw, the_reorder_gpu_handle.module,  "general_4d_reorder_2p_dword_nchw2nhcw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2nhwc,  the_reorder_gpu_handle.module, "general_4d_reorder_2p_dword_nchw2nhwc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2nwch,  the_reorder_gpu_handle.module, "general_4d_reorder_2p_dword_nchw2nwch"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2nwhc, the_reorder_gpu_handle.module,  "general_4d_reorder_2p_dword_nchw2nwhc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2cnhw, the_reorder_gpu_handle.module,  "general_4d_reorder_2p_dword_nchw2cnhw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2cnwh,  the_reorder_gpu_handle.module, "general_4d_reorder_2p_dword_nchw2cnwh"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2chnw,  the_reorder_gpu_handle.module, "general_4d_reorder_2p_dword_nchw2chnw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2chwn, the_reorder_gpu_handle.module,  "general_4d_reorder_2p_dword_nchw2chwn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2cwnh, the_reorder_gpu_handle.module,  "general_4d_reorder_2p_dword_nchw2cwnh"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2cwhn,  the_reorder_gpu_handle.module, "general_4d_reorder_2p_dword_nchw2cwhn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2hncw,  the_reorder_gpu_handle.module, "general_4d_reorder_2p_dword_nchw2hncw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2hnwc, the_reorder_gpu_handle.module,  "general_4d_reorder_2p_dword_nchw2hnwc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2hcnw, the_reorder_gpu_handle.module,  "general_4d_reorder_2p_dword_nchw2hcnw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2hcwn,  the_reorder_gpu_handle.module, "general_4d_reorder_2p_dword_nchw2hcwn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2hwnc,  the_reorder_gpu_handle.module, "general_4d_reorder_2p_dword_nchw2hwnc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2hwcn, the_reorder_gpu_handle.module,  "general_4d_reorder_2p_dword_nchw2hwcn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2wnch, the_reorder_gpu_handle.module,  "general_4d_reorder_2p_dword_nchw2wnch"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2wnhc,  the_reorder_gpu_handle.module, "general_4d_reorder_2p_dword_nchw2wnhc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2wcnh,  the_reorder_gpu_handle.module, "general_4d_reorder_2p_dword_nchw2wcnh"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2wchn, the_reorder_gpu_handle.module,  "general_4d_reorder_2p_dword_nchw2wchn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2whnc, the_reorder_gpu_handle.module,  "general_4d_reorder_2p_dword_nchw2whnc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_2p_dword_nchw2whcn,  the_reorder_gpu_handle.module, "general_4d_reorder_2p_dword_nchw2whcn"));

        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2ncwh, the_reorder_gpu_handle.module,  "general_4d_reorder_4p_dword_nchw2ncwh"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2nhcw, the_reorder_gpu_handle.module,  "general_4d_reorder_4p_dword_nchw2nhcw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2nhwc,  the_reorder_gpu_handle.module, "general_4d_reorder_4p_dword_nchw2nhwc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2nwch,  the_reorder_gpu_handle.module, "general_4d_reorder_4p_dword_nchw2nwch"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2nwhc, the_reorder_gpu_handle.module,  "general_4d_reorder_4p_dword_nchw2nwhc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2cnhw, the_reorder_gpu_handle.module,  "general_4d_reorder_4p_dword_nchw2cnhw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2cnwh,  the_reorder_gpu_handle.module, "general_4d_reorder_4p_dword_nchw2cnwh"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2chnw,  the_reorder_gpu_handle.module, "general_4d_reorder_4p_dword_nchw2chnw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2chwn, the_reorder_gpu_handle.module,  "general_4d_reorder_4p_dword_nchw2chwn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2cwnh, the_reorder_gpu_handle.module,  "general_4d_reorder_4p_dword_nchw2cwnh"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2cwhn,  the_reorder_gpu_handle.module, "general_4d_reorder_4p_dword_nchw2cwhn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2hncw,  the_reorder_gpu_handle.module, "general_4d_reorder_4p_dword_nchw2hncw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2hnwc, the_reorder_gpu_handle.module,  "general_4d_reorder_4p_dword_nchw2hnwc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2hcnw, the_reorder_gpu_handle.module,  "general_4d_reorder_4p_dword_nchw2hcnw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2hcwn,  the_reorder_gpu_handle.module, "general_4d_reorder_4p_dword_nchw2hcwn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2hwnc,  the_reorder_gpu_handle.module, "general_4d_reorder_4p_dword_nchw2hwnc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2hwcn, the_reorder_gpu_handle.module,  "general_4d_reorder_4p_dword_nchw2hwcn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2wnch, the_reorder_gpu_handle.module,  "general_4d_reorder_4p_dword_nchw2wnch"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2wnhc,  the_reorder_gpu_handle.module, "general_4d_reorder_4p_dword_nchw2wnhc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2wcnh,  the_reorder_gpu_handle.module, "general_4d_reorder_4p_dword_nchw2wcnh"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2wchn, the_reorder_gpu_handle.module,  "general_4d_reorder_4p_dword_nchw2wchn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2whnc, the_reorder_gpu_handle.module,  "general_4d_reorder_4p_dword_nchw2whnc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2whcn,  the_reorder_gpu_handle.module, "general_4d_reorder_4p_dword_nchw2whcn"));
        
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2ncwh, the_reorder_gpu_handle.module,  "general_4d_reorder_8p_dword_nchw2ncwh"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2nhcw, the_reorder_gpu_handle.module,  "general_4d_reorder_8p_dword_nchw2nhcw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2nhwc,  the_reorder_gpu_handle.module, "general_4d_reorder_8p_dword_nchw2nhwc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2nwch,  the_reorder_gpu_handle.module, "general_4d_reorder_8p_dword_nchw2nwch"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2nwhc, the_reorder_gpu_handle.module,  "general_4d_reorder_8p_dword_nchw2nwhc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2cnhw, the_reorder_gpu_handle.module,  "general_4d_reorder_8p_dword_nchw2cnhw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2cnwh,  the_reorder_gpu_handle.module, "general_4d_reorder_8p_dword_nchw2cnwh"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2chnw,  the_reorder_gpu_handle.module, "general_4d_reorder_8p_dword_nchw2chnw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2chwn, the_reorder_gpu_handle.module,  "general_4d_reorder_8p_dword_nchw2chwn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2cwnh, the_reorder_gpu_handle.module,  "general_4d_reorder_8p_dword_nchw2cwnh"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2cwhn,  the_reorder_gpu_handle.module, "general_4d_reorder_8p_dword_nchw2cwhn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2hncw,  the_reorder_gpu_handle.module, "general_4d_reorder_8p_dword_nchw2hncw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2hnwc, the_reorder_gpu_handle.module,  "general_4d_reorder_8p_dword_nchw2hnwc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2hcnw, the_reorder_gpu_handle.module,  "general_4d_reorder_8p_dword_nchw2hcnw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2hcwn,  the_reorder_gpu_handle.module, "general_4d_reorder_8p_dword_nchw2hcwn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2hwnc,  the_reorder_gpu_handle.module, "general_4d_reorder_8p_dword_nchw2hwnc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2hwcn, the_reorder_gpu_handle.module,  "general_4d_reorder_8p_dword_nchw2hwcn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2wnch, the_reorder_gpu_handle.module,  "general_4d_reorder_8p_dword_nchw2wnch"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2wnhc,  the_reorder_gpu_handle.module, "general_4d_reorder_8p_dword_nchw2wnhc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2wcnh,  the_reorder_gpu_handle.module, "general_4d_reorder_8p_dword_nchw2wcnh"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2wchn, the_reorder_gpu_handle.module,  "general_4d_reorder_8p_dword_nchw2wchn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2whnc, the_reorder_gpu_handle.module,  "general_4d_reorder_8p_dword_nchw2whnc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2whcn,  the_reorder_gpu_handle.module, "general_4d_reorder_8p_dword_nchw2whcn"));

        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2ncwh, the_reorder_gpu_handle.module,  "general_4d_reorder_16p_dword_nchw2ncwh"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2nhcw, the_reorder_gpu_handle.module,  "general_4d_reorder_16p_dword_nchw2nhcw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2nhwc,  the_reorder_gpu_handle.module, "general_4d_reorder_16p_dword_nchw2nhwc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2nwch,  the_reorder_gpu_handle.module, "general_4d_reorder_16p_dword_nchw2nwch"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2nwhc, the_reorder_gpu_handle.module,  "general_4d_reorder_16p_dword_nchw2nwhc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2cnhw, the_reorder_gpu_handle.module,  "general_4d_reorder_16p_dword_nchw2cnhw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2cnwh,  the_reorder_gpu_handle.module, "general_4d_reorder_16p_dword_nchw2cnwh"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2chnw,  the_reorder_gpu_handle.module, "general_4d_reorder_16p_dword_nchw2chnw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2chwn, the_reorder_gpu_handle.module,  "general_4d_reorder_16p_dword_nchw2chwn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2cwnh, the_reorder_gpu_handle.module,  "general_4d_reorder_16p_dword_nchw2cwnh"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2cwhn,  the_reorder_gpu_handle.module, "general_4d_reorder_16p_dword_nchw2cwhn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2hncw,  the_reorder_gpu_handle.module, "general_4d_reorder_16p_dword_nchw2hncw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2hnwc, the_reorder_gpu_handle.module,  "general_4d_reorder_16p_dword_nchw2hnwc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2hcnw, the_reorder_gpu_handle.module,  "general_4d_reorder_16p_dword_nchw2hcnw"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2hcwn,  the_reorder_gpu_handle.module, "general_4d_reorder_16p_dword_nchw2hcwn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2hwnc,  the_reorder_gpu_handle.module, "general_4d_reorder_16p_dword_nchw2hwnc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2hwcn, the_reorder_gpu_handle.module,  "general_4d_reorder_16p_dword_nchw2hwcn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2wnch, the_reorder_gpu_handle.module,  "general_4d_reorder_16p_dword_nchw2wnch"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2wnhc,  the_reorder_gpu_handle.module, "general_4d_reorder_16p_dword_nchw2wnhc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2wcnh,  the_reorder_gpu_handle.module, "general_4d_reorder_16p_dword_nchw2wcnh"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2wchn, the_reorder_gpu_handle.module,  "general_4d_reorder_16p_dword_nchw2wchn"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2whnc, the_reorder_gpu_handle.module,  "general_4d_reorder_16p_dword_nchw2whnc"));
        HIP_CALL(hipModuleGetFunction(&the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2whcn,  the_reorder_gpu_handle.module, "general_4d_reorder_16p_dword_nchw2whcn"));
        
        inited = 1;
    }
}

typedef struct {
    void * p_dst;
    void * p_src;
    uint32_t height;
    uint32_t width;
    uint32_t dim_stride;
    uint32_t dim_total;
    uint32_t magic_h;
    uint32_t shift_h;
    uint32_t magic_w;
    uint32_t shift_w;
} __attribute__((packed)) transpose_kernel_t;

typedef struct {
    void * p_dst;
    void * p_src;
    uint32_t dim_0;
    uint32_t dim_1;
    uint32_t dim_2;
    uint32_t dim_3;
    uint32_t dim_stride;
    uint32_t dim_total;
    uint32_t magic_stride0;
    uint32_t shift_stride0;
    uint32_t magic_stride1;
    uint32_t shift_stride1;
    uint32_t magic_stride2;
    uint32_t shift_stride2;
} __attribute__((packed)) reorder_kernel_t;

static inline void dump_transpose_kernel_arg(transpose_kernel_t * karg)
{
    printf("dst:%p, src:%p, h:%u, w:%u, dim_stride:%u, dim_total:%u, mh:%u, sh:%u, mw:%u, sw:%u\n",
        karg->p_dst,
        karg->p_src,
        karg->height,
        karg->width,
        karg->dim_stride,
        karg->dim_total,
        karg->magic_h,
        karg->shift_h,
        karg->magic_w,
        karg->shift_w);
    fflush(stdout);
}

typedef struct{
    int tile_x;
    int tile_y;
    int pack_x;
    int pack_y;
    int ediv_x;
    int ediv_y;
}transpose_kernel_param_t;

template<size_t type_size>
struct tensor_reorder_kernel_get_all_param_t{
    static std::vector<transpose_kernel_param_t> get(){
        std::vector<transpose_kernel_param_t> the_list {
            {1, 1, 1, 1, 1, 1},
            {2, 1, 1, 1, 1, 1},
            {4, 1, 1, 1, 1, 1},
            {8, 1, 1, 1, 1, 1},
            {16, 1, 1, 1, 1, 1},
            //{16, 32, 1, 1, 1, 1},
            //{32, 32, 1, 1, 1, 1},

            //{4,  64, 1, 1, 1, 1},
            //{ 64, 4, 1, 1, 1, 1},
            //{4, 128, 1, 1, 1, 1},
            //{128, 4, 1, 1, 1, 1},
            //{4, 256, 1, 1, 1, 1},
            //{256, 4, 1, 1, 1, 1},
        };
        return the_list;
    }
};

template<>
struct tensor_reorder_kernel_get_all_param_t<4>{
    static std::vector<transpose_kernel_param_t> get(){
        std::vector<transpose_kernel_param_t> the_list {
            {1, 1, 1, 1, 1, 1},
            {2, 1, 1, 1, 1, 1},
            {4, 1, 1, 1, 1, 1},
            {8, 1, 1, 1, 1, 1},
            {16, 1, 1, 1, 1, 1},
            //{16, 32, 1, 1, 1, 1},
            //{32, 32, 1, 1, 1, 1},

            //{4,  64, 1, 1, 1, 1},
            //{ 64, 4, 1, 1, 1, 1},
            //{4, 128, 1, 1, 1, 1},
            //{128, 4, 1, 1, 1, 1},
            //{4, 256, 1, 1, 1, 1},
            //{256, 4, 1, 1, 1, 1},
        };
        return the_list;
    }
};

template<size_t type_size>
struct transpose_kernel_get_all_param_t{
};

template<>
struct transpose_kernel_get_all_param_t<4>{
    static std::vector<transpose_kernel_param_t> get(){
        std::vector<transpose_kernel_param_t> the_list {
            {16, 16, 1, 1, 1, 1},
            {32, 16, 1, 1, 1, 1},
            {16, 32, 1, 1, 1, 1},
            {32, 32, 1, 1, 1, 1},

            {4,  64, 1, 1, 1, 1},
            { 64, 4, 1, 1, 1, 1},
            {4, 128, 1, 1, 1, 1},
            {128, 4, 1, 1, 1, 1},
            {4, 256, 1, 1, 1, 1},
            {256, 4, 1, 1, 1, 1},
        };
        return the_list;
    }
};

template<>
struct transpose_kernel_get_all_param_t<2>{
    static std::vector<transpose_kernel_param_t> get(){
        std::vector<transpose_kernel_param_t> the_list {
            {16, 16, 1, 1, 1, 1},
            {32, 16, 1, 1, 1, 1},
            {16, 32, 1, 1, 1, 1},
            {32, 32, 1, 1, 1, 1},

            {4,  64, 1, 1, 1, 1},
            { 64, 4, 1, 1, 1, 1},
            {4, 128, 1, 1, 1, 1},
            {128, 4, 1, 1, 1, 1},
            {4, 256, 1, 1, 1, 1},
            {256, 4, 1, 1, 1, 1},

            {32, 32, 2, 2, 2, 2},
            {32, 32, 2, 2, 1, 2},
            {32, 32, 2, 2, 2, 1},
            {32, 32, 2, 2, 1, 1},

            {64, 32, 4, 2, 4, 2},
            {64, 32, 4, 2, 2, 2},
            {64, 32, 4, 2, 2, 1},

            {32, 64, 2, 4, 2, 4},
            {32, 64, 2, 4, 2, 2},
            {32, 64, 2, 4, 1, 2},

            {16, 64, 1, 4, 1, 2},
            {64, 16, 4, 1, 2, 1},

            {64, 64, 4, 4, 4, 4},
            {64, 64, 4, 4, 2, 2},
        };
        return the_list;
    }
};

template<>
struct transpose_kernel_get_all_param_t<1>{
    static std::vector<transpose_kernel_param_t> get(){
        std::vector<transpose_kernel_param_t> the_list {
            {16, 16, 1, 1, 1, 1},
            {32, 16, 1, 1, 1, 1},
            {16, 32, 1, 1, 1, 1},
            {32, 32, 1, 1, 1, 1},

            {4,  64, 1, 1, 1, 1},
            { 64, 4, 1, 1, 1, 1},
            {4, 128, 1, 1, 1, 1},
            {128, 4, 1, 1, 1, 1},
            {4, 256, 1, 1, 1, 1},
            {256, 4, 1, 1, 1, 1},
        };
        return the_list;
    }
};

template<size_t type_size, typename dst_order>
struct reorder_kernel_select_t{
    static hipFunction_t get(const transpose_kernel_param_t * kparam){
        //(0, ...)
        if(dst_order::at(0)==0 && dst_order::at(1)==1 && dst_order::at(2)==3 && dst_order::at(3)==2){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2ncwh;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2ncwh;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2ncwh;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2ncwh;
        }
        else if(dst_order::at(0)==0 && dst_order::at(1)==2 && dst_order::at(2)==1 && dst_order::at(3)==3){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2nhcw;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2nhcw;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2nhcw;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2nhcw;
        }
        else if(dst_order::at(0)==0 && dst_order::at(1)==2 && dst_order::at(2)==3 && dst_order::at(3)==1){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2nhwc;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2nhwc;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2nhwc;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2nhwc;      
        }
        else if(dst_order::at(0)==0 && dst_order::at(1)==3 && dst_order::at(2)==1 && dst_order::at(3)==2){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2nwch;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2nwch;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2nwch;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2nwch;          
        }
        else if(dst_order::at(0)==0 && dst_order::at(1)==3 && dst_order::at(2)==2 && dst_order::at(3)==1){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2nwhc;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2nwhc;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2nwhc;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2nwhc; 
        }
        //(1,...)
        else if(dst_order::at(0)==1 && dst_order::at(1)==0 && dst_order::at(2)==2 && dst_order::at(3)==3){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2cnhw;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2cnhw;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2cnhw;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2cnhw;      
        }
        else if(dst_order::at(0)==1 && dst_order::at(1)==0 && dst_order::at(2)==3 && dst_order::at(3)==2){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2cnwh;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2cnwh;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2cnwh;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2cnwh;           
        }
        else if(dst_order::at(0)==1 && dst_order::at(1)==2 && dst_order::at(2)==0 && dst_order::at(3)==3){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2chnw;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2chnw;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2chnw;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2chnw;  
        }
        else if(dst_order::at(0)==1 && dst_order::at(1)==2 && dst_order::at(2)==3 && dst_order::at(3)==0){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2chwn;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2chwn;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2chwn;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2chwn;        
        }
        else if(dst_order::at(0)==1 && dst_order::at(1)==3 && dst_order::at(2)==0 && dst_order::at(3)==2){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2cwnh;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2cwnh;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2cwnh;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2cwnh;          
        }
        else if(dst_order::at(0)==1 && dst_order::at(1)==3 && dst_order::at(2)==2 && dst_order::at(3)==0){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2cwhn;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2cwhn;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2cwhn;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2cwhn; 
        }
        //(2,...)
        else if(dst_order::at(0)==2 && dst_order::at(1)==0 && dst_order::at(2)==1 && dst_order::at(3)==3){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2hncw;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2hncw;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2hncw;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2hncw;       
        }
        else if(dst_order::at(0)==2 && dst_order::at(1)==0 && dst_order::at(2)==3 && dst_order::at(3)==1){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2hnwc;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2hnwc;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2hnwc;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2hnwc;          
        }
        else if(dst_order::at(0)==2 && dst_order::at(1)==1 && dst_order::at(2)==0 && dst_order::at(3)==3){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2hcnw;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2hcnw;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2hcnw;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2hcnw;
        }
        else if(dst_order::at(0)==2 && dst_order::at(1)==1 && dst_order::at(2)==3 && dst_order::at(3)==0){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2hcwn;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2hcwn;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2hcwn;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2hcwn;      
        }
        else if(dst_order::at(0)==2 && dst_order::at(1)==3 && dst_order::at(2)==0 && dst_order::at(3)==1){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2hwnc;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2hwnc;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2hwnc;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2hwnc;          
        }
        else if(dst_order::at(0)==2 && dst_order::at(1)==3 && dst_order::at(2)==1 && dst_order::at(3)==0){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2hwcn;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2hwcn;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2hwcn;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2hwcn;  
        }
        //(3,...)
        else if(dst_order::at(0)==3 && dst_order::at(1)==0 && dst_order::at(2)==1 && dst_order::at(3)==2){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2wnch;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2wnch;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2wnch;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2wnch;      
        }
        else if(dst_order::at(0)==3 && dst_order::at(1)==0 && dst_order::at(2)==2 && dst_order::at(3)==1){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2wnhc;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2wnhc;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2wnhc;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2wnhc;           
        }
        else if(dst_order::at(0)==3 && dst_order::at(1)==1 && dst_order::at(2)==0 && dst_order::at(3)==2){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2wcnh;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2wcnh;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2wcnh;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2wcnh; 
        }
        else if(dst_order::at(0)==3 && dst_order::at(1)==1 && dst_order::at(2)==2 && dst_order::at(3)==0){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2wchn;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2wchn;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2wchn;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2wchn;       
        }
        else if(dst_order::at(0)==3 && dst_order::at(1)==2 && dst_order::at(2)==0 && dst_order::at(3)==1){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2whnc;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2whnc;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2whnc;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2whnc;           
        }
        //else if(dst_order::at(0)==3 && dst_order::at(1)==2 && dst_order::at(2)==1 && dst_order::at(3)==0){
        else{
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2whcn;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2whcn;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2whcn;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2whcn;          
        }   
    }
};

template<typename dst_order>
struct reorder_kernel_select_t<4, dst_order>{
    static hipFunction_t get(const transpose_kernel_param_t * kparam){
        //(0, ...)
        if(dst_order::at(0)==0 && dst_order::at(1)==1 && dst_order::at(2)==3 && dst_order::at(3)==2){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2ncwh;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2ncwh;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2ncwh;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2ncwh;
        }
        else if(dst_order::at(0)==0 && dst_order::at(1)==2 && dst_order::at(2)==1 && dst_order::at(3)==3){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2nhcw;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2nhcw;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2nhcw;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2nhcw;
        }
        else if(dst_order::at(0)==0 && dst_order::at(1)==2 && dst_order::at(2)==3 && dst_order::at(3)==1){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2nhwc;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2nhwc;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2nhwc;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2nhwc;      
        }
        else if(dst_order::at(0)==0 && dst_order::at(1)==3 && dst_order::at(2)==1 && dst_order::at(3)==2){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2nwch;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2nwch;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2nwch;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2nwch;          
        }
        else if(dst_order::at(0)==0 && dst_order::at(1)==3 && dst_order::at(2)==2 && dst_order::at(3)==1){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2nwhc;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2nwhc;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2nwhc;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2nwhc; 
        }
        //(1,...)
        else if(dst_order::at(0)==1 && dst_order::at(1)==0 && dst_order::at(2)==2 && dst_order::at(3)==3){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2cnhw;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2cnhw;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2cnhw;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2cnhw;      
        }
        else if(dst_order::at(0)==1 && dst_order::at(1)==0 && dst_order::at(2)==3 && dst_order::at(3)==2){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2cnwh;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2cnwh;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2cnwh;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2cnwh;           
        }
        else if(dst_order::at(0)==1 && dst_order::at(1)==2 && dst_order::at(2)==0 && dst_order::at(3)==3){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2chnw;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2chnw;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2chnw;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2chnw;  
        }
        else if(dst_order::at(0)==1 && dst_order::at(1)==2 && dst_order::at(2)==3 && dst_order::at(3)==0){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2chwn;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2chwn;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2chwn;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2chwn;        
        }
        else if(dst_order::at(0)==1 && dst_order::at(1)==3 && dst_order::at(2)==0 && dst_order::at(3)==2){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2cwnh;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2cwnh;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2cwnh;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2cwnh;          
        }
        else if(dst_order::at(0)==1 && dst_order::at(1)==3 && dst_order::at(2)==2 && dst_order::at(3)==0){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2cwhn;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2cwhn;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2cwhn;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2cwhn; 
        }
        //(2,...)
        else if(dst_order::at(0)==2 && dst_order::at(1)==0 && dst_order::at(2)==1 && dst_order::at(3)==3){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2hncw;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2hncw;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2hncw;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2hncw;       
        }
        else if(dst_order::at(0)==2 && dst_order::at(1)==0 && dst_order::at(2)==3 && dst_order::at(3)==1){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2hnwc;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2hnwc;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2hnwc;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2hnwc;          
        }
        else if(dst_order::at(0)==2 && dst_order::at(1)==1 && dst_order::at(2)==0 && dst_order::at(3)==3){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2hcnw;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2hcnw;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2hcnw;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2hcnw;
        }
        else if(dst_order::at(0)==2 && dst_order::at(1)==1 && dst_order::at(2)==3 && dst_order::at(3)==0){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2hcwn;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2hcwn;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2hcwn;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2hcwn;      
        }
        else if(dst_order::at(0)==2 && dst_order::at(1)==3 && dst_order::at(2)==0 && dst_order::at(3)==1){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2hwnc;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2hwnc;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2hwnc;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2hwnc;          
        }
        else if(dst_order::at(0)==2 && dst_order::at(1)==3 && dst_order::at(2)==1 && dst_order::at(3)==0){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2hwcn;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2hwcn;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2hwcn;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2hwcn;  
        }
        //(3,...)
        else if(dst_order::at(0)==3 && dst_order::at(1)==0 && dst_order::at(2)==1 && dst_order::at(3)==2){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2wnch;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2wnch;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2wnch;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2wnch;      
        }
        else if(dst_order::at(0)==3 && dst_order::at(1)==0 && dst_order::at(2)==2 && dst_order::at(3)==1){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2wnhc;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2wnhc;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2wnhc;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2wnhc;           
        }
        else if(dst_order::at(0)==3 && dst_order::at(1)==1 && dst_order::at(2)==0 && dst_order::at(3)==2){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2wcnh;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2wcnh;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2wcnh;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2wcnh; 
        }
        else if(dst_order::at(0)==3 && dst_order::at(1)==1 && dst_order::at(2)==2 && dst_order::at(3)==0){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2wchn;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2wchn;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2wchn;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2wchn;       
        }
        else if(dst_order::at(0)==3 && dst_order::at(1)==2 && dst_order::at(2)==0 && dst_order::at(3)==1){
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2whnc;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2whnc;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2whnc;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2whnc;           
        }
        //else if(dst_order::at(0)==3 && dst_order::at(1)==2 && dst_order::at(2)==1 && dst_order::at(3)==0){
        else{
            if(kparam->tile_x == 1)        return the_reorder_gpu_handle.kernel_general_4d_reorder_1p_dword_nchw2whcn;
            else if(kparam->tile_x == 4)   return the_reorder_gpu_handle.kernel_general_4d_reorder_4p_dword_nchw2whcn;
            else if(kparam->tile_x == 8)   return the_reorder_gpu_handle.kernel_general_4d_reorder_8p_dword_nchw2whcn;
            else if(kparam->tile_x ==16)  return the_reorder_gpu_handle.kernel_general_4d_reorder_16p_dword_nchw2whcn;          
        } 
    }
};

template<size_t type_size>
struct transpose_kernel_select_t{
};

template<>
struct transpose_kernel_select_t<4>{
    static hipFunction_t get(const transpose_kernel_param_t * kparam){
        if(kparam->tile_x == 16 && kparam->tile_y == 16){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_16x16_dword;
                }
            }
        }
        else if(kparam->tile_x == 32 && kparam->tile_y == 16){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_32x16_dword;
                }
            }
        }
        else if(kparam->tile_x == 16 && kparam->tile_y == 32){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_16x32_dword;
                }
            }
        }
        else if(kparam->tile_x == 32 && kparam->tile_y == 32){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_32x32_dword;
                }
            }
        }
        else if(kparam->tile_x == 4 && kparam->tile_y == 128){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_4x128_dword;
                }
            }
        }
        else if(kparam->tile_x == 128 && kparam->tile_y == 4){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_128x4_dword;
                }
            }
        }
        else if(kparam->tile_x == 4 && kparam->tile_y == 64){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_4x64_dword;
                }
            }
        }
        else if(kparam->tile_x == 64 && kparam->tile_y == 4){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_64x4_dword;
                }
            }
        }
        else if(kparam->tile_x == 4 && kparam->tile_y == 256){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_4x256_dword;
                }
            }
        }
        else if(kparam->tile_x == 256 && kparam->tile_y == 4){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_256x4_dword;
                }
            }
        }
    }
};

template<>
struct transpose_kernel_select_t<2>{
    static hipFunction_t get(const transpose_kernel_param_t * kparam){
        if(kparam->tile_x == 16 && kparam->tile_y == 16){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_16x16_half;
                }
            }
        }
        else if(kparam->tile_x == 32 && kparam->tile_y == 16){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_32x16_half;
                }
            }
        }
        else if(kparam->tile_x == 16 && kparam->tile_y == 32){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_16x32_half;
                }
            }
        }
        else if(kparam->tile_x == 4 && kparam->tile_y == 128){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_4x128_half;
                }
            }
        }
        else if(kparam->tile_x == 128 && kparam->tile_y == 4){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_128x4_half;
                }
            }
        }
        else if(kparam->tile_x == 4 && kparam->tile_y == 64){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_4x64_half;
                }
            }
        }
        else if(kparam->tile_x == 64 && kparam->tile_y == 4){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_64x4_half;
                }
            }
        }
        else if(kparam->tile_x == 4 && kparam->tile_y == 256){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_4x256_half;
                }
            }
        }
        else if(kparam->tile_x == 256 && kparam->tile_y == 4){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_256x4_half;
                }
            }
        }
        else if(kparam->tile_x == 32 && kparam->tile_y == 32){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_32x32_half;
                }
            }
            else if(kparam->pack_x == 2 && kparam->pack_y == 2){
                if(kparam->ediv_x == 2 && kparam->ediv_y == 2){
                    return the_transpose_gpu_handle.kernel_batched_transpose_32x32_pack_2x2_ediv_2x2_half;
                }
                else if(kparam->ediv_x == 1 && kparam->ediv_y == 2){
                    return the_transpose_gpu_handle.kernel_batched_transpose_32x32_pack_2x2_ediv_1x2_half;
                }
                else if(kparam->ediv_x == 2 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_32x32_pack_2x2_ediv_2x1_half;
                }
                else if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_32x32_pack_2x2_ediv_1x1_half;
                }
            }
        }
        // else if(tile_x == 32 && tile_y == 64){
        //     if(pack_x == 2 && pack_y == 4)
        //         return the_transpose_gpu_handle.kernel_batched_transpose_32x64_pack_2x4_half;
        // }
        else if(kparam->tile_x == 64 && kparam->tile_y == 32){
            if(kparam->pack_x == 4 && kparam->pack_y == 2){
                if(kparam->ediv_x == 4 && kparam->ediv_y == 2){
                    return the_transpose_gpu_handle.kernel_batched_transpose_64x32_pack_4x2_ediv_4x2_half;
                }
                else if(kparam->ediv_x == 2 && kparam->ediv_y == 2){
                    return the_transpose_gpu_handle.kernel_batched_transpose_64x32_pack_4x2_ediv_2x2_half;
                }
                else if(kparam->ediv_x == 2 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_64x32_pack_4x2_ediv_2x1_half;
                }
            }
        }
        else if(kparam->tile_x == 32 && kparam->tile_y == 64){
            if(kparam->pack_x == 2 && kparam->pack_y == 4){
                if(kparam->ediv_x == 2 && kparam->ediv_y == 4){
                    return the_transpose_gpu_handle.kernel_batched_transpose_32x64_pack_2x4_ediv_2x4_half;
                }
                else if(kparam->ediv_x == 2 && kparam->ediv_y == 2){
                    return the_transpose_gpu_handle.kernel_batched_transpose_32x64_pack_2x4_ediv_2x2_half;
                }
                else if(kparam->ediv_x == 1 && kparam->ediv_y == 2){
                    return the_transpose_gpu_handle.kernel_batched_transpose_32x64_pack_2x4_ediv_1x2_half;
                }
            }
        }
        else if(kparam->tile_x == 16 && kparam->tile_y == 64){
            if(kparam->pack_x == 1 && kparam->pack_y == 4){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 2){
                    return the_transpose_gpu_handle.kernel_batched_transpose_16x64_pack_1x4_ediv_1x2_half;
                }
            }
        }
        else if(kparam->tile_x == 64 && kparam->tile_y == 16){
            if(kparam->pack_x == 4 && kparam->pack_y == 1){
                if(kparam->ediv_x == 2 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_64x16_pack_4x1_ediv_2x1_half;
                }
            }
        }
        else if(kparam->tile_x == 64 && kparam->tile_y == 64){
            if(kparam->pack_x == 4 && kparam->pack_y == 4){
                if(kparam->ediv_x == 4 && kparam->ediv_y == 4){
                    return the_transpose_gpu_handle.kernel_batched_transpose_64x64_pack_4x4_ediv_4x4_half;
                }
                else if(kparam->ediv_x == 2 && kparam->ediv_y == 2){
                    return the_transpose_gpu_handle.kernel_batched_transpose_64x64_pack_4x4_ediv_2x2_half;
                }
            }
        }
        assert(false);
    }
};

template<>
struct transpose_kernel_select_t<1>{
    static hipFunction_t get(const transpose_kernel_param_t * kparam){
        if(kparam->tile_x == 16 && kparam->tile_y == 16){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_16x16_byte;
                }
            }
        }
        else if(kparam->tile_x == 32 && kparam->tile_y == 16){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_32x16_byte;
                }
            }
        }
        else if(kparam->tile_x == 16 && kparam->tile_y == 32){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_16x32_byte;
                }
            }
        }
        else if(kparam->tile_x == 32 && kparam->tile_y == 32){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_32x32_byte;
                }
            }
        }
        else if(kparam->tile_x == 4 && kparam->tile_y == 128){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_4x128_byte;
                }
            }
        }
        else if(kparam->tile_x == 128 && kparam->tile_y == 4){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_128x4_byte;
                }
            }
        }
        else if(kparam->tile_x == 4 && kparam->tile_y == 64){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_4x64_byte;
                }
            }
        }
        else if(kparam->tile_x == 64 && kparam->tile_y == 4){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_64x4_byte;
                }
            }
        }
        else if(kparam->tile_x == 4 && kparam->tile_y == 256){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_4x256_byte;
                }
            }
        }
        else if(kparam->tile_x == 256 && kparam->tile_y == 4){
            if(kparam->pack_x == 1 && kparam->pack_y == 1){
                if(kparam->ediv_x == 1 && kparam->ediv_y == 1){
                    return the_transpose_gpu_handle.kernel_batched_transpose_256x4_byte;
                }
            }
        }
    }
};

bool transpose_kernel_is_valid(uint32_t batch, uint32_t height, uint32_t width, const transpose_kernel_param_t * kparam)
{
    (void)batch;
    return width % kparam->ediv_x == 0 && height % kparam->ediv_y == 0;
}

template<typename T>
void gpu_batched_transpose(T * dst, T * src, uint32_t batch, uint32_t height, uint32_t width, const transpose_kernel_param_t * kparam)
{
    hipDeviceProp_t dev_prop;
    hipDevice_t dev;
    HIP_CALL(hipGetDevice(&dev));
    HIP_CALL(hipGetDeviceProperties(&dev_prop, dev));

    // TODO: need find better way to decide transpose tile size
    uint32_t dim_h = (height + kparam->tile_y - 1) / kparam->tile_y;
    uint32_t dim_w = (width  + kparam->tile_x - 1) / kparam->tile_x;
    uint32_t dim_total = batch * dim_h * dim_w;

    size_t block_size = 256;
#if BATCHED_TRANSPOSE_PERSISTENT
    int num_cu = dev_prop.multiProcessorCount;
    const int occupancy = 4;
    size_t grid_size = num_cu * occupancy;
#else
    size_t grid_size = batch * dim_h * dim_w;
#endif

    magic_div_u32_t magic_h = magic_div_u32_gen(dim_h);
    magic_div_u32_t magic_w = magic_div_u32_gen(dim_w);

    hipFunction_t kernel = transpose_kernel_select_t<sizeof(T)>::get(kparam);

    transpose_kernel_t karg;
    karg.p_dst          = reinterpret_cast<void*>(dst);
    karg.p_src          = reinterpret_cast<void*>(src);
    karg.height         = height;
    karg.width          = width;
    karg.dim_stride     = grid_size;
    karg.dim_total      = dim_total;
    karg.magic_h        = magic_h.magic;
    karg.shift_h        = magic_h.shift;
    karg.magic_w        = magic_w.magic;
    karg.shift_w        = magic_w.shift;
    size_t karg_size    = sizeof(karg);

    // dump_transpose_kernel_arg(&karg);

    void *config[] = {HIP_LAUNCH_PARAM_BUFFER_POINTER, &karg,
                        HIP_LAUNCH_PARAM_BUFFER_SIZE, &karg_size,
                        HIP_LAUNCH_PARAM_END};
    //Hcc version using grid_size*block_size(total thread number) while None-Hcc version using grid_size(workgroup number)
    HIP_CALL(hipExtModuleLaunchKernel(kernel, grid_size * block_size, 1, 1,
                                            block_size, 1, 1, 0, 0, NULL,
                                            (void **)&config, NULL, NULL));
}

template<typename T, typename dst_order>
void gpu_general_tensor_reorder(T * dst, T * src, uint32_t batch, uint32_t channel, uint32_t height, uint32_t width, const transpose_kernel_param_t * kparam)
{
    hipDeviceProp_t dev_prop;
    hipDevice_t dev;
    HIP_CALL(hipGetDevice(&dev));
    HIP_CALL(hipGetDeviceProperties(&dev_prop, dev));

    // TODO: need find better way to decide transpose tile size
    uint32_t pixel_total =  batch * channel * height * width;
    size_t block_size = 256;
    uint32_t dim_total = (pixel_total+ block_size * kparam->tile_x -1) / (block_size * kparam->tile_x);

#if BATCHED_TRANSPOSE_PERSISTENT
    int num_cu = dev_prop.multiProcessorCount;
    const int occupancy = 4;
    size_t grid_size = num_cu * occupancy;
#else
    size_t grid_size = dim_total;
#endif

    magic_div_u32_t magic_stride0 = magic_div_u32_gen(channel * height * width);
    magic_div_u32_t magic_stride1 = magic_div_u32_gen(height * width);
    magic_div_u32_t magic_stride2 = magic_div_u32_gen(width);
    //loop over
    hipFunction_t kernel = reorder_kernel_select_t<sizeof(T), dst_order>::get(kparam);

    reorder_kernel_t karg;
    karg.p_dst          = reinterpret_cast<void*>(dst);
    karg.p_src          = reinterpret_cast<void*>(src);
    karg.dim_0          = batch;
    karg.dim_1          = channel;
    karg.dim_2          = height;
    karg.dim_3          = width;
    karg.dim_stride     = grid_size;
    karg.dim_total      = dim_total;
    karg.magic_stride0  = magic_stride0.magic;
    karg.shift_stride0  = magic_stride0.shift;
    karg.magic_stride1  = magic_stride1.magic;
    karg.shift_stride1  = magic_stride1.shift;
    karg.magic_stride2  = magic_stride2.magic;
    karg.shift_stride2  = magic_stride2.shift;
    size_t karg_size    = sizeof(karg);


    void *config[] = {HIP_LAUNCH_PARAM_BUFFER_POINTER, &karg,
                        HIP_LAUNCH_PARAM_BUFFER_SIZE, &karg_size,
                        HIP_LAUNCH_PARAM_END};

    HIP_CALL(hipExtModuleLaunchKernel(kernel, grid_size * block_size, 1, 1,
                                            block_size, 1, 1, 0, 0, NULL,
                                            (void **)&config, NULL, NULL));
}
#endif
