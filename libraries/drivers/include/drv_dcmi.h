/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-07-27     thread-liu        the first version
 */

#ifndef __DRV_DCMI_H__
#define __DRV_DCMI_H__

#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

struct rt_dcmi_device
{
    struct rt_device parent;
};

extern DMA_HandleTypeDef hdma_dcmi;
extern void rt_hw_dcmi_dma_config(rt_uint32_t dst_addr1, rt_uint32_t dst_addr2, rt_uint32_t len);

#ifdef USE_MULTI_BUFFER
extern void DCMI_Start(void);
extern void DCMI_Stop(void);
#else
extern void DCMI_Start(uint32_t* JpegBuffer,uint32_t pictureBufferLength);
extern void DCMI_Stop(void);
#endif

#ifdef __cplusplus
}
#endif

#endif
