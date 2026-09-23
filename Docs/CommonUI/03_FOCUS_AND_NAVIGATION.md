# 03：默认焦点与基础导航

## 本实验要回答的问题

- 页面激活和控件获得焦点是什么关系？
- CommonUI 如何决定页面激活后首先聚焦哪个控件？
- 键盘和手柄方向导航是否需要 IA/IMC？
- 如何在不依赖焦点外观的情况下验证当前焦点？

本实验先使用普通 UMG `Button`，暂不引入 `CommonButtonBase`、Button Style 和 Text Style。这样可以单独观察 CommonUI 的激活焦点与 Slate 导航；CommonButton 将在后续样式实验中引入。

## 1. 添加两个可聚焦按钮

打开 `/Game/UI/Dev/WBP_CommonUI_SmokeTest`。

在现有页面内容中添加一个纵向排列的 `Vertical Box`，并放入两个普通 `Button`：

- `Button_First`，文字为 `First`
- `Button_Second`，文字为 `Second`

两个按钮都需要：

- 勾选 `Is Variable`
- 保持 `Is Focusable` 为 true
- 在纵向布局中按 First、Second 的顺序排列

分别绑定 `On Clicked`：

```text
Button_First.OnClicked  -> Print String "First Clicked"
Button_Second.OnClicked -> Print String "Second Clicked"
```

打印用于验证焦点位置，不依赖默认 Button Style 是否清楚显示 Focused 状态。

## 2. 指定页面默认焦点

在 SmokeTest 蓝图的 Overrides 中实现：

```text
Get Desired Focus Target
  -> Return Value = Button_First
```

`UCommonActivatableWidget` 默认启用 `Supports Activation Focus`。当 SmokeTest 成为当前最深层的激活页面时，CommonUI 会查询 `Get Desired Focus Target`，并尝试把焦点设置到返回的控件。

不要在 `On Activated` 中手动调用 `Set Keyboard Focus`；本实验需要验证 CommonUI 自己的激活焦点机制。

## 3. 声明菜单输入配置

当前项目的 `ULyraUIActivatableWidget` 仍是最小空基类，不像 Lyra 的基类那样原生提供输入模式属性。因此先在 SmokeTest 蓝图中实现：

```text
Get Desired Input Config
  -> Make UIInputConfig
       Input Mode = Menu
       Mouse Capture Mode = No Capture
       Mouse Lock Mode = Do Not Lock
       Hide Cursor During Viewport Capture = false
  -> Return Value
```

`Get Desired Focus Target` 只说明页面内部应该聚焦哪个控件；`Get Desired Input Config` 才负责 Menu/Game 输入模式和鼠标捕获状态。不要在 Level Blueprint 中手动调用 `Set Input Mode UI Only` 或设置 `Show Mouse Cursor`。

在 PIE 的“高级设置”中启用 `Game Gets Mouse Control`。UE 5.8 在编辑器中只有当游戏视口位于焦点路径时才应用 CommonUI 的 Input Config 和默认焦点；否则启动时会跳过这一步。该选项是本机编辑器偏好，不属于项目运行时配置。

## 4. PIE 验证

开始 PIE 后不要先点击页面：

1. 直接按 `Enter` 或手柄确认键，应打印 `First Clicked`。
2. 按方向键向下或手柄 D-Pad Down。
3. 再按确认键，应打印 `Second Clicked`。
4. 按方向键向上，再确认，应重新打印 `First Clicked`。
5. 按 `Escape`，SmokeTest 应正常处理 Back 并关闭。
6. 按 `Shift+Escape` 停止 PIE。

键盘方向导航和基础手柄方向导航由 Slate/UMG Navigation 处理，本实验不需要创建 IA 或 IMC。

普通 UMG `Button` 的键盘确认由 Slate Navigation 提供，默认 `Enter` 和 `Space` 都映射为 `Accept`，不依赖 `B_CommonUIInputData.DefaultClickAction`。如果确认键偶发无效，应完全停止并重新运行 PIE，再确认游戏视口焦点；不要先用 Default Click Action 掩盖焦点问题。

## 完成标准

- 页面激活后无需鼠标点击，第一次确认直接触发 First。
- Up/Down 可以在两个按钮间移动。
- 键盘与手柄均能完成导航和确认。
- Back 行为没有被焦点控件破坏。
- 能区分页面 `Activated`、UI Action 输入可达和具体控件 Focus 三个概念。

## 2026-09-23 实测结果

- Action Router 成功应用 `Menu + NoCapture + DoNotLock`。
- 日志确认默认焦点目标为 `Button_First`。
- `Enter` 和 `Space` 均可触发当前聚焦按钮。
- Up/Down 可在两个按钮之间导航，First/Second 的点击打印均已观察到。
- `Escape` 仍由 SmokeTest 的 Back Handler 处理。
- 键盘与鼠标验收通过；实体手柄验证可在具备设备时补做。
