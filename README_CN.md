
<h1 align = "center">🏆T-MixRF🏆</h1>


| 正面 | 反面 |
| :---: | :---: |
| ![alt text](./docs/README_img/T-MixRF_正面.png) | ![alt text](./docs/README_img/T-MixRF_标识.png) |

---
[English Version](README.md)

多功能射频开发板，支持多种无线通信协议

## 硬件规格

| 项目 | 参数 |
|------|------|
| 主控芯片 | ESP32-S3-WROOM-1-N16R8 |
| CPU | Xtensa® dual-core 32-bit LX7 |
| 主频 | 240MHz |
| 内存 | 16MB Flash + 8MB PSRAM |
| 工作电压 | 3.3V |

## 模块功能

### 1. LR1121 LoRa 模块
- **频段支持**:
  - 150~960 MHz (Sub-GHz)
  - 2400~2500 MHz (2.4GHz)
- **最大发射功率**:
  - Sub-GHz: 22 dBm
  - 2.4GHz: 13 dBm
- **SPI接口**: CS(33), DIO9(9), RST(37), BUSY(6)
- **用途**: 长距离通信、LoRaWAN、远距离传输

### 2. CC1101 Sub-GHz 模块
- **频段支持**: 315 / 433 / 868 / 915 MHz
- **SPI接口**: CS(11), GDO0(14), GDO2(12)
- **频段切换**: 通过SW0/SW1引脚控制
- **用途**: 低成本遥控、安防设备、家电控制

### 3. NRF24L01 2.4GHz 模块
- **频段**: 2400~2525 MHz
- **SPI接口**: CS(34), CE(4), IRQ(5)
- **特点**: 低功耗、支持多接收节点
- **用途**: 2.4GHz无线通信、传感器网络

### 4. ST25R3916 NFC 模块
- **支持协议**: ISO14443A/B, ISO15693, ISO18092, NFCIP-1
- **功能**: NFC标签读写、卡模拟、点对点通信
- **SPI接口**: BSS(36), EN(35)
- **用途**: NFC应用开发、门禁系统、电子支付

## 示例程序

| 示例 | 功能描述 |
|------|----------|
| `CC1101_send/CC1101_recv` | CC1101 发送/接收测试 |
| `LR1121_send/LR1121_recv` | LR1121 LoRa 发送/接收测试 |
| `NRF24_send_1/NRF24_recv_1` | NRF24L01 基本通信测试 |
| `NRF24_send_2` | NRF24L01 扩展功能测试 |
| `NFC/` | NFC标签读写、NDEF解析 |
| `rf_test/` | 多射频模块综合测试 |

## 库依赖

- **RadioLib**: LR1121/CC1101/NRF24L01 驱动
- **NFC-RFAL-fork**: ST25R3916 NFC协议栈
- **RF24-master**: NRF24L01 驱动库

## 引脚定义

```
SPI总线:
  SCK  = GPIO17
  MOSI = GPIO15
  MISO = GPIO16

LR1121: CS=33, DIO9=9, RST=37, BUSY=6
CC1101: CS=11, GDO0=14, GDO2=12, SW0=13, SW1=10
NRF24:  CS=34, CE=4, IRQ=5
ST25R3916: BSS=36, EN=35
```

## 应用场景

- 远距离LoRa物联网通信
- 多协议无线网关
- NFC/RFID应用开发
- 无线传感器网络
- 遥控与遥测系统

## 资料

- 硬件原理图: `hardware/` 目录
- 开发板定义: `boards/` 目录

