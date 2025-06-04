#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>
#include <board.h>
#include <msh.h>

#include <wlan_mgnt.h>  // Wi-Fi 管理接口
#include <wlan_prot.h>  // Wi-Fi 协议结构体定义
#include <wlan_cfg.h>   // Wi-Fi 配置接口
#include <webclient.h>  //http请求
#include <stdio.h>
#include <stdlib.h>
#include <cJSON.h>
#include <time.h>
#include <ntp.h>

#include <drv_lcd.h>
#include <rtdbg.h>
#include <board.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define WLAN_SSID "666"             // 要连接的 WiFi 热点名称
#define WLAN_PASSWORD "12345678"   // WiFi 密码
#define NET_READY_TIME_OUT (rt_tick_from_millisecond(15 * 1000)) // 等待获取 IP 的超时时间
#define TIME_API_URL "http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp"    //获取时间的URL


static void print_wlan_information(struct rt_wlan_info *info,int index);
static int wifi_autoconnect(void);

static struct rt_semaphore net_ready;   // 信号量: 用于网络准备就绪
static struct rt_semaphore scan_done;   // 信号量: 用于WiFi扫描完成标志

struct rt_wlan_info info; //wifi信息

#define THREAD_PRIORITY         1      // 线程优先级
#define THREAD_STACK_SIZE       2048     // 线程堆栈大小
#define THREAD_TIMESLICE        5       // 线程时间片

static rt_thread_t tid_wifi = RT_NULL;


// 扫描结果回调函数：每发现一个热点调用一次
void wlan_scan_report_hander(int event,struct rt_wlan_buff *buff,void *parameter)
{
    struct rt_wlan_info *info = RT_NULL;
    int index = 0;
    RT_ASSERT(event == RT_WLAN_EVT_SCAN_REPORT);
    RT_ASSERT(buff != RT_NULL);
    RT_ASSERT(parameter != RT_NULL);

    info = (struct rt_wlan_info *)buff->data; // 获取热点信息结构体
    index = *((int *)(parameter)); // 获取热点索引
    print_wlan_information(info,index); // 打印热点信息
    ++ *((int *)(parameter)); // index 自增
}

// 扫描完成事件回调：释放 scan_done 信号量
void wlan_scan_done_hander(int event,struct rt_wlan_buff *buff,void *parameter)
{
    RT_ASSERT(event == RT_WLAN_EVT_SCAN_DONE);
    rt_sem_release(&scan_done); // 通知扫描完成
}

// 成功连接热点并获取到 IP 后回调，释放 net_ready 信号量
void wlan_ready_handler(int event, struct rt_wlan_buff *buff, void *parameter)
{
    rt_sem_release(&net_ready);
}

// 热点断开连接时触发的回调函数
void wlan_station_disconnect_handler(int event, struct rt_wlan_buff *buff, void *parameter)
{
    LOG_I("disconnect from the network!");
}

// 连接成功的回调函数（触发自动重连时使用）
static void wlan_connect_handler(int event, struct rt_wlan_buff *buff, void *parameter)
{
    rt_kprintf("%s\n", __FUNCTION__);
    if ((buff != RT_NULL) && (buff->len == sizeof(struct rt_wlan_info)))
    {
        rt_kprintf("ssid : %s \n", ((struct rt_wlan_info *)buff->data)->ssid.val);
    }
}

// 连接失败的回调函数（触发自动重连时使用）
static void wlan_connect_fail_handler(int event, struct rt_wlan_buff *buff, void *parameter)
{
    rt_kprintf("%s\n", __FUNCTION__);
    if ((buff != RT_NULL) && (buff->len == sizeof(struct rt_wlan_info)))
    {
        rt_kprintf("ssid : %s \n", ((struct rt_wlan_info *)buff->data)->ssid.val);
    }
}

static void update_rtc_with_ntp_time(void)
{
        time_t now;
        struct tm tm_now;

        /* 从默认NTP服务器获取UTC时间 */
        now = ntp_get_time(NULL);

        if (now == 0)
        {
            rt_kprintf("NTP get time failed!\n");
            return;
        }

        /* 将UTC时间转换为结构化时间 */
        gmtime_r(&now, &tm_now);

        rt_kprintf("NTP time: %04d-%02d-%02d %02d:%02d:%02d UTC\n",
                   tm_now.tm_year + 1900,
                   tm_now.tm_mon + 1,
                   tm_now.tm_mday,
                   tm_now.tm_hour,
                   tm_now.tm_min,
                   tm_now.tm_sec);

        /* 设置RTC日期和时间 */
        set_date(tm_now.tm_year + 1900, tm_now.tm_mon + 1, tm_now.tm_mday);
        set_time(tm_now.tm_hour + 8, tm_now.tm_min, tm_now.tm_sec);
}



static void Wifi_Init(void *parameter)
{
    static int i = 0;
    int result = RT_EOK;

    // 等待WiFi初始化完成
    rt_thread_mdelay(500); // 延时500ms，等待 RW007 模块初始化完成

    // 步骤1: 扫描周围热点
    LOG_D("start to scan ap ...");
    rt_sem_init(&scan_done,"scan_done",0,RT_IPC_FLAG_FIFO); // 初始化扫描完成信号量
    rt_wlan_register_event_handler(RT_WLAN_EVT_SCAN_REPORT, wlan_scan_report_hander,&i); // 注册热点报告事件回调
    rt_wlan_register_event_handler(RT_WLAN_EVT_SCAN_DONE, wlan_scan_done_hander,RT_NULL); // 注册扫描完成事件回调

    if(rt_wlan_scan() == RT_EOK) // 启动热点扫描
    {
        LOG_D("the scan is started... ");
    }else
    {
        LOG_E("scan failed");
    }

    rt_sem_take(&scan_done,RT_WAITING_FOREVER); // 等待扫描完成

    // 步骤2: 连接指定热点
    LOG_D("start to connect ap ...");
    rt_sem_init(&net_ready, "net_ready", 0, RT_IPC_FLAG_FIFO); // 初始化连接完成信号量
    rt_wlan_register_event_handler(RT_WLAN_EVT_READY, wlan_ready_handler, RT_NULL); // 注册获取IP完成回调
    rt_wlan_register_event_handler(RT_WLAN_EVT_STA_DISCONNECTED, wlan_station_disconnect_handler, RT_NULL); // 注册断开事件

    result = rt_wlan_connect(WLAN_SSID, WLAN_PASSWORD); // 同步连接热点
    if (result == RT_EOK)
    {
        rt_memset(&info, 0, sizeof(struct rt_wlan_info));
        rt_wlan_get_info(&info); // 获取当前连接的热点信息
        LOG_D("station information:");
        print_wlan_information(&info,0);


        result = rt_sem_take(&net_ready, NET_READY_TIME_OUT); // 等待获取IP（通过信号量判断）
        if (result == RT_EOK)
        {
            LOG_D("networking ready!");
            msh_exec("ifconfig", rt_strlen("ifconfig")); // 打印 IP 地址等信息
        }
        else
        {
            LOG_D("wait ip got timeout!");
        }
        rt_wlan_unregister_event_handler(RT_WLAN_EVT_READY); // 注销事件
        rt_sem_detach(&net_ready); // 释放信号量资源
    }
    else
    {
        LOG_E("The AP(%s) is connect failed!", WLAN_SSID);
    }

    update_rtc_with_ntp_time();

    return ;
}

// 打印单个热点信息
static void print_wlan_information(struct rt_wlan_info *info,int index)
{
    char *security;
    if(index == 0)
    {
        rt_kprintf("             SSID                      MAC            security    rssi chn Mbps\n");
        rt_kprintf("------------------------------- -----------------  -------------- ---- --- ----\n");
    }
    rt_kprintf("%-32.32s", &(info->ssid.val[0]));
    rt_kprintf("%02x:%02x:%02x:%02x:%02x:%02x  ",
            info->bssid[0], info->bssid[1], info->bssid[2],
            info->bssid[3], info->bssid[4], info->bssid[5]);

    switch (info->security)
    {
    case SECURITY_OPEN: security = "OPEN"; break;
    case SECURITY_WEP_PSK: security = "WEP_PSK"; break;
    case SECURITY_WEP_SHARED: security = "WEP_SHARED"; break;
    case SECURITY_WPA_TKIP_PSK: security = "WPA_TKIP_PSK"; break;
    case SECURITY_WPA_AES_PSK: security = "WPA_AES_PSK"; break;
    case SECURITY_WPA2_AES_PSK: security = "WPA2_AES_PSK"; break;
    case SECURITY_WPA2_TKIP_PSK: security = "WPA2_TKIP_PSK"; break;
    case SECURITY_WPA2_MIXED_PSK: security = "WPA2_MIXED_PSK"; break;
    case SECURITY_WPS_OPEN: security = "WPS_OPEN"; break;
    case SECURITY_WPS_SECURE: security = "WPS_SECURE"; break;
    default: security = "UNKNOWN"; break;
    }

    rt_kprintf("%-14.14s %-4d %3d %4d\n", security, info->rssi, info->channel, info->datarate / 1000000);
}

// 配置并启动自动连接机制
static int wifi_autoconnect(void)
{
    rt_wlan_set_mode(RT_WLAN_DEVICE_STA_NAME, RT_WLAN_STATION); // 设置工作模式为 STA（客户端）
    rt_wlan_config_autoreconnect(RT_TRUE);                      // 启用自动重连功能
    rt_wlan_register_event_handler(RT_WLAN_EVT_STA_CONNECTED, wlan_connect_handler, RT_NULL); // 注册连接成功回调
    rt_wlan_register_event_handler(RT_WLAN_EVT_STA_CONNECTED_FAIL, wlan_connect_fail_handler, RT_NULL); // 注册连接失败回调
    return 0;
}

int thread_wifi(void)
{

    tid_wifi = rt_thread_create("wifi",
                            Wifi_Init, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (tid_wifi != RT_NULL)
        rt_thread_startup(tid_wifi);
    return 0;
}

INIT_APP_EXPORT(thread_wifi);

