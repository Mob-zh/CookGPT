#include <stdio.h>
#include <drv_lcd.h>
#include "screen.h"

#define THREAD_PRIORITY         20      // 线程优先级
#define THREAD_STACK_SIZE       1024     // 线程堆栈大小
#define THREAD_TIMESLICE        5       // 线程时间片

static rt_thread_t tid_screen = RT_NULL;

extern float temperature;
extern float humidity;    // 温湿度全局变量
extern struct rt_wlan_info info; // wifiID

static void screen_entry(void *parameter)
{

    lcd_clear(WHITE);
    char show_str[16] = {0};
    while(1){
        //title
        lcd_set_color(WHITE, BLUE);
        lcd_show_string(54, 10, 32, "Cook-GPT");

        lcd_set_color(WHITE, BLACK);
        //传感器数据
        sprintf(show_str,"temp:%.2f C",temperature);
        lcd_show_string(10, 10+40   , 24, show_str);

        sprintf(show_str,"wet:%.2f %%",humidity);
        lcd_show_string(10, 10+40+24, 24, show_str);

        //rtc时钟
        time_t now;
        now = time(RT_NULL);
        sprintf(show_str,"%s", ctime(&now));
        lcd_show_string(10, 10+40+24+24, 16, show_str);

        // wifiID
        lcd_show_string(10, 200, 24, "Wifi:");
        lcd_show_string(72, 200, 24, (const char *)info.ssid.val);

    }


}

/* 线程示例 */
int thread_screen(void)
{

    tid_screen = rt_thread_create("screen",
                            screen_entry, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (tid_screen != RT_NULL)
        rt_thread_startup(tid_screen);
    return 0;
}

INIT_APP_EXPORT(thread_screen);


