# 05：第二层页面与 Stack 返回

## 本实验要回答的问题

- 同一个 `CommonActivatableWidgetStack` Push 第二个页面时，第一个页面发生什么？
- 为什么页面不应该自己搜索或直接持有 RootLayout 的 Stack？
- 栈顶页面退出后，上一层页面如何重新激活并恢复焦点？
- Back 与页面内“关闭”按钮能否走同一条退出路径？

## 预期结构

```text
WBP_CommonUI_RootLayout
└─ ScreenStack
   ├─ WBP_CommonUI_SmokeTest
   └─ WBP_CommonUI_SecondScreen   <- 当前栈顶
```

RootLayout 拥有并管理 `ScreenStack`。SmokeTest 只发出“请求打开第二页”的事件，不使用 `Get All Widgets Of Class` 搜索 RootLayout，也不直接创建第二页。

## 1. 创建第二层页面

在 `/Game/UI/Dev` 创建 `WBP_CommonUI_SecondScreen`：

- 父类：`LyraUIActivatableWidget`
- `Input Config = Menu`
- `Is Back Handler = true`

页面至少包含：

- 一块与 SmokeTest 明显不同的背景
- 文本 `Second Screen`
- 可聚焦按钮 `Button_Close`，显示文本 `Close`

实现：

```text
Get Desired Focus Target
  -> Return Value = Button_Close

Button_Close.OnClicked
  -> Deactivate Widget
```

在 `On Activated` 和 `On Deactivated` 中分别打印：

```text
SecondScreen Activated
SecondScreen Deactivated
```

## 2. SmokeTest 只发送导航请求

在 `WBP_CommonUI_SmokeTest` 中：

1. 新建无参数 Event Dispatcher：`RequestOpenSecondScreen`。
2. 可以复用 `Button_Second`，将其 `On Clicked` 改为调用 `RequestOpenSecondScreen`；也可以另加一个 `Button_OpenSecondScreen`。
3. 不在 SmokeTest 中 Create Widget、Push Widget 或查找 RootLayout。

页面负责表达用户意图，导航容器负责决定把哪个页面 Push 到哪一层。

## 3. RootLayout 接收请求并 Push

`WBP_CommonUI_RootLayout.ShowSmokeTest` 当前已经通过 `ScreenStack -> Push Widget` 创建 SmokeTest。

使用该节点的 `Return Value`：

1. Cast 到 `WBP_CommonUI_SmokeTest`。
2. 对它的 `RequestOpenSecondScreen` 执行 `Bind Event`。
3. 在绑定的自定义事件中调用：

```text
ScreenStack -> Push Widget
  Activatable Widget Class = WBP_CommonUI_SecondScreen
```

这样 RootLayout 仍是唯一知道 `ScreenStack` 和具体页面类的对象。

## 4. PIE 验证

1. 启动 PIE，确认 SmokeTest 激活并聚焦第一个按钮。
2. 导航到打开第二页的按钮并确认。
3. 应看到 SmokeTest Deactivated，随后 SecondScreen Activated。
4. 不点击鼠标，直接按 Enter/Space；`Button_Close` 应关闭第二页。
5. 再次打开第二页，按 Escape；结果应与 Close 按钮一致。
6. 第二页退出后，应看到 SecondScreen Deactivated，随后 SmokeTest Activated。
7. 验证 SmokeTest 能继续接收导航、确认和 Escape。

## 完成标准

- 第二页始终由同一个 `ScreenStack` Push。
- Push 后只有栈顶第二页处于激活状态并接管焦点、确认与 Back。
- Close 与 Back 都通过 `DeactivateWidget` 退出栈顶页面。
- 第二页退出后 SmokeTest 自动重新激活，无需再次 `Create Widget`。
- SmokeTest 不直接依赖 RootLayout 或 ScreenStack。

## 2026-09-24 实测结果

- 已创建 `/Game/UI/Dev/WBP_CommonUI_SecondScreen`，并由 RootLayout 的现有 `ScreenStack` Push。
- SmokeTest 通过无参数事件分发器 `RequestOpenSecondScreen` 表达导航请求；RootLayout 使用 Push 返回的实际 SmokeTest 实例完成绑定。
- Push 时观察到 `SmokeTest Deactivated`，随后 `SecondScreen Activated`。
- Stack 使用 `Fade Only` 过渡：旧页面先渐隐，新页面再渐显；视觉过渡由 Stack 执行，不是 `DeactivateWidget()` 自身的行为。
- 第二页的 `Button_Close` 可通过默认焦点和 Enter 触发 `DeactivateWidget()`，随后原 SmokeTest 实例重新激活。
- SmokeTest 重新激活后，CommonUI 再次查询 `Get Desired Focus Target`，焦点回到 `Button_First`；按 Enter 打印 `First Clicked`。
- 同一 Stack 内的非栈顶页面仍由 Stack 持有，但不再显示，也不位于 CommonUI 当前有效输入路径。
