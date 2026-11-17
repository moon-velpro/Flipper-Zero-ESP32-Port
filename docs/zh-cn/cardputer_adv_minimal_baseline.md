# Cardputer-Adv 中文补丁：最小化重建基线

## 基线与边界

- 基线：`main` 的 `e6ec11972a8e025d7ab686a6ef3016cf52360834`。
- 用户确认原版固件在实机上可以正常启动；此陈述是用户提供的硬件验证，不等同于此分支已有新构建/新烧录测试。
- 旧实验分支：`feature/cardputer-adv-zh-cn-r1`（从基线领先 75 次提交，GitHub 比较显示 30 个变动文件）。
- **本分支首先只保存审计记录；不直接合并或 cherry-pick 旧实验分支。**
- 禁止为了中文化改动任务栈、I2C、I2S、NFC、红外、键盘、分区表、Wi-Fi 配置及其它板型。

## 首轮差异分类（根据 GitHub compare 文件清单）

| 类别 | 旧分支文件 | 处理 |
| --- | --- | --- |
| 中文渲染核心 | `components/gui/canvas_utf8.c/.h`、`canvas.c`、`elements.c` | 审核后按最小依赖移植 |
| 中文控件 | `modules/text_box.c`、`modules/text_input.c`、`widget_element_text_scroll.c` | 在基础中文显示启动验证后逐项移植 |
| 中文菜单 | `components/loader/loader_menu.c` | 字体渲染验证后再移植 |
| 构建开关 | `components/gui/CMakeLists.txt`、`components/loader/CMakeLists.txt` | 仅允许 Cardputer-Adv 条件编译 |
| 硬件/系统改动 | `thread.c`、`furi_hal_*.c`、板级头文件、`target_input.c`、`main/app_main.c` | **不移植** |
| 运行配置改动 | `sdkconfig.defaults.m5stack_cardputer_adv`、`partitions_cardputer_adv.csv` | **不移植**，沿用可启动原版 |
| 测试与 CI | `tests/host/canvas_utf8_test.c`、旧 workflow | 可独立审阅，不能代替实机测试 |

## 必须逐步验证

1. 原版 SHA 对应的构建配置与用户可启动镜像是否完全一致（目前尚未拿到原版镜像 SHA-256）。
2. 在**不修改任何驱动/分区/Kconfig**的前提下加入独立 UTF-8 解码器和宿主测试。
3. 只加入 Flash 常驻字体和最低限度的中文绘制；检查链接映射、静态 DRAM、运行时内部堆和最大连续块。
4. 在实机验证启动和基本按键后，分批加入菜单翻译和控件适配。
5. 每一步分别保存提交、构建产物哈希和启动日志；任何失败立即回退到上一已验证节点。

## 当前验证状态

仅完成 GitHub 源码差异清点与分支隔离。**尚未编译、烧录或证明新的中文固件可以启动。** 不把旧实验分支的硬件修复当成中文补丁。
