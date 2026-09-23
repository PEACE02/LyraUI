# 02：最小 Activatable Widget Stack

## 本实验要回答的问题

- Stack 接收的是蓝图资产还是蓝图生成类？
- 页面实例由谁创建？
- Push 后为什么会自动激活？
- Back 为什么能让栈顶页面退出？
- 为什么只有根布局使用 `Add to Viewport`？

本实验先使用 Level Blueprint 做临时启动器，以便专注观察 CommonUI。稳定的 UI 创建位置会在后续生命周期实验中迁移到项目类，不把 Level Blueprint 当最终架构。

## 1. 完善 SmokeTest 页面

打开 `/Game/UI/Dev/WBP_CommonUI_SmokeTest`。

Designer：

1. 添加一个铺满页面的 `Border`。
2. 在 Border 中添加文本 `CommonUI Smoke Test - Active`。
3. 使用明显的背景色，保证 PIE 时能确认页面正在显示。

Class Defaults：

1. 搜索 `Back`。
2. 勾选 `Is Back Handler`。
3. 暂不重写 `On Handle Back Action`。UE 5.8 的默认实现会调用 `DeactivateWidget()`。

Graph：

1. `Event On Activated` -> `Print String`：`SmokeTest Activated`。
2. `Event On Deactivated` -> `Print String`：`SmokeTest Deactivated`。

编译并保存。

## 2. 创建根布局

在 `/Game/UI/Dev/` 创建 Widget Blueprint：

- 名称：`WBP_CommonUI_RootLayout`
- 父类：`CommonUserWidget`

Designer：

1. 以 `Overlay` 作为全屏根控件。
2. 添加 `Common Activatable Widget Stack`，铺满整个 Overlay。
3. 命名为 `ScreenStack`。
4. 确认 `Is Variable` 已勾选。
5. Stack 的 `Root Content Widget Class` 暂时留空。

在根布局 Graph 中新增公共函数 `ShowSmokeTest`：

```text
ScreenStack
  -> Push Widget
       Activatable Widget Class = WBP_CommonUI_SmokeTest
```

`Push Widget` 输入的是 `WBP_CommonUI_SmokeTest_C` 类，Stack 根据该类创建或从对象池取得页面实例。

编译并保存。

## 3. 临时启动根布局

打开当前测试关卡的 Level Blueprint：

```text
Event BeginPlay
  -> Get Player Controller (0)
  -> Create Widget
       Class = WBP_CommonUI_RootLayout
       Owning Player = Player Controller
  -> Promote Return Value to variable: RootLayout
  -> Add to Viewport
  -> ShowSmokeTest
```

注意：只有根布局执行 `Add to Viewport`。`WBP_CommonUI_SmokeTest` 必须通过 `ScreenStack -> Push Widget` 进入 UI。

## 4. PIE 验证

1. 开始 PIE，页面应显示。
2. 屏幕左上角应打印 `SmokeTest Activated`。
3. 按 `Escape` 或当前平台的 Back 键。
4. 应打印 `SmokeTest Deactivated`，页面从 Stack 自动移除。
5. 根布局仍留在 Viewport，只是 Stack 为空。

若 Back 没有触发，不要临时监听键盘事件绕过 CommonUI；记录现象，下一步检查 CommonInput 的默认 Back Action 与输入路由配置。

## 机制说明

`Push Widget` 的核心流程：

```text
传入 Widget Class (_C)
  -> Stack 从对象池获取或创建实例
  -> 注册到 Stack
  -> 成为栈顶
  -> ActivateWidget
  -> 页面接管对应输入与焦点
```

当栈顶页面执行 `DeactivateWidget()`：

```text
On Deactivated
  -> Stack 观察到栈顶停用
  -> 从容器移除该页面
  -> 恢复并激活前一个页面（如果存在）
```

## 完成标准

- PIE 中只有一个根布局实例。
- 测试页面由 `Push Widget` 创建，而不是直接 `Create Widget`。
- `On Activated` 和 `On Deactivated` 都被观察到。
- Back 能让测试页面退出。
- 能解释根布局与页面实例的不同所有权和生命周期。

