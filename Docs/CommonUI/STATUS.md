# 接手状态

最后更新：2026-09-21

## 当前阶段

阶段 1 / 实验 1：代码与编译验证已完成，等待编辑器内资产验证。

## 已确认事实

- 学习项目：`D:\Dev\MyProject\LyraUI`
- 参考项目：`D:\LyraStarterGame`
- 两者均关联 UE `5.8`。
- `LyraUI` 是一个几乎空白的 C++ 项目，运行时模块名为 `LyraUI`。
- 项目已在 `.uproject` 中显式启用 CommonUI。
- `LyraUI.Build.cs` 已直接公开依赖 `CommonUI` 和 `UMG`。
- 已添加项目自有父类 `ULyraUIActivatableWidget`。
- `LyraUIEditor Win64 Development` 已通过 UnrealBuildTool 编译。
- `DefaultGame.ini` 已有一小段 CommonUI 配置，但这不等价于启用插件或完成输入数据配置。
- 当前没有项目级 `AGENTS.md`。
- 当前工作目录没有可用的 Git 仓库状态输出；开始大量修改前建议建立版本控制。

## 当前结论

先做 CommonUI 自身的最小闭环，不立即引入 Lyra 的以下部分：

- Experience / GameFeature 驱动的 UI 注入
- `CommonGame` 的 `PrimaryGameLayout` 和 UI Policy
- 登录、在线用户和完整设置系统
- Loading Screen、Messaging、UIExtension

这些内容会在掌握 Activatable Widget、Stack、输入与焦点后逐层加入。

## 下一步（只做这一项）

打开编辑器，执行实验 1 的资产验证：在 `/Game/UI/Dev/` 创建 `WBP_CommonUI_SmokeTest`，父类选择 `LyraUIActivatableWidget`，然后编译并保存。

本实验完成标准：

- 编辑器能搜索到 `LyraUIActivatableWidget` 父类。
- Widget Blueprint 可以编译并保存。
- Output Log 没有 CommonUI/CommonInput 加载错误。

## 后续会话接手流程

1. 阅读本文件和 `README.md`。
2. 检查项目实际文件与本文是否一致。
3. 只推进“下一步”中的实验。
4. 将操作、异常、验证结果写入 `notes/`，并更新本文件。
