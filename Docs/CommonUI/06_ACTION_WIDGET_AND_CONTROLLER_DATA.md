# 06：动态按键提示与 Controller Data

## 本实验要回答的问题

- Back 已经可以工作，为什么还需要按键提示控件？
- 键盘按键为什么适合使用通用键帽加动态文字，而不是每个键制作一张贴图？
- Data Table 中的按键配置与 Controller Data 中的手柄图标配置分别负责什么？
- CommonInput 如何在当前输入类型变化后自动刷新提示图标？
- 缺少按键图标时，`CommonActionWidget` 为什么会在运行时自动折叠？

## 数据链路

```text
DT_CommonUIInputActions.Back
  └─ 为每种输入类型选择物理键
       Escape / Gamepad FaceButton Right / Android Back
                        │
                        ▼
ULyraUIActionPrompt 监听当前输入类型
  ├─ Keyboard/Mouse：FKey.GetDisplayName → 通用键帽 + Esc
  ├─ Gamepad：CommonActionWidget → Controller Data Brush
  └─ Touch：隐藏操作提示，使用直接可触摸的界面控件
```

提示控件只负责显示与刷新，不负责注册或触发 Back。当前 Back 能够关闭页面，仍然来自 `CommonGameViewportClient`、Action Router、`B_CommonUIInputData.DefaultBackAction` 和页面的 `Is Back Handler`。

## 1. 项目 C++ 包装控件

项目已添加 `ULyraUIActionPrompt`：

- `InputAction` 指向一个 `FCommonInputActionDataBase` 行。
- 键鼠模式读取该行的主 `FKey`，使用 `GetDisplayName(false)` 生成 `Esc`、`Space`、`Enter` 等短名称。
- 手柄模式改为显示内部的 `CommonActionWidget`；触摸模式隐藏两种提示。
- 监听 `UCommonInputSubsystem::OnInputMethodChangedNative`，不使用 Tick。

## 2. 创建可复用的外观蓝图

在 `/Game/UI/Foundation/Input` 创建 `WBP_CommonUI_ActionPrompt`：

- 父类：`LyraUIActionPrompt`
- `Input Action`：`DT_CommonUIInputActions.Back`

推荐层级：

```text
Overlay
├─ SizeBox (InputKeyContainer, Is Variable)
│  └─ Border
│     └─ Text (InputKeyText, Is Variable)
└─ Common Action Widget (InputActionIcon, Is Variable)
```

三个括号中的名字必须完全一致，它们对应 C++ `BindWidget` 属性。

建议外观：

- `InputKeyContainer`：Minimum Desired Width/Height 约为 `32 x 28`。
- Border：使用 Rounded Box 或普通 Box Brush，左右 Padding 约为 8～10。
- `InputKeyText`：居中、白色，字号约为 16～18。
- 不需要键帽纹理；Border 会随 `Esc`、`Space` 等文本长度自动扩展。

## 3. 在第二页使用 Back 提示

打开 `/Game/UI/Dev/WBP_CommonUI_SecondScreen`。

在页面底部的 `Horizontal Box` 中放入：

- `WBP_CommonUI_ActionPrompt`
- 普通 Text，内容为 `Back`

移除之前直接添加的裸 `Common Action Widget (Action_Back)`。提示控件不需要 Clicked 事件，也不是按钮。

## 4. 运行时观察

打开第二页后：

- 键鼠模式应显示通用键帽和动态文字 `Esc`。
- Escape 仍应正常关闭第二页，证明“动作路由有效”与“提示图标存在”互相独立。
- 后续接入手柄 Controller Data 后，切换到手柄输入应隐藏文字键帽并显示 B/Circle 图标。

最初创建的空 `B_CommonInput_KeyboardMouse` 已移除。以后需要鼠标按钮图片时，可以迁移 Lyra 的键鼠 Controller Data 或重新创建项目版本；普通键盘键不需要它。

## 完成标准

- 能区分 Input Action Data、动态键名和 Controller Data 的职责。
- Back 提示使用项目自己的 `DT_CommonUIInputActions.Back`，不依赖硬编码文本判断当前设备。
- Escape、Space、Enter 等键盘按键共用同一个键帽样式并动态显示文字。
- 当前输入类型改变时，Action Widget 自动刷新，无需蓝图 Tick。

## 2026-09-24 键鼠实测

- 已创建 `/Game/UI/Foundation/Input/WBP_CommonUI_ActionPrompt`，父类为 `ULyraUIActionPrompt`。
- `InputKeyContainer`、`InputKeyText` 和 `InputActionIcon` 已通过 `BindWidget` 与 C++ 属性自动绑定。
- SecondScreen 已使用该提示控件；PIE 中能够从 `DT_CommonUIInputActions.Back` 读取 Escape 并显示动态键帽 `Esc`。
- 键盘提示显示不负责注册输入，原有 Escape Back 路由保持正常。
- 下一步迁移 Lyra 的 Xbox Controller Data 及其依赖，验证 Gamepad 分支。

## Xbox Controller Data

已从 Lyra 迁移：

```text
/Game/UI/Foundation/Platform/Input/GamepadXboxSeriesX/CommonInput_Gamepad_XSX
```

迁移工具同时带入它实际引用的 `ControllerConfig` 和 XboxOne 按键纹理。项目通过 `Config/Windows/WindowsGame.ini` 注册该 Controller Data：

```ini
[CommonInputPlatformSettings_Windows CommonInputPlatformSettings]
DefaultInputType=MouseAndKeyboard
bSupportsMouseAndKeyboard=True
bSupportsTouch=False
bSupportsGamepad=True
DefaultGamepadName=XSX
bCanChangeGamepadType=True
+ControllerData=/Game/UI/Foundation/Platform/Input/GamepadXboxSeriesX/CommonInput_Gamepad_XSX.CommonInput_Gamepad_XSX_C
```

该配置只声明 Windows 支持哪些输入类型、默认手柄族以及到 Controller Data 的引用；它不会创建游戏操作 IA/IMC，也不会改变 Back Action 本身。

## 当前验收状态

- 键鼠：PIE 已确认显示动态键帽 `Esc`，Escape Back 行为正常。
- 手柄：XSX Controller Data 及依赖已迁移并注册；实体手柄运行时从 `Esc` 切换为 B 图标的验收暂缓。
- 触摸：`ULyraUIActionPrompt` 显式隐藏键帽和操作图标，触摸端使用直接可点击的 UI。
