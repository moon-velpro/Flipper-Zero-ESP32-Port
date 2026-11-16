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

## 当前干净构建阻塞

在 ESP-IDF v5.4.1 的干净 CI 环境中，基线 e6ec11972a8e025d7ab686a6ef3016cf52360834 与中文分支均在链接阶段超出 DRAM 328 字节。两份 map 的 .dram0.data（29,140 B）和 .dram0.bss（202,344 B）完全相同；本次中文改动增加约 211,740 B Flash 内容，但没有增加静态 DRAM。该问题属于 Cardputer-Adv 基线构建配置，必须在发布前单独解决，不能通过关闭现有功能掩盖。

## 尚未完成

- **R1 代码尚未经过 Cardputer-Adv 实机验证。**
- `components/gui/modules/text_box.c` 与部分复杂格式文本控件仍然依赖原来的字节级分页逻辑，中文长文本的支持属于后续阶段。
- 各个内置应用中的业务页面、错误提示尚未全面汉化。
- 暂时没有语言切换设置：对 Cardputer-Adv 默认编译中文；其他板型为英文。
- 字形基线偏移当前为代码内的临时数值，须按实机显示调整。
- Unicode BMP 以外字符以 `?` 替代；未包含的汉字也使用替代符号。

## CI

`.github/workflows/cardputer-adv-zh-cn.yml` 使用 ESP-IDF **v5.4.1** 编译 Cardputer-Adv 固件，通过后保存构建产物，仅用于审核，不会自动烧录设备。
