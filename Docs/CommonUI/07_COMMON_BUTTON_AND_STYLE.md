# 07：CommonButtonBase 与可复用样式

## 本实验要回答的问题

- `CommonButtonBase` 与普通 UMG `Button` 的核心区别是什么？
- 为什么按钮的行为 Widget 和外观 Style 要分开？
- Hovered、Pressed、Selected、Disabled 分别是什么状态？
- 按钮状态变化时，文字样式如何跟随背景样式变化？

## 本阶段的最小结构

```text
ULyraUIButtonBase（C++）
  ├─ CommonButtonBase：点击、焦点、选择、输入动作和状态
  ├─ ButtonText：每个按钮实例自己的文字
  └─ Text_ButtonLabel：显示文字，并跟随当前按钮文本样式

ButtonStyle_CommonUI_Primary（CommonButtonStyle 蓝图类）
  ├─ Normal / Hovered / Pressed / Disabled Brush
  ├─ MinWidth / MinHeight / Padding
  └─ 各状态对应的 CommonTextStyle

WBP_CommonUI_TextButton（ULyraUIButtonBase 蓝图子类）
  └─ Common Text：Text_ButtonLabel
```

`CommonButtonBase` 本身已经拥有内部按钮与交互逻辑。制作
`WBP_CommonUI_TextButton` 时，不要再向其中添加普通 UMG `Button`；只添加按钮的视觉内容。

## 为什么使用 Class Style

`UCommonButtonStyle` 和 `UCommonTextStyle` 都是以蓝图类及其 CDO 保存默认值。
按钮实例持有 Style 类引用，运行时从 Style CDO 读取统一的 Brush、Padding、尺寸、文字和声音配置。

因此修改一个 Style，就可以统一改变所有引用它的按钮，而不必逐个修改页面 Widget。

## 第一步：项目按钮基类

项目已添加 `ULyraUIButtonBase`。它刻意保持很薄：

- 不重新实现点击和焦点逻辑。
- 暴露可按实例编辑的 `ButtonText`。
- 要求蓝图中存在名为 `Text_ButtonLabel` 的 `Common Text`。
- 当 CommonUI 根据按钮状态切换当前 Text Style 时，将其同步给文字控件。

编译完成后再进入编辑器创建 Style 和按钮蓝图。

## 编译结果

- `LyraUIEditor Win64 Development` 已通过编译。
- 直接在受限环境运行 UBT 时出现的 `0xe0434352` 弹窗，实际原因是 UBT 无权清理用户 `AppData` 中的旧 Trace 日志；允许其访问日志目录后构建成功，与项目 C++ 代码无关。

## 本轮验收（进行中）

- C++ 项目成功编译。✅
- 创建 Normal、Hovered、Disabled 三种 `CommonTextStyle`。✅
- 创建一个 `CommonButtonStyle`，配置背景 Brush、尺寸和上述 Text Style。✅
- 创建 `WBP_CommonUI_TextButton`，并替换 SmokeTest 的两个普通 Button。✅
- PIE 验证鼠标 Hover/Click 和 Disabled 外观。✅
- 实体手柄导航与确认暂缓验收。

## 根内容为什么保留 Overlay

`CommonButtonBase::Initialize()` 会把蓝图的根内容放进它自动创建的内部 Button，
并强制该根内容的 `ButtonSlot` 使用水平和垂直 Fill。这保证视觉内容覆盖完整的可点击区域。

如果直接使用 `Common Text` 作为根内容，对根 Slot 设置的居中方式会在编译重建时被覆盖。
当前按钮因此使用：

```text
Overlay（根；由 CommonButtonBase 强制 Fill）
└─ Text_ButtonLabel（Overlay Slot 中居中）
```

## 导航防重入实验

用新的 CommonButton 快速连续按 Enter 打开第二页时，日志曾出现两次
`Second Clicked`，并创建了 `SecondScreen_C_0` 和 `SecondScreen_C_1` 两个实例。
这不是关闭页面时连续 Pop：SmokeTest 在第一次 Push 时正常 Deactivated，随后因为第二次
Push 打断正在进行的 AnimatedSwitcher 转场，关闭两个 SecondScreen 后没有正常重新 Activated。

当前最小防护：

- `Button_Second` 第一次点击后立即 `Set Is Interaction Enabled(false)`，再发送打开请求。
- SmokeTest 再次 `On Activated` 时恢复 `Button_Second` 的交互。
- SecondScreen 的关闭按钮也在第一次点击后禁用，并在 `On Activated` 时恢复。

PIE 快速连续 Enter 已无法复现重复 Push。后续实现分层 RootLayout 时，应参考 Lyra 在
Stack 转场期间统一 Suspend/Resume 输入，而不是长期依赖每个按钮各自防重入。

## Focus、Hover 与 Selected 的最终取舍

当前项目按 Lyra 的输入设备取舍处理通用按钮：

- 鼠标通过真实 Hover 使用 Hovered Style。
- 手柄通过 `bLinkCursorToGamepadFocus=True` 将虚拟光标移动到焦点控件，从而复用 Hovered Style。
- 键鼠模式主要使用鼠标，不为键盘 Focus 增加独立视觉；项目未覆盖
  `CommonButtonAcceptKeyHandling`，因此保持 Lyra 的 `Ignore` 默认行为，Enter/Space
  不直接触发当前焦点 CommonButton。
- Selected 保留给 Tab、单选项和当前项等持久业务状态，不用来模拟 Focus。
- `RenderFocusRule=Never` 关闭 Slate 默认焦点框，与 Lyra 一致。

因此不在 `ULyraUIButtonBase` 中增加 Focused Style，也不在
`WBP_CommonUI_TextButton` 中保留 `Border_Focus`。实体手柄的虚拟光标 Hover 表现仍留待设备可用时验收。
