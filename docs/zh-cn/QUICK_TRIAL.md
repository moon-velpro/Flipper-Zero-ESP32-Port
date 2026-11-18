# Cardputer-Adv：最小中文固件试用指南

> 当前为预览源码（Draft PR #2）。原版可启动来自设备使用者确认；**最小中文分支尚无成功的 ESP-IDF 编译或实机开机报告**。不要把旧实验分支的 BIN 当成本分支产物。

## 本版实际包含

- Cardputer-Adv 专用 UTF-8 解码和 U8g2 文泉驿 GB2312 中文字形回退（字库随只读固件存储，非将整套字库复制到堆）。
- 列表样式主菜单与部分设置菜单中文显示（例如：应用、设置、蓝牙、界面、图形）。
- 常见菜单滚动及截断避免从 UTF-8 字节中间切断。
- ASCII 与其他板型仍使用原有字体路径。
- **尚不是完整汉化版**；复杂文本框、中文键盘、全部应用菜单和极紧凑布局以后分阶段适配。

## 编译（Windows，ESP-IDF v5.4.1）

```powershell
git clone --recursive -b feature/cardputer-adv-zh-cn-minimal https://github.com/moon-velpro/Flipper-Zero-ESP32-Port.git
cd Flipper-Zero-ESP32-Port
python winbuild.py check
python winbuild.py build --board cardputer_adv
```

必须使用本项目的 ESP-IDF **5.4.1**，不要直接用其它设备项目的 ESP-IDF 6.x。构建脚本读取 `ESP_IDF_DIR`，默认 Windows 路径是 `C:\Espressif\frameworks\esp-idf-v5.4.1`。

编译后预期产物：`build_cardputer_adv/furi_esp32.bin`，但文件只有**真正编译成功后**才能使用。主应用必须不大于原版 factory 分区 0x600000 字节；还需核实 `sdkconfig` 与原版一致（8MB Flash、无 PSRAM）。

## 仅在编译成功、确认原版有可恢复备份后刷机

请先确认当前串口号；下方 `COM14` 仅作示例：

```powershell
python winbuild.py flash --board cardputer_adv --port COM14
python winbuild.py monitor --port COM14 --duration 30
```

首次运行检查：能否正常启动；列表式主菜单是否显示中文“应用/设置”；英文应用是否可打开；上/下/确定/返回按键是否正常；无 FreeRTOS assert、循环重启。测试紧凑菜单样式时仍保留英文，避免 12px 字形压到小行距。

如果无法启动，请恢复经验证能启动的原版镜像，保存启动串口日志与该分支 commit；不要靠继续修改任务栈或驱动来“试”修问题。

## 状态与验证

- Github 审阅：[Draft PR #2](https://github.com/moon-velpro/Flipper-Zero-ESP32-Port/pull/2)
- 工作流：`.github/workflows/cardputer-adv-minimal-utf8.yml`，设计为逐个编译原版和最小汉化版、比较静态 DRAM、Flash 和 `sdkconfig`。
- 宿主 UTF-8 回归测试：在隔离的 Linux GCC/ASan/UBSan 镜像代码上执行并通过；**不是 ESP-IDF 固件构建结果**。
- 截至此记录写入时，GitHub API 尚未返回该新分支的 Actions 运行。
