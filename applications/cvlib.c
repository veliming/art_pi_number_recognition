/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-19     jiong       the first version
 */
#include "cvlib.h"
#include "rtdef.h"

#define ImageGray_Pixel(inImg,i,j) ((inImg)->data[(inImg)->width*(i)+j])

uint8_t * rgb565_malloc(struct Rgb565BufStruct *pb,uint16_t width,uint16_t height)
{
    pb->width = width;
    pb->height = height;
    pb->data = rt_malloc(width*height*2);
    return pb->data;
}

uint8_t * gray_malloc(struct GrayBufStruct *pb,uint16_t width,uint16_t height)
{
    pb->width = width;
    pb->height = height;
    pb->data = rt_malloc(width*height);
    return pb->data;
}

uint16_t gray8_to_gray16(uint8_t value)
{
    uint16_t rgb565_gray =0;
    rgb565_gray |= (value<<10) & 0xf800;
    rgb565_gray |= (value<<5) & 0x07e0;
    rgb565_gray |= (value>>1) & 0x001f;
    return rgb565_gray;
}

uint8_t gray8_to_gray_bin(uint8_t value,uint8_t binaryThresh)
{
    return value<=binaryThresh?0:255;
}

uint8_t rgb565_to_gray8(uint16_t pixel)
{
    uint8_t r,g,b;
    r = ((pixel >> 11) & 0x1f)<<1;
    g = ((pixel >> 5) & 0x3f);
    b = ((pixel) & 0x1f)<<1;
    uint8_t value = (r*19595 + g*38469 + b*7472) >> 16;
    return value;
}

void rgb565_add_roi()
{

}

void get_roi_img(struct GrayBufStruct *inImg,struct GrayBufStruct *outImg,uint16_t si,uint16_t sj,uint16_t ei,uint16_t ej){
    uint16_t i,j;
    uint8_t *pDst=outImg->data;
    uint8_t *pSrc=inImg->data;

    for(j=sj;j<ej;j++){
        for(i=si;i<ei;i++,pDst++){
            *pDst=pSrc[j*inImg->width+i];
        }
    }
}

uint8_t extract_connect_omponents(struct GrayBufStruct *inImg,struct GrayBufStruct *ccImg,uint8_t* stack,uint8_t v, uint8_t type){
    uint16_t i,j,k,r,c,ti,tj;
    uint8_t label=255;
    int8_t neighbors[8*2]={
        -1,0, //left
        0,-1, //top
        1,0,  //right
        0,1,  //bottom
        -1,-1,//left-top
        1,-1, //right-top
        -1,1, //left-bottom
        1,1  //right-bottom
    };
    int32_t top=0;
    memset(stack,0,STACK_SIZE);
    memset(ccImg->data,0,ccImg->height*ccImg->width);

    ti=0;
    while(ti<(ccImg->width))
    {
        i=(ccImg->height/2);
        j=ti;
        if(ImageGray_Pixel(inImg,i,j)!=v || ImageGray_Pixel(ccImg,i,j)!=0)
        {
            ti++;
            continue;

        }
        label--;

        ((uint16_t*)stack)[top*2]=i;
        ((uint16_t*)stack)[top*2+1]=j;
        top++;


        ImageGray_Pixel(ccImg,i,j)=label;

        while(top!=0){
                top--;
                ti=((uint16_t*)stack)[top*2];
                tj=((uint16_t*)stack)[top*2+1];

                for(k=0;k<type;k++){
                    r=ti+neighbors[k*2];
                    c=tj+neighbors[k*2+1];
                    if(r<0 || r>=inImg->height || c<0 || c>=inImg->width)
                        continue;

                    if(ImageGray_Pixel(inImg,r,c)!=v ||
                        ImageGray_Pixel(ccImg,r,c)!=0)
                    continue;

                    ImageGray_Pixel(ccImg,r,c)=label;

                    ((uint16_t*)stack)[top*2]=r;
                    ((uint16_t*)stack)[top*2+1]=c;
                    top++;
                }
        }
    }
    //rt_free(stack);
    return 255-label;
}

void compute_componentBBox(struct GrayBufStruct *gray,uint16_t bbox[4],uint16_t componentId)
{
    uint16_t i,j;
    uint8_t *pSrc=gray->data;

    uint16_t x0=gray->width,x1=0,y0=gray->height,y1=0;
    for(i=0;i<gray->height;i++){
        for(j=0;j<gray->width;j++,pSrc++){
            if(*pSrc==(254-componentId))
            {
                if(x0>j) x0=j;
                if(x1<j) x1=j;
                if(y0>i) y0=i;
                if(y1<i) y1=i;
            }
        }
    }
    bbox[0]=x0;
    bbox[1]=y0;
    bbox[2]=x1;
    bbox[3]=y1;
}


void ccNum_save(struct GrayBufStruct *ccGrayImg,struct GrayBufStruct *ccNumImg,uint16_t bbox[4])
{
    uint16_t i,j;
    uint16_t x0,x1,y0,y1;
    uint8_t *pDst=ccNumImg->data;
    uint8_t *pSrc=ccGrayImg->data;
    x0=bbox[0];
    y0=bbox[1];
    x1=bbox[2];
    y1=bbox[3];
    for(j=y0;j<=y1;j++){
        for(i=x0;i<=x1;i++,pDst++){
            *pDst=pSrc[j*ccGrayImg->width+i];
        }
    }
}
static uint8_t cross_num(struct GrayBufStruct *ccNumImg,uint16_t v,uint16_t i,uint8_t type)
{
    uint16_t j=0,k=0;
        k=0;
        j=0;
    if(type==0){
        while(j<ccNumImg->width){
            while((j<ccNumImg->width)&&(ImageGray_Pixel(ccNumImg,i,j)!=v))
                j++;
            if(j==ccNumImg->width)
                return k;
            k++;
            while((j<ccNumImg->width) && (ImageGray_Pixel(ccNumImg,i,j)==v))
                j++;
        }
    }
    else if(type==1){
    k=0;
    j=0;
    while(j<ccNumImg->height){
        while(j<ccNumImg->height&&ImageGray_Pixel(ccNumImg,j,i)!=v)
            j++;
        if(j==ccNumImg->height)
            return k;
        k++;
        while(j<ccNumImg->height&&ImageGray_Pixel(ccNumImg,j,i)==v)
            j++;
    }
    }
        return k;
}

static uint8_t vertThrough(struct GrayBufStruct *ccNumImg,uint16_t v){
    uint8_t x=0,count=0;
    uint32_t i=0,j;
    uint8_t *pSrc=ccNumImg->data;
    for(i=0;i<ccNumImg->width;i++)
    {
        j=1;
        while(pSrc[j*ccNumImg->width+i]==v)
        {
            j++;
            count++;
        }

        if(count>=(ccNumImg->height*0.8))
        {
            x=i;
            break;
        }
        else
        {
            count=0;
        }
    }
    return x;
}

static uint8_t horiThrough(struct GrayBufStruct *ccNumImg,uint16_t v)
{
    uint8_t y=0,count=0;
    uint32_t i=0,j=0;
    uint8_t *pSrc=ccNumImg->data;
    for(i=ccNumImg->height/2;i<ccNumImg->height;i++)
    {
        j=2;
        while(pSrc[i*ccNumImg->width+j]==v)
        {
            j++;
            count++;
        }
        if(count>=(ccNumImg->width-2))
        {
            y=i;
            break;
        }
        else
        {
            count=0;
        }
    }
    return y;
}

static uint8_t ImageGray_pixNum(struct GrayBufStruct *ccNumImg,uint16_t v,uint16_t si,uint16_t sj,uint16_t ei,uint16_t ej)
{
    uint8_t pixNum=0;
    uint32_t i=0,j=0;
    uint8_t *pSrc=ccNumImg->data;

    for(j=sj;j<ej;j++)
    {
        i=si;

        while(i<ei)
        {
            if(pSrc[ccNumImg->width*j+i]!=v)
            {
                i++;
            }
            else
            {
                pixNum++;
                break;
            }

        }
    }
    return pixNum;
}
uint8_t detect_digital(struct GrayBufStruct *ccNumImg,uint16_t v){
    uint16_t detectNum=-1;
    uint8_t vertThrough_x=0,horiThrough_y=0;
    uint16_t horiCrossNumCenter;//
    uint16_t vertCrossNumCenter;//

    horiCrossNumCenter=cross_num(ccNumImg,v,ccNumImg->height/2,0);
    vertCrossNumCenter=cross_num(ccNumImg,v,ccNumImg->width/2,1);
    vertThrough_x=vertThrough(ccNumImg,v);
    horiThrough_y=horiThrough(ccNumImg,v);

    if(vertThrough_x!=0&&(vertCrossNumCenter!=3)&&(horiThrough_y!=0))
    {
        if((ccNumImg->height-horiThrough_y)<=4&&horiThrough_y!=0)
            return 1;
        else return 4;

    }

    if(vertCrossNumCenter==2)
    {
        if(horiCrossNumCenter==1)
            return 7;
        else
            return 0;
    }


    if(ImageGray_pixNum(ccNumImg,v,0,0,ccNumImg->width/2,ccNumImg->height/2)<(ccNumImg->height/2-4))
    {
        if(ImageGray_pixNum(ccNumImg,v,ccNumImg->width/2,ccNumImg->height/2,ccNumImg->width,ccNumImg->height)<(ccNumImg->height/2-1))
        return 2;
        else
            return 3;
    }
    else
    {
        if(ImageGray_pixNum(ccNumImg,v,ccNumImg->width/2,0,ccNumImg->width,ccNumImg->height/2)<(ccNumImg->height/2))
        {
            if(ImageGray_pixNum(ccNumImg,v,0,ccNumImg->height/2,ccNumImg->width/2,ccNumImg->height)<(ccNumImg->height/2))
                return 5;
            else
                return 6;
        }
        else
        {
            if(ImageGray_pixNum(ccNumImg,v,0,ccNumImg->height/2,ccNumImg->width/2,ccNumImg->height)<(ccNumImg->height/2))
                return 9;
            else
                return 8;
        }

    }
    return detectNum;
}


