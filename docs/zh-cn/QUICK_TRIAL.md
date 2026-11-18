# Cardputer-Adv：最小中文固件试用指南

> 当前为预览源码（Draft PR #2）。原版可启动来自设备使用者确认；**最小中文分支尚无成功的 ESP-IDF 编译或实机开机报告**。不要把旧实验分支的 BIN 当成本分支产物。

## 本版实际包含

- Cardputer-Adv 专用 UTF-8 解码和 U8g2 文泉驿 GB2312 中文字形回退（字库随只读固件存储，非将整套字库复制到堆）。
- 列表样式主菜单与部分设置菜单中文显示（例如：应用、设置、蓝牙、界面、图形）。
- 常见菜单滚动及截断避免从 UTF-8 字节中间切断；中文滚动/截断路径已改用小型栈缓冲，避免 GUI 每帧反复申请堆内存。
- ASCII 与其他板型仍使用原有字体路径。
- **尚不是完整汉化版**；复杂文本框、中文键盘、全部应用菜单和极紧凑布局以后分阶段适配。

## 最快编译（Windows，ESP-IDF v5.4.1）

使用最小分支递归克隆后，在 PowerShell 中运行：

```powershell
git clone --recursive -b feature/cardputer-adv-zh-cn-minimal https://github.com/moon-velpro/Flipper-Zero-ESP32-Port.git
cd Flipper-Zero-ESP32-Port
powershell -ExecutionPolicy Bypass -File .\tools\build_cardputer_zh_preview.ps1
```

脚本仅执行环境检查与构建；失败时停止，不会自动烧录。成功后显示固件路径、字节数、SHA-256，并验证 8 MB Flash、无 PSRAM 及原版 6 MiB 应用分区限制。它无法代替实机启动测试，也不会自行更改原版的 ESP-IDF 或分区配置。

## 手动编译（Windows，ESP-IDF v5.4.1）

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

## 可复现的 UTF-8 宿主验证（2026-11-19）

从本 PR 分支检索下列文件，在隔离的 Linux 工作目录中还原，使用 `git hash-object` 校验其 Git Blob SHA 与仓库一致：

- `components/gui/canvas_utf8.c`：`f250b84dac0778a01c9a0a74cc5670e3b4cdc54b`
- `components/gui/canvas_utf8.h`：`e7a348e2822b584e529c5b7be77191b412e1d762`
- `tests/host/canvas_utf8_test.c`：`099af6f234e144820aa3eb99492a61e5b1638df9`

随后执行：

```sh
cc -std=c11 -Wall -Wextra -Werror -fsanitize=address,undefined -fno-omit-frame-pointer \
  -Icomponents/gui components/gui/canvas_utf8.c tests/host/canvas_utf8_test.c \
  -o /tmp/cardputer_utf8_test
ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1 /tmp/cardputer_utf8_test
```

输出：`canvas UTF-8 tests passed`。没有报告编译警告或 Sanitizer 错误。**这仅证明上述精确版本的 UTF-8 宿主测试通过；不代表 ESP-IDF 整机编译或 Cardputer-Adv 实机启动验证通过。**

## 状态与验证

- Github 审阅：[Draft PR #2](https://github.com/moon-velpro/Flipper-Zero-ESP32-Port/pull/2)
- 工作流：`.github/workflows/cardputer-adv-minimal-utf8.yml`，设计为逐个编译原版和最小汉化版、比较静态 DRAM、Flash 和 `sdkconfig`。
- 宿主 UTF-8 回归测试：在隔离的 Linux GCC/ASan/UBSan 镜像代码上执行并通过；**不是 ESP-IDF 固件构建结果**。
- 截至此记录写入时，GitHub API 尚未返回该新分支的 Actions 运行。
