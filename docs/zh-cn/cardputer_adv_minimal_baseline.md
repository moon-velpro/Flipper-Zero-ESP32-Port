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

已经开始最小中文绘制与第一批菜单翻译；**尚未通过新的 ESP-IDF 固件构建或实机验证**。GitHub Actions 尚未返回本分支运行记录。不把旧实验分支的硬件修复当成中文补丁。

## 阶段 1：独立 UTF-8 解码器（已提交源码，未验证）

已从旧分支单独提取 `components/gui/canvas_utf8.c/.h` 与 `tests/host/canvas_utf8_test.c`，并去掉解码器的 `FLIPPER_ZH_CN` 包围宏，保证源文件可独立供宿主编译。**没有修改 CMakeLists.txt**，因此该解码器目前不会进入设备固件，也没有改变原版任何启动路径或运行内存占用。

在仓库根目录的宿主测试命令：

```sh
cc -std=c11 -Wall -Wextra -Werror -fsanitize=address,undefined -fno-omit-frame-pointer -Icomponents/gui components/gui/canvas_utf8.c tests/host/canvas_utf8_test.c -o /tmp/cardputer_utf8_test
/tmp/cardputer_utf8_test
```

以上是**待执行**命令，并非已获得的测试结果。下一步应先运行宿主测试，再最小化接入 Cardputer-Adv GUI 构建，并比较与原版相同 sdkconfig 下的镜像/内存；在获得构建证据之前不移植字体或翻译。

## 阶段 2：最小中文预览（源码已实现）

- `components/gui/canvas.c`：仅 Cardputer-Adv 打开 `FLIPPER_ZH_CN`，CJK 字符由 U8g2 GB2312 Flash 字体回退，ASCII 仍沿用原字体。
- `components/gui/elements.c`：列表菜单的 UTF-8 滚动和截断不破坏码点边界。
- `components/loader/loader_menu.c`：首批常见中文标签；保持英文 app ID、path、配置 key 不变；主菜单只在 List 样式显示中文。
- 构建工作流升级为从原版 SHA 检出两个独立工作区，编译两份固件，比较 sdkconfig、静态 DRAM 与 Flash 镜像大小。
- 独立宿主 GCC + ASan/UBSan 在本地镜像解码器代码上执行了回归测试并通过；仍需在 CI 用仓库中的源文件重复验证。
- **不涉及**原版 `sdkconfig.defaults.m5stack_cardputer_adv`、分区表、`components/furi_hal/*`、`components/furi/core/thread.c` 或板级输入。
- Draft PR：#2；试用编译与安全验证指引见 `docs/zh-cn/QUICK_TRIAL.md`。
