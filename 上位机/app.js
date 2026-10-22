// app.js
App({
  onLaunch() {
    const logs = wx.getStorageSync('logs') || [];
    logs.unshift(Date.now());
    wx.setStorageSync('logs', logs);

    wx.login({
      success: () => {}
    });
  },
  globalData: {
    userInfo: null,
    client: null,
    host: 'broker.emqx.io:8084',
    username: '',
    password: '',
    topic: '32fdH21323J7328HJ/002112011SWP/121211',
    subTopic: '32fdH21323J7328HJ/002112011SWP/121211',
    pubTopic: '32fdH21323J7328HJ/002112011SWP/121211',
    pubMsg: 'Hello World',
    receivedMsg: '',
    currMsg: '',
    mqttStatus: 0,
    reconnectPeriod: 1000,
    connectTimeout: 30 * 1000
  }
});
