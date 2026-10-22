// pages/index/index.js
const mqtt = require('../../utils/mqtt.min.js');

const app = getApp();
const { username, password, reconnectPeriod, connectTimeout, host, subTopic, pubTopic } = app.globalData;

const ALERT_INTERVAL = 10000;

const unitMap = {
  gatest: 'F/O',
  mq2: '%',
  sr602: 'SED',
  flame: 'Y/N'
};

const nameMap = {
  gatest: '门禁状态',
  mq2: '烟雾浓度',
  sr602: '人体红外',
  flame: '火焰检测',
  mqmax: '烟雾阈值',
  wifi_flag: 'WiFi状态',
  mqtt_flag: 'MQTT状态'
};

function getSensorValue(data, key) {
  const item = data[key] || data[key.toLowerCase()] || data[key.toUpperCase()];
  if (item && typeof item === 'object' && item.val !== undefined) {
    return Number(item.val);
  }
  return Number(item);
}

function payloadToString(payload) {
  if (typeof payload === 'string') {
    return payload;
  }
  if (payload && payload.toString) {
    return payload.toString();
  }
  return '';
}

Page({
  data: {
    displayData: [],
    mqttStatusText: '连接中',
    lastMessageTime: '',
    activeAlerts: []
  },

  lastAlertAt: {},

  slider3(e) {
    const value = e.detail.value;
    const pubMsg = `TODEVICEDATA##MQMax##${value}##`;
    if (app.globalData.client && app.globalData.mqttStatus === 1) {
      app.globalData.client.publish(pubTopic, pubMsg);
      wx.showToast({ title: '阈值已发送', icon: 'success' });
      return;
    }

    wx.showToast({ title: 'MQTT未连接', icon: 'none' });
  },

  onLoad() {
    this.connectMqtt();
  },

  onUnload() {
    if (app.globalData.client) {
      app.globalData.client.end(true);
      app.globalData.client = null;
      app.globalData.mqttStatus = 0;
    }
  },

  connectMqtt() {
    try {
      const clientId = `wx_${Date.now()}_${Math.floor(Math.random() * 1000)}`;
      app.globalData.client = mqtt.connect(`wxs://${host}/mqtt`, {
        username,
        password,
        reconnectPeriod,
        connectTimeout,
        clientId,
        clean: true
      });
    } catch (error) {
      console.log('mqtt.connect error', error);
      this.setData({ mqttStatusText: '连接失败' });
      return;
    }

    const client = app.globalData.client;
    if (!client) {
      return;
    }

    client.on('connect', () => {
      app.globalData.mqttStatus = 1;
      this.setData({ mqttStatusText: '已连接' });
      client.subscribe(subTopic, { qos: 0 }, (error) => {
        if (error) {
          console.log('subscribe error', error);
          this.setData({ mqttStatusText: '订阅失败' });
        }
      });
    });

    client.on('reconnect', () => {
      this.setData({ mqttStatusText: '重连中' });
    });

    client.on('close', () => {
      app.globalData.mqttStatus = 0;
      this.setData({ mqttStatusText: '已断开' });
    });

    client.on('error', (error) => {
      console.log('mqtt error', error);
      this.setData({ mqttStatusText: '连接异常' });
    });

    client.on('message', (topic, payload) => {
      const message = payloadToString(payload);
      console.log(`收到消息 - Topic: ${topic}, Payload: ${message}`);
      this.handleDeviceMessage(message);
    });
  },

  handleDeviceMessage(message) {
    if (message.indexOf('TODEVICEDATA##') === 0) {
      return;
    }

    try {
      const data = JSON.parse(message);
      const updatedData = Object.keys(data).map((key) => {
        const lowerKey = key.toLowerCase();
        const item = data[key];
        return {
          label: nameMap[lowerKey] || key,
          value: item && typeof item === 'object' ? item.val : item,
          unit: item && item.unit !== undefined ? item.unit : unitMap[lowerKey] || ''
        };
      });

      const activeAlerts = this.getActiveAlerts(data);
      app.globalData.receivedMsg = message;
      app.globalData.currMsg = message;

      this.setData({
        displayData: updatedData,
        activeAlerts,
        lastMessageTime: this.formatTime(new Date())
      });

      activeAlerts.forEach((alert) => this.notifyAlert(alert));
    } catch (error) {
      console.log('JSON解析错误', error, message);
    }
  },

  getActiveAlerts(data) {
    const mq2 = getSensorValue(data, 'MQ2');
    const mqMax = getSensorValue(data, 'MQMax');
    const flame = getSensorValue(data, 'Flame');
    const sr602 = getSensorValue(data, 'SR602');
    const gateST = getSensorValue(data, 'GateST');
    const alerts = [];
    const fireLimit = Number.isFinite(mqMax) ? mqMax : 80;

    if (mq2 > fireLimit && flame === 1) {
      alerts.push({ type: 'fire', title: '火情警报', content: `检测到火焰，烟雾浓度 ${mq2}%` });
    }

    if (sr602 === 1 && gateST === 0) {
      alerts.push({ type: 'intrusion', title: '入侵警报', content: '门禁关闭状态下检测到人体活动' });
    }

    return alerts;
  },

  notifyAlert(alert) {
    const now = Date.now();
    if (now - (this.lastAlertAt[alert.type] || 0) < ALERT_INTERVAL) {
      return;
    }

    this.lastAlertAt[alert.type] = now;
    wx.vibrateLong({});
    wx.showModal({
      title: alert.title,
      content: alert.content,
      showCancel: false,
      confirmText: '知道了'
    });
  },

  formatTime(date) {
    const pad = (num) => String(num).padStart(2, '0');
    return `${pad(date.getHours())}:${pad(date.getMinutes())}:${pad(date.getSeconds())}`;
  },

  onReady() {},
  onShow() {},
  onHide() {},
  onPullDownRefresh() {},
  onReachBottom() {},
  onShareAppMessage() {}
});
