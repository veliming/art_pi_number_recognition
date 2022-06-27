#include <rtthread.h>
#include <rtdevice.h>
#include <drv_dcmi.h>
#include <lcd_port.h>
#include "background.h"
#include "Number.h"
#include "cvlib.h"

#define KEY_PIN GET_PIN(H, 4)

#define THREAD_PRIORITY         25
#define THREAD_TIMESLICE        5

#define RGB565_SIZE_WIDTH   320
#define RGB565_SIZE_HEIGHT  240

#define Gray_SIZE_WIDTH   320
#define Gray_SIZE_HEIGHT  240
//#define RGB565_SIZE  RGB565_SIZE_WIDTH*RGB565_SIZE_HEIGHT
//#define RGB565_BUF_SIZE  RGB565_SIZE*2
//#define RGB565_GRAY_SIZE  RGB565_SIZE

#define LCD_SIZE_WIDTH   800
#define LCD_SIZE_HEIGHT  480

#define LCD_SHOW_OFFET_WIDTH   20
#define LCD_SHOW_OFFET_HEIGHT  40

//#define ROI_S_X 15
//#define ROI_E_X 225
//#define ROI_S_Y 55
//#define ROI_E_Y 265

#define ROI_S_X 35
#define ROI_E_X 205
#define ROI_S_Y 75
#define ROI_E_Y 245

//#define CONNECT_COMPONENT_SIZE_WIDTH   240
//#define CONNECT_COMPONENT_SIZE_HEIGHT  320
//#define CONNECT_COMPONENT_BUF_SIZE  CONNECT_COMPONENT_SIZE_WIDTH*CONNECT_COMPONENT_SIZE_HEIGHT
//#define CONNECT_COMPONENT_STACK_SIZE 1024*200
#define BINARY_THRESH 10

struct Rgb565BufStruct rgb565_data_buf;
struct GrayBufStruct bin_data_buf;
struct GrayBufStruct roiGrayImg;
struct GrayBufStruct ccGrayImg;
struct GrayBufStruct ccNumImg;
uint8_t * ccStack;
/* 指向信号量的指针 */
static rt_sem_t cpd_sem = RT_NULL;//capture photo done semaphore
ALIGN(RT_ALIGN_SIZE)
static char thread1_stack[1024];
static struct rt_thread thread1;


static void lcd_background_init(struct drv_lcd_device *lcd)
{
    for(int i=0;i<LCD_SIZE_WIDTH*LCD_SIZE_HEIGHT;i++)
    {
        uint16_t * d = (uint16_t*)lcd->lcd_info.framebuffer;
        d[i] = background[i];
    }
}

static void lcd_show_number(struct drv_lcd_device *lcd,int showNum,int startX,int startY)
{
    for(int i=0;i<NUMBER_W;i++)
   {
       for(int j=0;j<NUMBER_W;j++)
       {
           uint16_t * d = (uint16_t*)lcd->lcd_info.framebuffer;
           d[LCD_SIZE_WIDTH*(i+startY) + j+startX] = NUMBER_BUF[showNum*2500+i*NUMBER_W+j];
       }
   }
}

static void lcd_show_2line(struct drv_lcd_device *lcd,int x,int y)
{
    uint16_t * d = (uint16_t*)lcd->lcd_info.framebuffer;
    for(int i=x-10;i<x+10;i++)
   {
        d[LCD_SIZE_WIDTH*(i+LCD_SHOW_OFFET_HEIGHT) + y+540] = 55555;
   }

    for(int j=y-10;j<y+10;j++)
   {
        d[LCD_SIZE_WIDTH*(x+LCD_SHOW_OFFET_HEIGHT) + j+540] = 55555;
   }

}

static void lcd_show_gray(struct drv_lcd_device *lcd, struct GrayBufStruct* bin_data_buf,int startX,int startY)
{
    for(int i=0;i<bin_data_buf->height;i++)
   {
       for(int j=0;j<bin_data_buf->width;j++)
       {
           uint16_t * d = (uint16_t*)lcd->lcd_info.framebuffer;

           d[LCD_SIZE_WIDTH*(i+startY) + j+startX] = gray8_to_gray16(bin_data_buf->data[i*bin_data_buf->width+j]);

       }
   }
}

static void lcd_show_rgb565(struct drv_lcd_device *lcd, struct Rgb565BufStruct* bin_data_buf,int startX,int startY)
{
    for(int i=0;i<bin_data_buf->height;i++)
   {
       for(int j=0;j<bin_data_buf->width;j++)
       {
           uint16_t * d = (uint16_t*)lcd->lcd_info.framebuffer;

           d[LCD_SIZE_WIDTH*(i+startY) + j+startX] = bin_data_buf->data[i*bin_data_buf->width+j];

       }
   }
}

static int rt_thread1_entry(void *parameter)
{
    rt_pin_mode(KEY_PIN, PIN_MODE_INPUT);
    int display=1;
    int xystate = 0;
    int allx = 0;
    int ally = 0;
    int times = 0;
    int realx=0;
    int realy=0;

    int fps=0;


    struct drv_lcd_device *lcd = (struct drv_lcd_device *)rt_device_find("lcd");
    lcd_background_init(lcd);
    if (RT_NULL == lcd)
    {
        rt_kprintf("find lcd  error!\n");
        return RT_ERROR;
    }
    if(rgb565_malloc(&rgb565_data_buf,RGB565_SIZE_WIDTH,RGB565_SIZE_HEIGHT)==RT_NULL)
    {
        rt_kprintf("rgb565_data_buf malloc error!\n");
        return RT_ERROR;
    }

    if(gray_malloc(&bin_data_buf,Gray_SIZE_HEIGHT,Gray_SIZE_WIDTH)==RT_NULL)
    {
        rt_kprintf("bin_data_buf malloc error!\n");
        return RT_ERROR;
    }
    if(gray_malloc(&roiGrayImg,ROI_E_X-ROI_S_X,ROI_E_Y-ROI_S_Y)==RT_NULL)
    {
        rt_kprintf("roiGrayImg malloc error!\n");
        return RT_ERROR;
    }

    if(gray_malloc(&ccGrayImg,ROI_E_X-ROI_S_X,ROI_E_Y-ROI_S_Y)==RT_NULL)
    {
       rt_kprintf("ccGrayImg malloc error!\n");
       return RT_ERROR;
    }

    ccStack = rt_malloc(STACK_SIZE);
    if(ccStack==RT_NULL)
    {
        rt_kprintf("ccStack malloc error!\n");
       return RT_ERROR;
    }

    while(1)
    {
        //显示控制按键
        if (rt_pin_read(KEY_PIN)==PIN_LOW) {
            display=~display;
            rt_thread_delay(100);
        }

        DCMI_Start((uint32_t*)rgb565_data_buf.data,(uint32_t)RGB565_SIZE_WIDTH*RGB565_SIZE_HEIGHT/2);
        rt_err_t result = rt_sem_take(cpd_sem, RT_WAITING_FOREVER);
        DCMI_Stop();
        if (result != RT_EOK)
        {
            rt_kprintf("take capture photo done semaphore failed.\n");
            rt_sem_delete(cpd_sem);
            return RT_ERROR;
        }
        else
        {
            uint16_t * p = rgb565_data_buf.data;
            uint16_t * d = (uint16_t*)lcd->lcd_info.framebuffer;

            for(int i=0;i<rgb565_data_buf.width;i++)//Y
            {
               for(int j=0;j<rgb565_data_buf.height;j++)//X
               {


                   uint16_t rgb565_pixel = p[(j+1)*rgb565_data_buf.width-i-1];
                   uint8_t gray_pixel_8 =rgb565_to_gray8(rgb565_pixel);
                   uint16_t gray_pixel_16 = gray8_to_gray16(gray_pixel_8);
                   uint8_t bin_pixel_8 =gray8_to_gray_bin(gray_pixel_8,BINARY_THRESH);
                   uint16_t bin_pixel_16 = gray8_to_gray16(bin_pixel_8);

                   if (display==1)
                   {
                       if((i == ROI_S_Y|| i == ROI_E_Y)&& j>ROI_S_X && j<ROI_E_X)
                       {
                           rgb565_pixel = 23456;
                           gray_pixel_16 = 65535;
                           bin_pixel_16 = 23456;
                       }
                       if((j == ROI_S_X|| j == ROI_E_X)&& i>ROI_S_Y && i<ROI_E_Y)
                       {
                           rgb565_pixel = 23456;
                           gray_pixel_16 = 65535;
                           bin_pixel_16 = 23456;
                       }
                       if((j == (ROI_S_X+ROI_E_X)/2)&& i>((ROI_S_Y+ROI_E_Y)/2)-10 && i<((ROI_S_Y+ROI_E_Y)/2)+10)
                       {
                           gray_pixel_16 = 23456;
                       }
                       if((i == (ROI_S_Y+ROI_E_Y)/2)&& j>((ROI_S_X+ROI_E_X)/2)-10 && j<((ROI_S_X+ROI_E_X)/2)+10)
                       {
                           gray_pixel_16 = 23456;
                       }

                       d[LCD_SIZE_WIDTH*(i+LCD_SHOW_OFFET_HEIGHT) + j+20] = rgb565_pixel;
                       d[LCD_SIZE_WIDTH*(i+LCD_SHOW_OFFET_HEIGHT) + j+280] = gray_pixel_16;

                   }

                   d[LCD_SIZE_WIDTH*(i+LCD_SHOW_OFFET_HEIGHT) + j+540] = bin_pixel_16;//必须执行


               }
            }//for1end


            //方向1
            for(int i=ROI_S_Y;i<ROI_E_Y;i++)//Y
            {
               for(int j=ROI_S_X;j<ROI_E_X;j++)//X
               {
                   if ((d[LCD_SIZE_WIDTH*(i+LCD_SHOW_OFFET_HEIGHT) + j+540-1]==23456||
                       d[LCD_SIZE_WIDTH*(i-1+LCD_SHOW_OFFET_HEIGHT) + j+540]==23456)&&
                       d[LCD_SIZE_WIDTH*(i+LCD_SHOW_OFFET_HEIGHT) + j+540]==0    )
                   {
                       d[LCD_SIZE_WIDTH*(i+LCD_SHOW_OFFET_HEIGHT) + j+540]=23456;
                   }
               }

            }

            //方向2
            for(int i=ROI_S_Y;i<ROI_E_Y;i++)//Y
            {
               for(int j=ROI_E_X;j>ROI_S_X;j--)//X
               {
                   if ((d[LCD_SIZE_WIDTH*(i+LCD_SHOW_OFFET_HEIGHT) + j+540+1]==23456||
                        d[LCD_SIZE_WIDTH*(i-1+LCD_SHOW_OFFET_HEIGHT) + j+540]==23456)&&
                        d[LCD_SIZE_WIDTH*(i+LCD_SHOW_OFFET_HEIGHT) + j+540]==0)
                   {
                       d[LCD_SIZE_WIDTH*(i+LCD_SHOW_OFFET_HEIGHT) + j+540]=23456;
                   }
               }

            }

            //方向3
            for(int i=ROI_E_Y;i>ROI_S_Y;i--)//Y
            {
               for(int j=ROI_S_X;j<ROI_E_X;j++)//X
               {
                   if ((d[LCD_SIZE_WIDTH*(i+LCD_SHOW_OFFET_HEIGHT) + j+540-1]==23456||
                        d[LCD_SIZE_WIDTH*(i+1+LCD_SHOW_OFFET_HEIGHT) + j+540]==23456)&&
                        d[LCD_SIZE_WIDTH*(i+LCD_SHOW_OFFET_HEIGHT) + j+540]==0)
                   {
                       d[LCD_SIZE_WIDTH*(i+LCD_SHOW_OFFET_HEIGHT) + j+540]=23456;
                   }
               }

            }

            //方向4
            for(int i=ROI_E_Y;i>ROI_S_Y;i--)//Y
            {
               for(int j=ROI_E_X;j>ROI_S_X;j--)//X
               {
                   if ((d[LCD_SIZE_WIDTH*(i+LCD_SHOW_OFFET_HEIGHT) + j+540+1]==23456||
                        d[LCD_SIZE_WIDTH*(i+1+LCD_SHOW_OFFET_HEIGHT) + j+540]==23456)&&
                        d[LCD_SIZE_WIDTH*(i+LCD_SHOW_OFFET_HEIGHT) + j+540]==0)
                   {
                       d[LCD_SIZE_WIDTH*(i+LCD_SHOW_OFFET_HEIGHT) + j+540]=23456;
                   }
               }

            }

            //遍历黑色
            for(int i=ROI_S_Y;i<ROI_E_Y;i++)//Y
            {
               for(int j=ROI_S_X;j<ROI_E_X;j++)//X
               {
                   if ((i>ROI_S_Y && i<ROI_E_Y)&&(j>ROI_S_X && j<ROI_E_X))
                   {
                       if (d[LCD_SIZE_WIDTH*(i+LCD_SHOW_OFFET_HEIGHT) + j+540]==0)
                       {
                           allx+=i;
                           ally+=j;
                           times++;
                       }
                   }
               }
            }


            //过小目标排除
            if (times>30)
            {
                realx=allx/times;
                realy=ally/times;
                xystate = 1;
            }
            else
            {
                xystate = 0;
            }

            //输出
            if (xystate==0) {
                if (display==1) {
                    lcd_show_number(lcd,xystate,280,390);
                }

            }
            else
            {
                rt_kprintf("X%03dY%03dE\n",realx,realy);

//                rt_kprintf("%d\n",fps);
//                fps++;//帧数测速

                if (display==1) {
                    lcd_show_number(lcd,xystate,280,390);
                    lcd_show_2line(lcd,realx,realy);
                }

            }

            allx=0;ally=0;times=0;

            if (display==1) {
                lcd->parent.control(&lcd->parent, RTGRAPHIC_CTRL_RECT_UPDATE, RT_NULL);
            }


        }



    }
    return RT_EOK;
}


/* After a frame of picture data has been collected. */
void camera_frame_data_process(void)
{
    rt_sem_release(cpd_sem);
}


/* 信号量示例的初始化 */
int number_recognition_sample(void)
{
    /* 创建一个动态信号量，初始值是 0 */
    cpd_sem = rt_sem_create("cpdSem", 0, RT_IPC_FLAG_FIFO);
    if (cpd_sem == RT_NULL)
    {
        rt_kprintf("capture photo done semaphore failed.\n");
        return -1;
    }
    else
    {
        rt_kprintf("create done. capture photo done semaphore value = 0.\n");
    }

    rt_thread_init(&thread1,
                   "thread1",
                   rt_thread1_entry,
                   RT_NULL,
                   &thread1_stack[0],
                   sizeof(thread1_stack),
                   THREAD_PRIORITY, THREAD_TIMESLICE);
    rt_thread_startup(&thread1);

    return 0;
}
/* 导出到 msh 命令列表中 */
//MSH_CMD_EXPORT(number_recognition_sample, camera lcd sample);
