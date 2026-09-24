# 04：Lyra 式 Activatable Widget 输入配置

## 目标

把每个页面重复实现的 `Get Desired Input Config` 上移到项目自有的 `ULyraUIActivatableWidget`，让页面通过类默认值声明输入行为。

参考实现：

- `D:\LyraStarterGame\Source\LyraGame\UI\LyraActivatableWidget.h`
- `D:\LyraStarterGame\Source\LyraGame\UI\LyraActivatableWidget.cpp`

## 项目实现

`ELyraUIWidgetInputMode` 提供四种模式：

- `Default`：当前页面不提供配置；Action Router 继续检查 Action Domain、激活树父节点，最后按 CommonUI 全局设置决定是否使用系统默认配置。
- `GameAndMenu`：游戏与 UI 都可接收输入，使用页面配置的鼠标捕获方式。
- `Game`：以游戏输入为主，使用页面配置的鼠标捕获方式。
- `Menu`：菜单输入，强制 `NoCapture`。

`ULyraUIActivatableWidget::GetDesiredInputConfig()` 将上述枚举转换为 CommonUI 的 `FUIInputConfig`。Action Router 会在页面成为有效激活叶节点时应用它，并在页面退出时恢复下层页面的配置。

## SmokeTest 迁移

重新打开编辑器并编译蓝图后：

1. 打开 `/Game/UI/Dev/WBP_CommonUI_SmokeTest`。
2. 在 Class Defaults 中将 `Input Config` 设置为 `Menu`。
3. 删除蓝图覆写的 `Get Desired Input Config` 函数。
4. 保留 `Get Desired Focus Target -> Button_First`。
5. 编译并保存。

## 回归验证

- PIE 启动后鼠标显示且不被捕获。
- `Button_First` 自动获得焦点。
- Enter/Space 和 Up/Down 行为不变。
- Escape 仍关闭 SmokeTest。
- 日志显示 Input Mode 为 Menu、Mouse Capture 为 NoCapture。

## 2026-09-23 实测结果

- SmokeTest 类默认值已选择 `Input Config = Menu`。
- 临时蓝图 `Get Desired Input Config` 覆写已移除，输入行为改由项目 C++ 基类提供。
- PIE 中鼠标保持显示，点击按钮后不会被游戏视口捕获。
- 页面激活时 `Button_First` 获得焦点，Enter/Space 与 Up/Down 行为正常。
- 鼠标点击空白区域后按钮失去焦点，此时 Enter/Space 不触发按钮；这是按键确认依赖当前 Slate 焦点的正常行为。
- Escape 仍能关闭栈顶页面，回归验证通过。

## 为什么保留 Default

并非每个 Activatable Widget 都应该覆盖输入配置。复合页面中的辅助 Activatable Widget 可以选择 `Default`，让配置继续由 Action Domain、上层主页面或 CommonUI 的系统默认值决定，避免激活一个局部控件时意外改变整个 PlayerController 的输入状态。
