/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-19     jiong       the first version
 */
#ifndef APPLICATIONS_CVLIB_H_
#define APPLICATIONS_CVLIB_H_
#include "stdint.h"
#define STACK_SIZE 100*1024
struct Rgb565BufStruct
{
    uint16_t *data;
    uint16_t width;
    uint16_t height;
};

struct GrayBufStruct
{
    uint8_t *data;
    uint16_t width;
    uint16_t height;
};

uint8_t * rgb565_malloc(struct Rgb565BufStruct *pb,uint16_t width,uint16_t height);
uint8_t * gray_malloc(struct GrayBufStruct *pb,uint16_t width,uint16_t height);
uint16_t gray8_to_gray16(uint8_t value);
uint8_t gray8_to_gray_bin(uint8_t value,uint8_t binaryThresh);
uint8_t rgb565_to_gray8(uint16_t pixel);
void get_roi_img(struct GrayBufStruct *inImg,struct GrayBufStruct *outImg,uint16_t si,uint16_t sj,uint16_t ei,uint16_t ej);
uint8_t extract_connect_omponents(struct GrayBufStruct *inImg,struct GrayBufStruct *ccImg,uint8_t* ccStack,uint8_t v, uint8_t type);
void compute_componentBBox(struct GrayBufStruct *gray,uint16_t bbox[4],uint16_t componentId);
void _ccNum_save(struct GrayBufStruct *ccGrayImg,struct GrayBufStruct *ccNumImg,uint16_t bbox[4]);
#endif /* APPLICATIONS_CVLIB_H_ */
