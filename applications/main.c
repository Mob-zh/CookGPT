/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-5-10      ShiHao       first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>


#define DBG_TAG "main"
#define DBG_LVL         DBG_LOG
#include <rtdbg.h>

#define RTC_NAME       "rtc"

void set_system_time(int16_t year_1,int16_t mon_1,int16_t day_1,int16_t hour_1,int16_t min_1,int16_t sec_1)
{
    struct tm tm_new = {0};

    tm_new.tm_year = year_1 - 1900;  // 年份从 1900 开始
    tm_new.tm_mon  = mon_1 - 1;        // 月份从 0 开始（5月是4）
    tm_new.tm_mday = day_1;
    tm_new.tm_hour = hour_1;
    tm_new.tm_min  = min_1;
    tm_new.tm_sec  = sec_1;

    time_t t = mktime(&tm_new);   // 转换成 time_t 类型

    stime(&t);  // 设置系统时间，将其同步到 RTC
}

//rtc时钟
int rtc_init()
{
    rt_err_t ret = RT_EOK;
    time_t now;
    rt_device_t device = RT_NULL;

    /* 寻找设备 */
    device = rt_device_find(RTC_NAME);
    if (!device)
    {
        rt_kprintf("find %s failed!", RTC_NAME);
        return RT_ERROR;
    }

    /* 初始化RTC设备 */
    if(rt_device_open(device, 0) != RT_EOK)
    {
        rt_kprintf("open %s failed!", RTC_NAME);
        return RT_ERROR;
    }

    /*设置时间，不准的时候调
    ret = set_date(2025, 5, 28);
        if (ret != RT_EOK)
        {
        rt_kprintf("set RTC date failed\n");
        return ret;
        }
        ret = set_time(13, 28, 20);
        if (ret != RT_EOK)
        {
        rt_kprintf("set RTC time failed\n");
        return ret;
      }


    /* 获取时间 */
    now = time(RT_NULL);
    rt_kprintf("%s\n", ctime(&now));

    /* 延时1秒 */
    rt_thread_mdelay(1000);

    /* 获取时间 */
    now = time(RT_NULL);
    rt_kprintf("%s\n", ctime(&now));

    return ret;
}

int main(void)
{
    rtc_init();

    return 0;
}



