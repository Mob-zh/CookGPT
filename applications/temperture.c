#include <stdio.h>
#include <aht10.h>

#define THREAD_PRIORITY         20      // 线程优先级
#define THREAD_STACK_SIZE       1024     // 线程堆栈大小
#define THREAD_TIMESLICE        5       // 线程时间片



float temperature;
float humidity;    // 温湿度全局变量


static rt_thread_t tid2 = RT_NULL;


static void get_temp(void *parameter)
{


    aht10_device_t dev;
    const char *i2c_bus_name = "i2c3";
    int count = 0;
    /* 等 待 传 感 器 正 常 工 作 */
    rt_thread_mdelay(2000);
    /* 初 始 化 aht10 */
    dev = aht10_init(i2c_bus_name);
    if (dev == RT_NULL)
    {
        rt_kprintf("The sensor initializes failure");
        return ;
    }
    while (count++ < 100)
    {
    /* 读 取 湿 度 */
    humidity = aht10_read_humidity(dev);
    /* 读 取 温 度 */
    temperature = aht10_read_temperature(dev);
    rt_thread_mdelay(2000);
    }
        return ;
}

/* 线程示例 */
int thread_gettemp(void)
{

    tid2 = rt_thread_create("temp",
                            get_temp, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (tid2 != RT_NULL)
        rt_thread_startup(tid2);
    return 0;
}
INIT_APP_EXPORT(thread_gettemp);


