# 接手状态

最后更新：2026-09-24

## 当前阶段

阶段 1 已完成：最小 Stack、Back、输入配置、默认焦点、基础导航和双页面 Push/返回均已通过键鼠验证；实体手柄验证待有设备时补做。下一步进入阶段 2（输入提示与跨设备切换）。

## 已确认事实

- 学习项目：`D:\Dev\MyProject\LyraUI`
- 参考项目：`D:\LyraStarterGame`
- 两者均关联 UE `5.8`。
- `LyraUI` 是一个几乎空白的 C++ 项目，运行时模块名为 `LyraUI`。
- 项目已在 `.uproject` 中显式启用 CommonUI。
- `LyraUI.Build.cs` 已直接公开依赖 `CommonUI` 和 `UMG`。
- 已添加项目自有父类 `ULyraUIActivatableWidget`。
- `LyraUIEditor Win64 Development` 已通过 UnrealBuildTool 编译。
- 已创建并保存 `/Game/UI/Dev/WBP_CommonUI_SmokeTest`，父类为项目自有的 Activatable Widget 基类。
- 测试资产已由 Git LFS 管理。
- `DefaultGame.ini` 已有一小段 CommonUI 配置，但这不等价于启用插件或完成输入数据配置。
- 当前没有项目级 `AGENTS.md`。
- Git 仓库已建立，并跟踪 GitHub 远端 `PEACE02/LyraUI`。
- 已创建 `/Game/Maps/L_CommonUITest`，并将其设为 `EditorStartupMap` 和 `GameDefaultMap`。
- 已创建 `/Game/UI/Dev/WBP_CommonUI_RootLayout`，其中包含 `ScreenStack`。
- 测试页面由 `ScreenStack -> Push Widget` 创建和管理，根布局是唯一执行 `Add to Viewport` 的 Widget。
- 已使用 `CommonGameViewportClient`，并配置项目自有的 Common Input Data 和 Back Action。
- PIE 实测：`Escape` 关闭栈顶 SmokeTest 页面；`Shift+Escape` 停止 PIE。
- SmokeTest 已实现 `Get Desired Focus Target`，默认聚焦 `Button_First`。
- `ULyraUIActivatableWidget` 已提供 `Default / GameAndMenu / Game / Menu` 类默认输入模式，并覆盖 `GetDesiredInputConfig()`。
- SmokeTest 已使用类默认值 `Input Config = Menu`，临时蓝图 Input Config 覆写已移除。
- PIE 实测：Enter/Space 可确认，Up/Down 可在两个普通 UMG Button 间导航，鼠标点击不会被视口捕获。
- PIE 实测：点击空白区域后按钮失焦，Enter/Space 不再触发按钮；再次聚焦按钮后行为恢复，符合 Slate 焦点规则。
- 已创建 `/Game/UI/Dev/WBP_CommonUI_SecondScreen`；SmokeTest 通过事件分发器请求导航，RootLayout 将第二页 Push 到同一个 ScreenStack。
- 双页面实测：Push 后旧页面 Deactivated 并渐隐，新页面 Activated 并渐显；第二页退出后原 SmokeTest 实例重新 Activated。
- SmokeTest 返回后再次使用 `Get Desired Focus Target`，焦点落到 `Button_First`，Enter 打印 `First Clicked`。

## 当前结论

先做 CommonUI 自身的最小闭环，不立即引入 Lyra 的以下部分：

- Experience / GameFeature 驱动的 UI 注入
- `CommonGame` 的 `PrimaryGameLayout` 和 UI Policy
- 登录、在线用户和完整设置系统
- Loading Screen、Messaging、UIExtension

这些内容会在掌握 Activatable Widget、Stack、输入与焦点后逐层加入。

## 下一步（只做这一项）

进入阶段 2：在测试页面加入 `CommonActionWidget`，先显示 Back Action 的当前按键提示，再补齐 Controller Data，观察键鼠与手柄输入类型变化时提示图标如何刷新。

## 后续会话接手流程

1. 阅读本文件和 `README.md`。
2. 检查项目实际文件与本文是否一致。
3. 只推进“下一步”中的实验。
4. 将操作、异常、验证结果写入 `notes/`，并更新本文件。
