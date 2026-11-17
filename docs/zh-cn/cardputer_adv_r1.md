# M5Stack Cardputer-Adv 简体中文适配 · R1

> 开发分支：`feature/cardputer-adv-zh-cn-r1`  
> 原始基线：`e6ec11972a8e025d7ab686a6ef3016cf52360834`  
> 目标硬件：M5Stack Cardputer-Adv（ESP32-S3FN8，8 MB Flash，无 PSRAM）  
> ESP-IDF：**v5.4.1**

## R1 的范围

- 仅当 `FLIPPER_BOARD=m5stack_cardputer_adv` 时启用 `FLIPPER_ZH_CN` 编译开关。
- 原有拉丁文字仍使用系统的 U8g2 字体；常用中文字符使用项目自带的 `u8g2_font_wqy12_t_gb2312` 回退字体。
- 字符宽度、菜单文本滚动、截断、多行提示文字使用 UTF-8 字符边界。
- 主菜单、设置菜单、界面、图形设置、锁屏设置、状态栏等显示标签增加简体中文映射。
- **显示名称与内部应用名称分离**：`appid`、应用启动参数、文件路径、设置存储键和枚举顺序不翻译。
- 主菜单的 C64、Compact、MNTM 样式使用 8–10px 行高，小于中文字体高度；这些样式下应用名称保留英文，避免行间覆盖。设置子菜单仍可显示中文。
- 其他板型保留原来的英文界面和字体逻辑。

## 在 Windows 上编译

确认 ESP-IDF 5.4.1 环境已经安装。无需安装第二份 ESP-IDF。

```powershell
git clone --branch feature/cardputer-adv-zh-cn-r1 https://github.com/moon-velpro/Flipper-Zero-ESP32-Port.git
cd Flipper-Zero-ESP32-Port

# 仅在 ESP-IDF 5.4.1 不在脚本默认位置时设置：
# $env:ESP_IDF_DIR = "D:\tools\esp-idf-v5.4.1"

python winbuild.py check
python winbuild.py build --board cardputer_adv
```

**此阶段不要直接刷写。** 先确认编译成功、固件尺寸符合分区限制，再决定是否进行实机测试。

## 回归检查

1. 编译与链接：确认中文字库符号存在，Flash 应用镜像没有超过分区大小。
2. 主菜单：检查“应用”“设置”“关于”等字体完整且位置正确。
3. 菜单滚动：检查中英混合的长菜单项不会出现半个汉字或乱码。
4. 界面设置：确认菜单样式、开关与状态栏选项仍能操作。
5. 功能安全：进入、退出应用后，原有应用启动 ID 及配置文件内容不变。
6. 屏幕布局：检查中文 12px 字形在原逻辑 128×64 界面中的行距、上下边距以及截断。

## Cardputer ADV 内存配置与发布门槛

Cardputer ADV 的 ESP32-S3FN8 没有 PSRAM，内部 SRAM 需要在 IRAM 与 DRAM 之间分配。早期干净构建在两项 Wi-Fi IRAM 优化都开启时出现 328 B DRAM 超限。

当前板级配置使用 ESP-IDF 5.4 的正式 Kconfig 名称 ESP_WIFI_*（ESP32_WIFI_* 是旧名称的兼容重命名）：只关闭通用 Wi-Fi IRAM 快速路径（CONFIG_ESP_WIFI_IRAM_OPT=n），保留接收路径优化（CONFIG_ESP_WIFI_RX_IRAM_OPT=y），并保留 AMPDU RX/TX、10 个静态 RX 缓冲、32 个动态 RX 缓冲和默认 32 个动态 TX 缓冲。这样避免通过关闭聚合或削减发送队列来换取内存；通用 Wi-Fi 快速路径改由 Flash 执行，吞吐仍可能下降。CI 会断言这些配置确实进入生成的 sdkconfig，并使用同一份板级配置比较原版与中文版。

发布前必须在 Cardputer ADV 实机验证 Wi-Fi 连接、扫描、持续收发、抓包以及 Wi-Fi/BLE 共存。若吞吐、丢包或响应延迟出现不可接受的退化，不得发布；不能仅凭编译成功判定通过。


## 尚未完成

- **R1 代码尚未经过 Cardputer-Adv 实机验证。**
- `TextBox` 导航、`elements_text_box` 富文本绘制、滚动文本和文本输入光标已增加 UTF-8 字符边界处理；其他应用内部自绘文本控件仍需逐一审查，不能据此认为全项目页面都已完成中文适配。
- 各个内置应用中的业务页面、错误提示尚未全面汉化。
- 暂时没有语言切换设置：对 Cardputer-Adv 默认编译中文；其他板型为英文。
- 字形基线偏移当前为代码内的临时数值，须按实机显示调整。
- Unicode BMP 以外字符以 `?` 替代；未包含的汉字也使用替代符号。

## CI

`.github/workflows/cardputer-adv-zh-cn.yml` 使用 ESP-IDF **v5.4.1** 编译 Cardputer-Adv 固件，通过后保存构建产物，仅用于审核，不会自动烧录设备。

## R1 构建验证记录（2026-11-17）

提交 `d261184bda99c82ae15a2363c659c1f0945dc7ba` 已完成 Cardputer ADV 基线/汉化固件构建，两个版本静态 DRAM 均为 231484 B，链接区溢出均为 0 B。汉化增加 214792 B Flash `.text + .rodata`，应用镜像 3369872 B，位于 6291456 B 应用分区内。固件 Artifact 为 GitHub Actions `43961303713` 中的 `cardputer-adv-zh-cn-r1`。其他四种板型构建、Native Tools 和 ProtoPirate 测试同时全部通过。

之后的 `056bcc5` 增加 `TextBox` 字体变化时重建分页的修复；**它不属于上面这个已构建固件**，须使用含该提交的新构建产物验证，不可将旧 BIN 当作最新提交的实机验收结果。

## 实机验收操作与回滚（只对 Cardputer-Adv）

测试前确认设备确为 **ESP32-S3FN8 / 8 MB Flash / 无 PSRAM**，串口号以设备管理器显示为准。使用 Windows PowerShell 和 ESP-IDF v5.4.1 对应环境。请确保没有串口监视器正在占用端口。

**1. 保存整片 Flash 备份：**

```powershell
$Port = "COMx"  # 替换为实际端口
python -m esptool --chip esp32s3 --port $Port flash_id
python -m esptool --chip esp32s3 --port $Port read_flash 0x0 0x800000 cardputer_adv_backup_8mb.bin
```

确认备份文件完整，大小应为 `8388608` 字节。备份可能包含 Wi-Fi 凭据、密钥和用户数据，应本地妥善保存，不要上传公共仓库。**备份失败时停止测试，不要刷写或擦除。**

**2. 仅刷写已通过 CI 的整套同版本产物：**

将 GitHub Actions 固件压缩包解压到一个目录，从该目录执行：

```powershell
python -m esptool --chip esp32s3 --port $Port --baud 460800 write_flash --flash_mode dio --flash_freq 80m --flash_size 8MB 0x0 bootloader/bootloader.bin 0x8000 partition_table/partition-table.bin 0x10000 furi_esp32.bin
```

如串口或写入异常，检查 USB 数据线、供电与串口占用；不要在验证过程中自行调整 Flash 分区地址、启用 PSRAM 或跨板烧录。

**3. 实机功能验收表：**

| 项目 | 通过标准 |
| --- | --- |
| 上电与重启 | 冷启动、软重启和连续 20 次进入菜单均无黑屏、复位循环或异常日志 |
| 中文主菜单 | “应用”“设置”等可读，焦点/选中框不遮挡，启动目标正确 |
| 中英混排 | 长标签连续滚动多个周期，不能出现半个 UTF-8 字符、乱码或越界 |
| 变更菜单样式 | List/Wii/DSi/PS4/Vertical 正常；C64/Compact/MNTM 按设计保留英文以防文字重叠 |
| 设置菜单 | 中文标题、选项、开关状态清楚；更改后保存，重启仍正确 |
| TextBox 与输入 | 中文分页上下滚动、切换字体后的分页重算、光标左右移动、退格和边界截断正确 |
| 文件与 SD 卡 | 挂载、浏览、打开和拔卡/重插后菜单正常，存储路径未受翻译影响 |
| Wi-Fi | 扫描、连接、长时间收发和抓包正常，记录吞吐/掉包/延迟，与未汉化基线比较 |
| BLE 与共存 | BLE 扫描、连接/断开、重新连接，Wi-Fi 同时运行无死锁或明显退化 |
| 运行稳定性 | 持续使用至少 30 分钟，无崩溃、内存耗尽或持续增长的泄漏迹象 |

记录测试固件的 Git SHA、BIN SHA-256、串口启动日志、出错时的复现步骤及界面照片。任何未通过项应标记失败，不要以“编译通过”代替实机结论。

**4. 回滚：**

如果测试异常，停止继续操作并保留现场日志。设备进入下载模式后，可用备份恢复原先 Flash：

```powershell
python -m esptool --chip esp32s3 --port $Port --baud 460800 write_flash --flash_mode dio --flash_freq 80m --flash_size 8MB 0x0 cardputer_adv_backup_8mb.bin
```

回滚镜像必须是**同一设备**测试前取得的完整备份。不要把别人的 8 MB 备份写入自己的设备。

**发布判定：** 最新提交 CI 全绿且完成上述实机验收后，再考虑将 Draft PR 转为 Ready。当前仍为开发试点，未宣布量产或正式发布。

## 首轮实机烧录反馈：启动循环（2026-11-17）

使用 2026-11-16 21:41 编译的已通过 CI 的 R1 镜像，在 Cardputer-ADV 实机通过 8 MB DIO Flash 引导、加载应用、初始化屏幕，随后在服务启动阶段发生确定性的循环崩溃。**此前的 CI 绿色不能代表实机启动成功。** 关键异常：

```text
assert failed: xTaskCreateStaticPinnedToCore freertos_tasks_c_additions.h:299
(xPortcheckValidStackMem(puxStackBuffer))
... Stack for 'PowerSrv' (4096 B) in internal RAM (PSRAM full)
E InputCardputerAdv: i2c_driver_install failed: ESP_FAIL
E esp_core_dump_flash: No core dump partition found!
E gpio: gpio_set_level(238): GPIO output gpio_num error
```

源代码审计发现：在无 PSRAM 的 ADV 上，`furi_thread_alloc_stack()` 的内部 RAM fallback 可能返回 NULL，旧代码仍把 NULL 交给 `xTaskCreateStatic`，导致上述断言；旧日志不能单独证明究竟是哪个内存区/碎片造成该分配失败。Power HAL 误占用 I2C0（Grove GPIO2/1），与 TCA8418 键盘专用 I2C0（GPIO8/9）冲突；NFC 不存在的 IRQ/RST 引脚以 UINT16_MAX 表示，但之前的 `#ifdef` 判断把这些数值当作实际 GPIO。

针对实机故障提交的下一轮修复（**尚未编译、尚未实机验证**）：
- Power HAL 按 `BOARD_POWER_SKIP_I2C` 禁止提前安装 I2C0；对 ADV 的外接 Grove I2C API 使用 I2C1，键盘独占 I2C0。
- NFC 检查 IRQ/RST 引脚不等于 UINT16_MAX，避免向 ESP-IDF 传递 GPIO65535。
- ADV 显示 DMA stripe 从 32 行（15,360 字节）降到 8 行（3,840 字节），减少 11,520 字节内部 DMA heap；仅对该板有效。
- ADV SpeakerWorker 改为首次实际出声时启动，释放启动阶段 4 KiB 的栈需求；对其他板型保留原行为。
- 栈分配失败时打印 `[TASK_STACK_OOM]` 和 internal free/largest/min_free，而非把 NULL 传给 FreeRTOS；各启动服务额外记录可分配内存。
- ADV 独立分区表在原有 6 MiB factory app 与 u2fkeys 后添加 128 KiB `data,coredump`；CI 对照版同步使用该分区表。

**测试状态：** 旧 ZIP（提交 `d261184`）实机可加载但循环崩溃，禁止再推荐它作为稳定固件；新修复尚无成功编译和实机复测记录。新烧录包须来自后续成功 CI 的同一提交的 bootloader / partition table / app 三件套。新分区表不可和旧包混刷；不需要额外执行全片擦除。
