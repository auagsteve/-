/*
HC05模块连接方式：
蓝牙模块    开发板
RXD         TX
TXD         RX
GND         GND
VCC         5V
EN(KEY)     不要连5V
请注意：一定版本以上的ios系统手机可能无法连接HC05模块，请确认自己手机型号与系统支持BLE4.0协议
常用AT指令：
AT:测试通信
AT+ORGL:出厂设置
AT+ROLE=?:设置模式
AT+PSWD=?:设置密码
AT+NAME=?:设置名字
*/

#include <SoftwareSerial.h> 
//SoftwareSerial.h库是Arduino自带模拟串口通信库，如果丢失请从第三方下载并解压至libiary中
// Pin10为RX，接HC05的TXD
// Pin11为TX，接HC05的RXD
SoftwareSerial BT(10, 11); 
char val;

void setup() {
  Serial.begin(38400); 
  Serial.println("bluetooth is ready");
  // HC-05默认，9600
  BT.begin(9600);
}

void loop() {
  if (Serial.available()) {
    BT.print(Serial.read());
  }

  if (BT.available()) {
    char c =BT.read();
    Serial.print("received:");
    Serial.println(c);
    if(c=='1'){
      BT.println("2");
      Serial.println("Sent 2");
    }
  }//如果一切正常，手机发送1，HC05将回复2
}
//下面这串是ai写的自动化配置程序，测试成功，但重复使用可能有bug，未修复
// #include <SoftwareSerial.h>

// // ========== 用户配置区（可按需修改）==========
// #define Role        0           // 1: 主机 (Master), 0: 从机 (Slave)
// #define Name        "BTSLAVE"  // 蓝牙模块名称
// #define Pswd        "8964"      // 配对密码（字符串形式）
// #define Baudrate    38400       // AT模式波特率（恢复出厂后为38400）
// #define Readtimeout 3000        // 串口读取超时时间（毫秒）

// // 软串口引脚
// #define BT_RX_PIN   10          // CH05 的 TX → Nano 的 D10
// #define BT_TX_PIN   11          // CH05 的 RX → Nano 的 D11

// // KEY 引脚（用于进入AT模式，可选）
// #define BT_KEY_PIN  4           // 连接到CH05的KEY脚

// // 调试串口波特率
// #define DEBUG_BAUD  38400
// // =================================================

// // 创建软串口对象
// SoftwareSerial HC05Serial(BT_RX_PIN, BT_TX_PIN);

// // 系统状态变量
// uint8_t systemState = 0x00;
// unsigned long systemTime;

// // 状态码定义
// #define STATE_INIT            0x00  // 初始化
// #define STATE_TEST_AT         0x01  // 发送 AT 测试
// #define STATE_WAIT_AT_RESP    0x02  // 等待 AT 响应
// #define STATE_RESTORE_FACTORY 0x03  // 发送恢复出厂设置
// #define STATE_WAIT_ORGL_RESP  0x04  // 等待恢复响应
// #define STATE_CONFIGURE       0x05  // 执行其他配置
// #define STATE_COMPLETE        0xFF  // 完成

// void setup() {
//   Serial.begin(DEBUG_BAUD);
//   while (!Serial);
//   Serial.println("\nCH05 蓝牙配置程序启动...");

//   HC05Serial.begin(Baudrate);

// #ifdef BT_KEY_PIN
//   pinMode(BT_KEY_PIN, OUTPUT);
//   digitalWrite(BT_KEY_PIN, HIGH);
//   Serial.println("进入AT模式...");
//   delay(1000);
// #endif

//   systemState = STATE_INIT;
//   systemTime = millis();
// }

// void loop() {
//   switch (systemState) {
//     case STATE_INIT:
//       Serial.println("步骤1: 测试通信 (AT)");
//       HC05Serial.println("AT");
//       systemState = STATE_WAIT_AT_RESP;
//       systemTime = millis();
//       break;

//     case STATE_WAIT_AT_RESP:
//       if (HC05Serial.available()) {
//         String response = HC05Serial.readString();
//         Serial.print("响应: ");
//         Serial.println(response);

//         if (response.indexOf("OK") >= 0) {
//           Serial.println("通信正常，准备恢复出厂设置...\n");
//           systemState = STATE_RESTORE_FACTORY;
//         } else {
//           Serial.println("未收到OK");
//           systemState = STATE_COMPLETE;
//         }
//       }
//       else if (millis() - systemTime > Readtimeout) {
//         Serial.println("请确认模块已进入AT模式。");
//         systemState = STATE_COMPLETE;
//       }
//       break;

//     case STATE_RESTORE_FACTORY:
//       Serial.println("步骤2: 恢复出厂设置 → AT+ORGL");
//       HC05Serial.println("AT+ORGL");
//       systemState = STATE_WAIT_ORGL_RESP;
//       systemTime = millis();
//       break;

//     case STATE_WAIT_ORGL_RESP:
//       if (HC05Serial.available()) {
//         String response = HC05Serial.readString();
//         Serial.print("响应: ");
//         Serial.println(response);

//         if (response.indexOf("OK") >= 0) {
//           Serial.println("已恢复出厂设置！\n");
//           systemState = STATE_CONFIGURE;
//         } else {
//           Serial.println("恢复出厂可能未成功，但仍继续配置...");
//           systemState = STATE_CONFIGURE;
//         }
//       }
//       else if (millis() - systemTime > Readtimeout) {
//         Serial.println("恢复出厂超时，继续下一步...");
//         systemState = STATE_CONFIGURE;
//       }
//       break;

//     case STATE_CONFIGURE:
//       // 开始执行配置
//       sendSetName();
//       delay(1000);

//       sendSetPin();
//       delay(1000);

//       sendSetRole();
//       delay(1000);

//       // 最后重启使所有设置生效
//       Serial.println("最后：重启模块 → AT+RESET");
//       HC05Serial.println("AT+RESET");
//       delay(1500);

//       systemState = STATE_COMPLETE;
//       break;

//     case STATE_COMPLETE:
//       Serial.println("\n所有配置已完成");
//       Serial.print("名称: "); Serial.println(Name);
//       Serial.print("密码: "); Serial.println(Pswd);
//       Serial.print("角色: "); Serial.println((Role == 1) ? "主机 (Master)" : "从机 (Slave)");
//       Serial.println("请断开KEY脚,并重启模块以退出AT模式。");
//       Serial.println("现在可用手机搜索蓝牙并配对。");

//       while (1); // 停止
//   }
// }

// // ========== 辅助函数 ==========
// void sendSetName() {
//   String cmd = "AT+NAME=";
//   cmd += Name;
//   Serial.print("步骤3: 设置名称 → ");
//   Serial.println(cmd);
//   HC05Serial.println(cmd);
// }

// void sendSetPin() {
//   String cmd = "AT+PIN=";
//   cmd += Pswd;
//   Serial.print("步骤4: 设置密码 → ");
//   Serial.println(cmd);
//   HC05Serial.println(cmd);
// }

// void sendSetRole() {
//   String cmd = "AT+ROLE=";
//   cmd += Role;
//   Serial.print("步骤5: 设置角色 → ");
//   Serial.println(cmd);
//   HC05Serial.println(cmd);
// }

