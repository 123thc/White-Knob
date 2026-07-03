# White Knob

White Knob 是一个基于 ESP32-S3 的智能旋钮项目，集成圆形屏幕、无刷电机触觉反馈、磁编码器、压力按压检测、BLE HID、WiFi 配网、网页设置中心和 WS2812 灯效控制。它既可以作为桌面控制器使用，也可以作为番茄钟、时间显示器、灯光控制器和 Windows Surface Dial 兼容设备进行演示。

## 功能特性

- 圆形 LCD 界面，基于 LVGL 构建多页面交互。
- 智能旋钮模式，支持 Free、Detent 等不同手感模式。
- 番茄钟功能，支持按时、分、秒进行独立时间设置和倒计时。
- 时间界面显示，支持通过网页上传 240 x 240 图片作为背景。
- WiFi AP 配网页面，支持扫描 WiFi、输入密码并连接网络。
- 网页设置中心，集成配网、照片上传和自定义灯效配置。
- WS2812 灯效控制，支持开关、模式、颜色、亮度、速度和方向设置。
- BLE HID Surface Dial 模式，可在 Windows 中作为径向控制器使用。
- HX711 压力检测用于旋钮按压，AS5047P 用于角度读取。
- 自定义分区表，包含 NVS、factory app、others SPIFFS 和 html SPIFFS 分区。

## 硬件组成

项目当前代码面向 ESP32-S3，主要外设包括：

- ESP32-S3 模组或开发板
- 圆形 LCD 屏幕
- AS5047P 磁编码器
- TMC6300 电机驱动
- 三相无刷电机
- HX711 压力传感器
- WS2812 灯环或灯带
- 电源按键与电池 ADC 检测

主要引脚定义位于：

```text
main/config/pin_config.h
```

## 软件环境

- ESP-IDF 5.x
- 目标芯片：ESP32-S3
- Flash：16MB
- 构建系统：ESP-IDF CMake
- 依赖组件：`espressif/esp_tinyusb`

依赖声明位于：

```text
main/idf_component.yml
```

自动下载的依赖会生成到：

```text
managed_components/
```

该目录不需要提交到 GitHub，别人编译时 ESP-IDF 会自动恢复。

## 项目结构

```text
.
├── CMakeLists.txt
├── my_partitions.csv
├── sdkconfig
├── components/
│   ├── bsp/
│   ├── esp_rlottie/
│   └── lvgl/
└── main/
    ├── White_Knob.c
    ├── app_core/
    │   ├── dial/
    │   ├── html/
    │   ├── lcd/
    │   ├── led/
    │   └── smart_knob/
    ├── config/
    └── drivers/
        ├── ble_hid/
        ├── encoder/
        ├── hx711/
        ├── lcd/
        ├── motor/
        ├── time/
        ├── wifi/
        └── ws2812/
```

## 构建与烧录

在 ESP-IDF 环境中打开项目目录：

```bash
idf.py set-target esp32s3
idf.py build
idf.py flash monitor
```

如果使用 VSCode 的 ESP-IDF 插件，可以直接选择对应串口后执行 Build、Flash 和 Monitor。

## 网页配置

进入 WiFi 配网页面后，设备会开启 AP 配网模式。连接设备热点后，在浏览器打开：

```text
http://192.168.100.1
```

网页中可以完成：

- WiFi 扫描与连接
- 时间界面照片上传
- 自定义灯光模式配置

时间照片上传会在网页端转换为 RGB565 数据，ESP32 只负责保存和显示。

## Surface Dial 模式

Surface Dial 模式通过 BLE HID 模拟 Windows 径向控制器。进入该界面后，设备会启动蓝牙 HID 服务，Windows 配对成功后可接收旋转、按压和松开事件。

需要注意的是，Windows 圆形菜单中显示哪些工具项由 Windows 和当前应用决定。ESP32 负责发送标准 HID 输入，不能单独决定电脑端菜单中出现哪些功能。

## 分区说明

当前使用自定义分区表：

```text
my_partitions.csv
```

主要分区：

- `nvs`：保存系统参数
- `factory`：主程序
- `others`：其他 SPIFFS 数据
- `html`：网页资源 SPIFFS 分区

网页资源位于：

```text
main/app_core/html
```

构建时会被打包到 `html` 分区。

## Git 忽略建议

建议忽略自动生成文件和本地 IDE 配置：

```gitignore
build/
managed_components/
.vscode/
.idea/
sdkconfig.old
*.bin
*.elf
*.map
*.log
.DS_Store
Thumbs.db
```

建议提交 `dependencies.lock`，这样别人克隆项目后可以复现相同的 ESP-IDF 组件依赖版本。

## 说明

本项目仍处于持续开发阶段，功能包括 UI、BLE HID、WiFi、灯效、电机控制和网页配置等多个模块。修改代码时建议优先保持现有页面切换、按压回调和旋钮模式逻辑的一致性，避免不同功能之间互相影响。
