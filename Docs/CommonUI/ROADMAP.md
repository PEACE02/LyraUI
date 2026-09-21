# CommonUI 学习路线

路线按依赖关系设计。每一阶段都必须有可见结果和验收动作，不以“看过源码”作为完成标准。

## 阶段 0：基线与可恢复记录

目标：确认 UE 版本、项目状态、Lyra 参考范围，并建立文档入口。

产物：本目录中的学习路线与接手状态。

## 阶段 1：CommonUI 最小可运行闭环

目标：理解 CommonUI 最基础的运行时对象。

实验：

1. 启用 CommonUI，补齐 C++ 模块依赖。
2. 创建一个 `CommonActivatableWidget` 页面。
3. 创建一个含 `CommonActivatableWidgetStack` 的根 Widget。
4. 将页面 Push 到 Stack，并通过 Back 将其 Pop。
5. 为页面设置明确的默认焦点，验证键盘和手柄导航。

验收：能解释 Active/Inactive 与普通 Visibility 的区别；能观察到 Stack 的 Push/Pop；鼠标、键盘和手柄都不会丢失焦点。

## 阶段 2：输入提示与跨设备切换

目标：理解 CommonInput 如何检测输入方式并提供按键图标。

实验：配置 Input Data、Controller Data、Confirm/Back；显示 `CommonActionWidget`；在键鼠和手柄之间切换。

验收：提示图标随当前输入设备变化；Back 在页面栈中行为一致。

## 阶段 3：按钮、样式与导航

目标：建立可复用的按钮和样式层，不依赖散落的 Widget 属性覆盖。

实验：`CommonButtonBase`、Button Style、Text Style、List 与 Tab；验证 Disabled、Selected、Hovered、Focused 状态。

验收：菜单能仅用手柄完整操作，样式可通过数据资产统一替换。

## 阶段 4：Lyra 式输入配置

目标：理解 Lyra 为什么给 Activatable Widget 增加 Game、Menu、GameAndMenu 输入模式。

参考：

- `D:\LyraStarterGame\Source\LyraGame\UI\LyraActivatableWidget.h`
- `D:\LyraStarterGame\Source\LyraGame\UI\LyraActivatableWidget.cpp`

实验：在本项目实现一个简化版基类，覆盖 `GetDesiredInputConfig()`，分别制作暂停菜单与游戏 HUD 页面。

验收：每个页面激活时自动得到正确的鼠标捕获与输入路由，不靠 Level Blueprint 临时切换 Input Mode。

## 阶段 5：分层根布局

目标：建立 Game、GameMenu、Menu、Modal 等层，并理解层级与页面生命周期。

先手工实现最小根布局，再研究 Lyra 使用的 `CommonGame`：

- `PrimaryGameLayout`
- UI Policy / UI Manager Subsystem
- `UCommonUIExtensions::PushContentToLayer_ForPlayer`

验收：HUD、暂停菜单、模态对话框互不争抢错误层级；关闭上层后焦点正确恢复。

## 阶段 6：与游戏生命周期集成

目标：从测试关卡过渡到稳定的 GameInstance、LocalPlayer、PlayerController/HUD 生命周期。

实验：多人本地玩家边界、关卡切换、HUD 重建、输入设备断开与恢复。

验收：UI 不依赖一次性的 Level Blueprint，Travel 后不会重复创建或残留。

## 阶段 7：Lyra 高层架构选修

按项目需求逐项学习，而不是整套搬入：

- GameFeature / Experience 注入 HUD
- UIExtension 动态扩展点
- CommonGame 消息对话框
- CommonLoadingScreen
- 设置页面和输入重映射
- 平台 Traits 与平台专属 Controller Data

验收：能说明引入每个系统带来的收益、依赖与复杂度，并有实际业务需求支撑。

## 推荐的内容目录（逐步创建）

```text
Content/UI/
  Foundation/       # 输入数据、基础样式、基础控件
  Layout/           # 根布局与各层容器
  Screens/          # 可激活的全屏/菜单页面
  HUD/              # 游戏内常驻信息
  Dialogs/          # Modal 页面
  Dev/              # 学习用测试页面，成熟后可移除
```

不要在阶段 1 一次性创建全部目录和资产；目录随实验产物出现。

