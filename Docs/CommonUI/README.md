# CommonUI 学习工程

本目录是 `LyraUI` 项目的长期学习记录，也是新会话或更换模型后的接手入口。

## 当前目标

用一个尽量小的 UE 5.8 项目逐步掌握 CommonUI，并把 Lyra 当作架构参考，而不是直接复制整套 Lyra。

第一条最小闭环：

```text
启用 CommonUI
  -> 创建 CommonActivatableWidget 页面
  -> 页面进入/退出 Activatable Widget Stack
  -> 验证焦点、输入模式和 Back 行为
  -> 验证键鼠/手柄输入方式切换
```

## 文档导航

- [STATUS.md](STATUS.md)：当前进度、下一步和接手说明。新会话先读这里。
- [ROADMAP.md](ROADMAP.md)：完整学习路线以及每阶段的验收目标。
- [01_PROJECT_BASELINE.md](01_PROJECT_BASELINE.md)：项目现状、Lyra 对照和第一步实操。
- [02_MINIMAL_STACK.md](02_MINIMAL_STACK.md)：最小页面栈与激活生命周期实验。
- `notes/`：后续每个实验的过程、问题与结论。
- `decisions/`：后续影响架构的选择及理由。

## 协作约定

1. 每次只推进一个可运行、可验证的小闭环。
2. 完成实验后同步更新 `STATUS.md`，不要只依赖聊天上下文。
3. 新增代码或资产时记录路径、父类、关键设置和验收结果。
4. Lyra 源码只作为只读参考：`D:\LyraStarterGame`。
5. 不直接迁移 Lyra 资产；先理解最小机制，再决定是否引入 `CommonGame`、`UIExtension` 或 Game Features。
6. `.uasset` 的编辑器操作由学习者实际完成；文档记录精确点击路径与最终配置。

## 学习方法

每一步都回答四个问题：

- 它解决什么问题？
- 不使用它时会怎样？
- 它在运行时由谁创建、持有和销毁？
- 如何通过日志、Widget Reflector 或实际输入验证？
