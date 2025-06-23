const mqtt = require('../../utils/mqtt.js');

Page({
  data: {
    // 传感器数据
    temperature: 0,
    humidity: 0,
    fireDetected: false,
    smokeLevel: 0,        // 新增烟雾等级（1-5级）
    // 系统状态
    updateTime: '未更新',
    statusText: '初始化中...',
    alarmStatus: 'normal',
    connectionStatus: '未连接',
    // 警报阈值
    tempThreshold: 50,
    humiThreshold: 80,
    smokeThreshold: 3,    // 新增烟雾阈值（默认3级）
    // MQTT配置
    mqttConfig: {
      broker: 'wxs://broker.emqx.io:8084/mqtt',
      options: {
        clientId: '',
        username: '',
        password: '',
        keepalive: 15,     // 缩短心跳间隔至15秒
        connectTimeout: 5, 
        reconnect: true,
        reconnectPeriod: 2000 // 新增：重连间隔2秒
      }
    }
  },

  onLoad() {
    // 持久化ClientID（避免页面刷新后ID变更导致连接冲突）
    let clientId = wx.getStorageSync('mqttClientId') || `cookgpt_${Date.now()}_${Math.floor(Math.random()*1000)}`;
    wx.setStorageSync('mqttClientId', clientId);
    this.data.mqttConfig.options.clientId = clientId;

    mqtt.setOnMessage(this.handleMqttMessage.bind(this));
    this.initMQTT();
    
    // 监听网络变化（增强网络感知）
    wx.onNetworkStatusChange(res => {
      if (!res.isConnected) {
        this.setData({ 
          statusText: '网络已断开',
          connectionStatus: '失败'
        });
        // 网络恢复后自动重连
        const networkCheck = setInterval(() => {
          wx.getNetworkType({
            success: res => {
              if (res.networkType !== 'none') {
                clearInterval(networkCheck);
                this.initMQTT();
              }
            }
          })
        }, 3000);
      }
    });
  },

  initMQTT() {
    const { broker, options } = this.data.mqttConfig;
    this.setData({ statusText: '连接中...', connectionStatus: '连接中' });

    mqtt.disconnect(); // 清除旧连接
    
    mqtt.connect(broker, options)
      .then(client => {
        this.setData({ statusText: '已连接', connectionStatus: '已连接' });
        
        // 批量订阅所有传感器主题
        const topics = [
          '/CookGPT/pub/temp',
          '/CookGPT/pub/wet',
          '/CookGPT/pub/fire',
          '/CookGPT/pub/smoke'
        ];
        
        // 延迟订阅确保协议握手完成
        setTimeout(() => {
          topics.forEach(topic => {
            client.subscribe(topic, { qos: 1 }); // QoS1保证至少一次送达
          });
          console.log('所有传感器订阅成功');
        }, 500);
      })
      .catch(err => {
        // 增强错误分类处理
        const errorMap = {
          1: "连接超时", 
          3: "服务器不可用", 
          7: "网络错误", 
          8: "地址无效",
          14: "证书错误"
        };
        const errorMsg = errorMap[err.errorCode] || `错误码: ${err.errorCode}`;
        this.setData({ 
          statusText: `连接失败: ${errorMsg}`, 
          connectionStatus: '失败',
          alarmStatus: 'danger'
        });
        
        // 指数退避重连策略
        const delay = Math.min(5000 * Math.pow(1.5, this.reconnectCount || 1), 30000);
        this.reconnectCount = (this.reconnectCount || 0) + 1;
        setTimeout(() => this.initMQTT(), delay);
      });
  },

  // 处理传感器数据流
  handleMqttMessage(topic, message) {
    const now = new Date();
    const timeStr = `${now.getHours().toString().padStart(2, '0')}:${now.getMinutes().toString().padStart(2, '0')}`;
    let updateData = {};

    switch(topic) {
      case '/CookGPT/pub/temp':
        updateData.temperature = parseFloat(message) || 0;
        break;
      case '/CookGPT/pub/wet':
        updateData.humidity = parseFloat(message) || 0;
        break;
      case '/CookGPT/pub/fire':
        // 兼容布尔值/数值型数据
        updateData.fireDetected = (message === '1' || message === 'true');
        break;
      case '/CookGPT/pub/smoke':
        // 烟雾数据标准化处理（0-100% → 1-5级）
        const rawValue = parseFloat(message) || 0;
        updateData.smokeLevel = Math.min(5, Math.max(1, Math.ceil(rawValue / 20)));
        break;
    }

    if (Object.keys(updateData).length > 0) {
      this.setData({ 
        ...updateData, 
        updateTime: timeStr,
        smokeWarning: updateData.smokeLevel >= this.data.smokeThreshold // 实时计算烟雾警报
      });
      this.checkAlarmStatus(); // 触发警报检测
    }
  },

  // 统一检测警报状态（多级预警）
  checkAlarmStatus() {
    const { 
      temperature, 
      humidity, 
      fireDetected, 
      smokeLevel,
      tempThreshold, 
      humiThreshold,
      smokeThreshold 
    } = this.data;
    
    // 警报级别计算
    const fireAlarm = fireDetected || smokeLevel >= 5; // 5级烟雾视为火灾
    const criticalSmoke = smokeLevel >= smokeThreshold && smokeLevel < 5;
    const tempWarn = temperature > tempThreshold;
    const humiWarn = humidity > humiThreshold;

    let status = 'normal';
    let statusText = '一切正常';

    // 警报优先级：火灾 > 烟雾 > 温湿度异常
    if (fireAlarm) {
      status = 'danger';
      statusText = '火灾警报!';
      wx.vibrateLong();  // 持续震动
      wx.showModal({ 
        title: '紧急警报', 
        content: '检测到火情，请立即撤离！', 
        showCancel: false,
        confirmText: '我知道了'
      });
    } else if (criticalSmoke) {
      status = 'danger';
      statusText = '高浓度烟雾!';
      wx.vibrateShort(); // 短震动
      wx.showToast({
        title: '检测到烟雾!',
        icon: 'none',
        duration: 3000
      });
    } else if (tempWarn || humiWarn) {
      status = 'warning';
      statusText = '环境异常';
    }

    this.setData({ 
      alarmStatus: status, 
      statusText,
      fireWarning: fireAlarm,       // 新增火灾警报状态
      tempWarning: tempWarn,        // 温度警报状态
      humiWarning: humiWarn         // 湿度警报状态
    });
  },

  // 新增：紧急处置按钮
  handleEmergency() {
    const client = mqtt.getClient();
    if (client && client.isConnected()) {
      // 发送紧急指令到水泵控制器
      const message = new Paho.Message("1");
      message.destinationName = "/CookGPT/cmd/pump";
      client.send(message);
      
      wx.showToast({ title: '已启动灭火系统', icon: 'success' });
    }
  },

  onUnload() {
    clearInterval(this.reconnectTimer);
    mqtt.disconnect();
  }
});