# 实验记录：插件与首次构建

日期：2026-09-21

## 改动

- 在 `LyraUI.uproject` 中启用 `CommonUI`。
- 在 `LyraUI.Build.cs` 的公共依赖中加入 `CommonUI` 和 `UMG`。
- 新建 `Source/LyraUI/UI/LyraUIActivatableWidget.h`。
- `ULyraUIActivatableWidget` 是抽象、可蓝图继承的最小项目父类，暂不添加额外行为。

## 构建方式

```powershell
& 'C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat' `
  LyraUIEditor Win64 Development `
  '-Project=D:\Dev\MyProject\LyraUI\LyraUI.uproject' `
  -WaitMutex -NoHotReloadFromIDE
```

## 遇到的问题

第一次构建只声明了 `CommonUI` 依赖。代码编译通过，但链接阶段报告大量来自 `UWidget` 和 `UUserWidget` 的未解析符号。

原因：项目公开类的继承链暴露了 UMG 类型，当前模块需要直接链接 `UMG`，不能依赖 `CommonUI` 间接带入。

修复：将 `UMG` 加入 `PublicDependencyModuleNames`。

## 验证结果

- UnrealHeaderTool 成功生成 `ULyraUIActivatableWidget` 反射代码。
- `UnrealEditor-LyraUI.dll` 链接成功。
- UnrealBuildTool 最终结果：`Succeeded`。
- 已在编辑器中创建、编译并保存 `/Game/UI/Dev/WBP_CommonUI_SmokeTest`。
- 资产验证日志未报告该蓝图错误。
- `.uasset` 已匹配 Git LFS 规则。

## 路径知识点

项目的物理目录 `Content` 在 Unreal 资产系统中挂载为虚拟根路径 `/Game`：

```text
D:\Dev\MyProject\LyraUI\Content\UI\Dev\WBP_CommonUI_SmokeTest.uasset
                            ↓
/Game/UI/Dev/WBP_CommonUI_SmokeTest
```

因此不应在磁盘 `Content` 下再创建一个名为 `Game` 的目录，否则虚拟路径会变成 `/Game/Game/...`。
