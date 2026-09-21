# 接手状态

最后更新：2026-09-21

## 当前阶段

阶段 0：项目基线与学习路线已建立。

## 已确认事实

- 学习项目：`D:\Dev\MyProject\LyraUI`
- 参考项目：`D:\LyraStarterGame`
- 两者均关联 UE `5.8`。
- `LyraUI` 是一个几乎空白的 C++ 项目，运行时模块名为 `LyraUI`。
- 项目已经启用 Enhanced Input，但尚未在 `.uproject` 中显式启用 CommonUI。
- `LyraUI.Build.cs` 尚未依赖 `CommonUI`、`CommonInput`、`UMG`、`Slate`、`SlateCore`。
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

执行 [01_PROJECT_BASELINE.md](01_PROJECT_BASELINE.md) 中的“实验 1：启用 CommonUI 并验证编译环境”。

本实验完成标准：

- CommonUI 插件已启用。
- C++ 模块可以引用 `UCommonActivatableWidget` 并成功编译。
- 编辑器重启后能创建以 `CommonActivatableWidget` 为父类的 Widget Blueprint。
- 记录实际使用的编辑器启动和编译方式。

## 后续会话接手流程

1. 阅读本文件和 `README.md`。
2. 检查项目实际文件与本文是否一致。
3. 只推进“下一步”中的实验。
4. 将操作、异常、验证结果写入 `notes/`，并更新本文件。

