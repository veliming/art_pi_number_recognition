/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-08-03     thread-liu        the first version
 */

#include "board.h"

#if defined(BSP_USING_OV7670)

#include <dfs_posix.h>
#include <drv_ov7670.h>
#include <drv_dcmi.h>

#define DRV_DEBUG
#define LOG_TAG     "drv.ov7670"
#include <drv_log.h>

#define DEV_ADDRESS      0x21 /* OV7670 address */
#define I2C_NAME        "i2c2"

struct rt_i2c_bus_device *i2c_bus  = RT_NULL;

#define RGB565_BUF_SIZE   320*240*2


#define RESET_PIN  GET_PIN(A, 3)

static rt_uint32_t *rgb565_data_buf = RT_NULL;

uint8_t get_picture_done =0;

static rt_err_t read_reg(struct rt_i2c_bus_device *bus, rt_uint8_t reg, rt_uint8_t len, rt_uint8_t *buf)
{
    struct rt_i2c_msg msg[2];

    RT_ASSERT(bus != RT_NULL);

    msg[0].addr  = DEV_ADDRESS;
    msg[0].flags = RT_I2C_WR;
    msg[0].buf   = &reg;
    msg[0].len   = 2;

    msg[1].addr  = DEV_ADDRESS;
    msg[1].flags = RT_I2C_RD;
    msg[1].len   = len;
    msg[1].buf   = buf;

    if (rt_i2c_transfer(bus, msg, 2) == 2)
    {
        return RT_EOK;
    }

    return RT_ERROR;
}

/* i2c write reg */
static rt_err_t write_reg(struct rt_i2c_bus_device *bus, rt_uint8_t reg, rt_uint8_t data)
{
    rt_uint8_t buf[2];
    struct rt_i2c_msg msgs;

    RT_ASSERT(bus != RT_NULL);

    buf[0] = reg ;
    buf[1] = data;

    msgs.addr = DEV_ADDRESS;
    msgs.flags = RT_I2C_WR;
    msgs.buf = buf;
    msgs.len = 2;

    if (rt_i2c_transfer(bus, &msgs, 1) == 1)
    {
        return RT_EOK;
    }

    return RT_ERROR;
}

void camera_dma_data_process(void)
{

}

///* After a frame of picture data has been collected. */
//void camera_frame_data_process(void)
//{
//    get_picture_done   = 1;
//}

int rt_ov7670_init(void)
{
	uint8_t OV7670_MIDH,OV7670_MIDL;
	uint16_t OV7670_MID;
	uint8_t OV7670_PID,OV7670_VER;
    rt_err_t result = RT_EOK;
    rt_device_t dcmi_dev = RT_NULL;

    /* ov2640 reset */
    rt_pin_mode(RESET_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(RESET_PIN, PIN_LOW);
    rt_thread_delay(20);
    rt_pin_write(RESET_PIN, PIN_HIGH);
    rt_thread_delay(20);

    rt_thread_mdelay(20);
    i2c_bus = rt_i2c_bus_device_find(I2C_NAME);
    if (i2c_bus == RT_NULL)
    {
        LOG_E("can't find %s deivce", I2C_NAME);
        return RT_ERROR;
    }

    /* Prepare the camera to be configured */
    result = write_reg(i2c_bus, 0xff, 0x01);
    if (result != RT_EOK )
    {
        LOG_E("ov7670 write reg error!");
        return RT_ERROR;
    }
    rt_thread_delay(10);
    result = write_reg(i2c_bus, 0x12, 0x80);
    if (result != RT_EOK)
    {
        LOG_E("ov7670 soft reset error!");
        return RT_ERROR;
    }
    rt_thread_mdelay(20);


    read_reg(i2c_bus, 0x1c, 1, &OV7670_MIDH);
	read_reg(i2c_bus, 0x1d, 1, &OV7670_MIDL);
	read_reg(i2c_bus, 0x0A, 1, &OV7670_PID);
	read_reg(i2c_bus, 0x0B, 1, &OV7670_VER);
	//rt_kprintf("ov7670 read midh=%d\n",OV7670_MIDH);
	//rt_kprintf("ov7670 read midl=%d\n",OV7670_MIDL);

    OV7670_MID = (OV7670_MIDH<<8&0xff00)|(OV7670_MIDL&0x00ff);
    rt_kprintf("ov7670 read mid=0x%04x\n",OV7670_MID);
    rt_kprintf("ov7670 read pid=0x%02x\n",OV7670_PID);
    rt_kprintf("ov7670 read ver=0x%02x\n",OV7670_VER);
    if (OV7670_MID != 0X7FA2)
     {
         LOG_E("ov7670 read id error!");
         //return RT_ERROR;
     }

    for(uint32_t i=0;i<OV7670_REG_NUM;i++)
    {
        write_reg(i2c_bus, OV7670_reg[i][0], OV7670_reg[i][1]);
    }
    dcmi_dev = rt_device_find("dcmi");
    if (dcmi_dev == RT_NULL)
    {
        LOG_E("can't find dcmi device!");
        return RT_ERROR;
    }
    rt_device_open(dcmi_dev, RT_DEVICE_FLAG_RDWR);

   rgb565_data_buf = rt_malloc(RGB565_BUF_SIZE);
   if (RT_NULL == rgb565_data_buf)
   {
       rt_kprintf("rgb565 data buf malloc error!\n");
       return RT_ERROR;
   }

    rt_kprintf("camera init success!\n");

    return RT_EOK;
}
INIT_APP_EXPORT(rt_ov7670_init);

int camera_sample(int argc, char **argv)
{
   rt_err_t result = RT_EOK;
   int fd = -1;
   rt_memset(rgb565_data_buf,0,RGB565_BUF_SIZE);
   get_picture_done = 0;
   DCMI_Start(rgb565_data_buf,(uint32_t)RGB565_BUF_SIZE);

   while (1)
   {
       while (get_picture_done != 1);
      
       DCMI_Stop();
		fd = open("/sdcard/rgbpic.bin", O_WRONLY | O_CREAT);
	   if (fd < 0)
	   {
		   rt_kprintf("open file for recording failed!\n");
		   result = -RT_ERROR;
		   goto _exit;
	   }
	   else
	   {
		   write(fd, rgb565_data_buf, RGB565_BUF_SIZE);
		   close(fd);
		   rt_kprintf("picture capture complate!\n");
		   break;
	   }
   }

_exit:
    return result;
}
MSH_CMD_EXPORT(camera_sample, record picture to a bin file);

#endif
