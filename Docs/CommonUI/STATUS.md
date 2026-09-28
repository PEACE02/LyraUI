# 接手状态

最后更新：2026-09-28

## 当前阶段

阶段 1 的页面栈、输入与焦点闭环以及阶段 2 的键鼠操作提示均已完成；Xbox Series X Controller Data 已配置，实体手柄验收暂缓。当前进入阶段 3：建立项目自己的 `CommonButtonBase` 与集中式按钮样式。

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
- 鼠标点击不会被视口捕获；键鼠模式按 Lyra 的默认取舍主要使用鼠标操作。
- 已创建 `/Game/UI/Dev/WBP_CommonUI_SecondScreen`；SmokeTest 通过事件分发器请求导航，RootLayout 将第二页 Push 到同一个 ScreenStack。
- 双页面实测：Push 后旧页面 Deactivated 并渐隐，新页面 Activated 并渐显；第二页退出后原 SmokeTest 实例重新 Activated。
- SmokeTest 返回后再次使用 `Get Desired Focus Target`，此前已通过日志及键盘确认实验验证焦点会恢复到 `Button_First`。
- 已添加最小项目按钮基类 `ULyraUIButtonBase`；它保留 CommonButtonBase 的交互职责，只统一按钮文字和当前 Text Style。
- 已创建三种按钮 `CommonTextStyle`、`ButtonStyle_CommonUI_Primary` 和 `WBP_CommonUI_TextButton`，并替换 SmokeTest 的两个普通 UMG Button。
- 已修复快速 Enter 重复发送打开请求导致同一 Stack 重复 Push 两个 SecondScreen 的问题；当前由导航按钮在请求后锁定交互，并在页面重新 Activated 时恢复。
- 通用按钮的 Focus 表现采用 Lyra 取舍：鼠标真实 Hover，手柄焦点通过虚拟光标复用 Hover；纯键盘不增加 Focused Style，Selected 保留其业务语义。
- 已移除 `CommonButtonAcceptKeyHandling=TriggerClick`；CommonButton 恢复 Lyra 使用的 `Ignore` 默认行为，键鼠不以 Enter/Space 直接确认焦点按钮。
- `ButtonStyle_CommonUI_Primary` 的 Disabled Brush / Disabled Text Style 已在 PIE 验证：禁用按钮不可 Hover、不可点击，外观正确切换。

## 当前结论

先做 CommonUI 自身的最小闭环，不立即引入 Lyra 的以下部分：

- Experience / GameFeature 驱动的 UI 注入
- `CommonGame` 的 `PrimaryGameLayout` 和 UI Policy
- 登录、在线用户和完整设置系统
- Loading Screen、Messaging、UIExtension

这些内容会在掌握 Activatable Widget、Stack、输入与焦点后逐层加入。

## 下一步（只做这一项）

继续阶段 3：不再拆分孤立小实验，直接实现完整的“设置中心”模块；用 TabList、Switcher 和 List 一次覆盖 Button Group、Selected、Disabled、页面切换、Back 与焦点恢复。

## 后续会话接手流程

1. 阅读本文件和 `README.md`。
2. 检查项目实际文件与本文是否一致。
3. 只推进“下一步”中的实验。
4. 将操作、异常、验证结果写入 `notes/`，并更新本文件。
