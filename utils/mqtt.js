const Paho = require('../paho-mqtt');

let client = null;
let onMessageCallback = null;
let reconnectTimer = null;

/**
 * 解析MQTT连接URL（适配微信小程序wxs协议）
 * @param {string} brokerUrl - 完整连接地址（如 wxs://broker.emqx.io:8084/mqtt）
 */
function parseBrokerUrl(brokerUrl) {
  const protocolMatch = brokerUrl.match(/^(wxs?):\/\//i);
  const protocol = protocolMatch ? protocolMatch[1].toLowerCase() : 'wxs';
  
  const noProtocol = brokerUrl.replace(/^\w+:\/\//, '');
  const [hostPart, ...pathParts] = noProtocol.split('/');
  const [host, portStr] = hostPart.split(':');
  
  // 端口处理：完全保留URL中的端口号
  const port = portStr ? parseInt(portStr) : (protocol === 'wxs' ? 8084 : 8084);
  const path = '/' + (pathParts.join('/') || 'mqtt'); // EMQX要求路径含/mqtt
  
  return { protocol, host, port, path };
}

/**
 * 指数退避重连策略
 * @param {Function} connectFn - 连接函数
 * @param {number} maxAttempts - 最大重试次数
 */
function exponentialBackoff(connectFn, maxAttempts = 5) {
  clearTimeout(reconnectTimer);
  let attempts = 0;
  
  const tryReconnect = () => {
    connectFn().catch(() => {
      attempts++;
      if (attempts < maxAttempts) {
        const delay = Math.min(1000 * Math.pow(2, attempts), 30000);
        reconnectTimer = setTimeout(tryReconnect, delay);
      }
    });
  };
  tryReconnect();
}

function connect(broker, options) {
  return new Promise((resolve, reject) => {
    try {
      const { host, port, path } = parseBrokerUrl(broker);
      client = new Paho.Client(host, port, path, options.clientId);
      
      // 核心修复：移除loopStart调用
      client.onConnectionLost = (response) => {
        console.error(`连接丢失: ${response.errorMessage}`);
        if (options.reconnect) exponentialBackoff(() => connect(broker, options));
      };
      
      client.onMessageArrived = (message) => {
        onMessageCallback?.(message.destinationName, message.payloadString);
      };
      
      const connectOptions = {
        timeout: options.connectTimeout || 5,
        userName: options.username || '',
        password: options.password || '',
        useSSL: broker.startsWith('wxs') || broker.startsWith('mqtts'),
        keepAliveInterval: options.keepalive || 30,
        // 启用Paho内置重连机制[1,4](@ref)
        reconnect: true,
        onSuccess: () => {
          console.log(`MQTT连接成功: ${host}:${port}${path}`);
          resolve(client);
        },
        onFailure: (err) => {
          const enhancedErr = {
            errorCode: err.errorCode || 7,
            errorMessage: err.errorMessage || `网络错误: ${err}`
          };
          reject(enhancedErr);
        }
      };
      
      client.connect(connectOptions);
    } catch (e) {
      reject({ errorCode: 8, errorMessage: `URL解析失败: ${broker}` });
    }
  });
}

function disconnect() {
  clearTimeout(reconnectTimer);
  if (client?.isConnected()) {
    client.disconnect();
    client = null;
  }
}

function setOnMessage(callback) {
  onMessageCallback = callback;
}

module.exports = {
  connect,
  disconnect,
  setOnMessage,
  getClient: () => client
};