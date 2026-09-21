# 01：项目基线与第一个实验

## 为什么第一步不是复制 Lyra

Lyra 的 UI 并不是只有 CommonUI。它还组合了 CommonGame、Game Features、Experience、UIExtension、CommonUser、设置系统和平台配置。如果从最终形态反向复制，很难区分哪部分是 CommonUI 的核心机制，哪部分是 Lyra 的产品架构。

因此，第一个实验只验证三件事：插件可用、C++ 能链接、编辑器能创建 CommonUI Widget。

## 本项目现状

`LyraUI.uproject` 当前仅显式启用了 `ModelingToolsEditorMode`。`Source/LyraUI/LyraUI.Build.cs` 当前公共依赖包括 `Core`、`CoreUObject`、`Engine`、`InputCore`、`EnhancedInput`。

Lyra 的运行时模块则包含 `UMG`、`CommonUI`、`CommonInput`、`Slate`、`SlateCore` 等依赖。我们第一阶段只添加实际使用所需的最小集合，不照抄 Lyra 的全部依赖。

## 实验 1：启用 CommonUI 并验证编译环境

### 1. 建议先建立版本控制

当前没有检测到有效 Git 状态。推荐先在项目根目录初始化 Git，并忽略至少这些生成目录：

```text
.vs/
Binaries/
DerivedDataCache/
Intermediate/
Saved/
```

是否使用 Git 由学习者决定；不影响 CommonUI 实验，但它能让后续配置和 C++ 变更易于回退。

### 2. 在编辑器启用插件

打开项目后执行：

```text
Edit -> Plugins -> 搜索 Common UI -> 勾选 Enabled -> 重启编辑器
```

重启后，`LyraUI.uproject` 应出现类似配置：

```json
{
  "Name": "CommonUI",
  "Enabled": true
}
```

CommonInput 是 CommonUI 使用的输入抽象。若编辑器提示依赖插件，将依赖一同启用；最终以 `.uproject` 实际记录为准。

### 3. 补齐模块依赖

本项目的公开头文件声明了 `ULyraUIActivatableWidget`。由于它继承 `UCommonActivatableWidget`，继承链又包含 `UUserWidget`，所以直接公开依赖必须包含 `CommonUI` 和 `UMG`：

```csharp
PublicDependencyModuleNames.AddRange(new string[]
{
    "UMG",
    "CommonUI"
});
```

`CommonInput`、`Slate` 和 `SlateCore` 会在代码直接使用它们时再添加，避免为了模仿 Lyra 而提前引入没有使用的依赖。

### 4. 重新生成并编译

关闭编辑器，重新生成项目文件并构建 `LyraUIEditor Development Editor Win64`。记录你最终使用的方式：Visual Studio、右键 `.uproject`，或 Unreal Build Tool 命令行。

### 5. 编辑器内验证

重启编辑器，在 Content Browser 新建 Widget Blueprint。父类搜索项目自有的 `LyraUIActivatableWidget`。

成功标准：

- 能找到该父类；
- Widget Blueprint 能打开和编译；
- Output Log 没有 CommonUI/CommonInput 加载错误。

这个临时资产可以命名为 `WBP_CommonUI_SmokeTest`，建议放在 `/Game/UI/Dev/`。

## 本次构建结论

第一次只加入 `CommonUI` 时，C++ 编译成功但链接失败，出现大量 `UUserWidget` 未解析符号。将 `UMG` 加为直接公共依赖后链接成功。这说明模块不能只依赖插件的传递依赖；公开继承链涉及的模块应由当前模块直接声明。

## 暂时不要做的事

- 不要迁移 Lyra 的 `/Game/UI` 整个目录。
- 不要先实现复杂主菜单或设置页。
- 不要用 `Add to Viewport` 堆叠多个可激活页面来假装页面栈。
- 不要跳过默认焦点和 Back 的验证；这两项是 CommonUI 初学时最常见的问题源头。

## 完成后要记录

在 `Docs/CommonUI/notes/01-plugin-and-build.md` 写下：

- `.uproject` 实际新增的插件；
- `Build.cs` 实际依赖；
- 使用的编译命令或 IDE 配置；
- 是否成功创建 `WBP_CommonUI_SmokeTest`；
- 所有错误及解决办法。

然后更新 `STATUS.md`，把下一步改为“创建最小 Activatable Widget Stack”。
