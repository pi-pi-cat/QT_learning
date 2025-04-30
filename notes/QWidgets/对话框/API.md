# QDialog API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags()) ▸ 典型用途：创建模态或非模态对话框 ▸ 内存策略：父子对象自动回收，未指定父对象时需手动删除

◆ 工厂方法

- 无内置工厂方法，通常通过子类化定制对话框

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名            | 功能描述                            | 使用频率 |
| ------------------- | ----------------------------------- | -------- |
| int exec()          | 以模态方式显示对话框                | ★★★★★    |
| void open()         | 以非模态方式显示对话框              | ★★★★     |
| void setModal(bool) | 设置对话框模态性                    | ★★★      |
| void setResult(int) | 设置对话框返回结果码                | ★★★★     |
| int result() const  | 获取对话框返回结果                  | ★★★★     |
| void accept()       | 接受对话框（返回QDialog::Accepted） | ★★★★★    |
| void reject()       | 拒绝对话框（返回QDialog::Rejected） | ★★★★★    |
| void done(int)      | 完成对话框并返回结果码              | ★★★★     |

▨ 重写方法

| 方法                           | 触发场景               | 必须调用基类 |
| ------------------------------ | ---------------------- | ------------ |
| void accept()                  | 确认对话框内容时       | 通常需要     |
| void reject()                  | 取消对话框操作时       | 通常需要     |
| void done(int)                 | 自定义结果完成对话框时 | 通常需要     |
| void closeEvent(QCloseEvent*)  | 关闭对话框时           | 是           |
| void keyPressEvent(QKeyEvent*) | 键盘事件处理           | 视情况而定   |

3️⃣【信号与槽】 ◇ 输出信号

- accepted() → 当对话框被接受时发射（如点击"确定"按钮）
- rejected() → 当对话框被拒绝时发射（如点击"取消"按钮或关闭按钮）
- finished(int result) → 当对话框完成时发射，携带结果码

◇ 输入槽

- accept() → 接受对话框并关闭，返回Accepted(1)
- reject() → 拒绝对话框并关闭，返回Rejected(0)
- done(int) → 以指定结果码完成对话框并关闭
- open() → 以非模态方式打开对话框
- exec() → 以模态方式执行对话框

4️⃣【枚举属性】 ◇ 相关属性

| 属性名          | 类型               | 默认值       | 描述                 |
| --------------- | ------------------ | ------------ | -------------------- |
| modal           | bool               | false        | 对话框是否为模态     |
| windowModality  | Qt::WindowModality | Qt::NonModal | 窗口模态级别         |
| sizeGripEnabled | bool               | false        | 是否显示尺寸调整手柄 |

◇ 相关枚举

- **QDialog::DialogCode**

- 作用：定义对话框返回的结果码
- 涉及函数：exec(), result(), done(), setResult()

| 成员     | 值   | 说明                 |
| -------- | ---- | -------------------- |
| Accepted | 1    | 对话框被接受（确定） |
| Rejected | 0    | 对话框被拒绝（取消） |

5️⃣【版本适配】 △ Qt 5.12特性

- 继承自QWidget的高DPI支持改进
- 非模态对话框性能优化

△ 跨平台差异

- Windows：对话框默认具有系统菜单和标题栏
- macOS：遵循HIG设计规范，对话框样式更轻量
- Linux：根据桌面环境（KDE/GNOME）风格不同

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 模态对话框
QDialog dialog(this);
dialog.setWindowTitle("模态对话框");
QVBoxLayout *layout = new QVBoxLayout(&dialog);
QPushButton *okButton = new QPushButton("确定", &dialog);
layout->addWidget(okButton);
connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
if (dialog.exec() == QDialog::Accepted) {
    // 用户点击了确定
}

// 非模态对话框
QDialog *dialog = new QDialog(this);
dialog->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动删除
dialog->setWindowTitle("非模态对话框");
dialog->show(); // 或 dialog->open();
```

◇ 危险操作：

```cpp
// 错误：主线程阻塞期间在其他线程中操作对话框
QDialog *dialog = new QDialog();
std::thread t([dialog]() {
    dialog->show(); // 错误：跨线程GUI操作
});

// 正确：在主线程创建和显示对话框
QDialog *dialog = new QDialog();
dialog->show();
// 使用信号槽机制在线程间通信
```





# QMessageBox API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QMessageBox(QWidget *parent = nullptr) ▸ 典型用途：创建空白消息框后设置属性 ▸ 内存策略：父子对象自动回收
- QMessageBox(Icon icon, const QString &title, const QString &text, StandardButtons buttons = NoButton, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint) ▸ 典型用途：一步完成所有配置 ▸ 示例：QMessageBox(QMessageBox::Warning, "警告", "文件未保存", QMessageBox::Save | QMessageBox::Discard, this)

◆ 工厂方法（静态便捷函数）

- static StandardButton information(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton) ▸ 返回值：用户选择的按钮
- static StandardButton question(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons = StandardButtons(Yes | No), StandardButton defaultButton = NoButton) ▸ 返回值：用户选择的按钮
- static StandardButton warning(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton) ▸ 返回值：用户选择的按钮
- static StandardButton critical(QWidget *parent, const QString &title, const QString &text, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton) ▸ 返回值：用户选择的按钮
- static void about(QWidget *parent, const QString &title, const QString &text) ▸ 典型用途：显示应用程序关于信息
- static void aboutQt(QWidget *parent, const QString &title = QString()) ▸ 典型用途：显示Qt相关信息

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                                | 功能描述             | 使用频率 |
| ------------------------------------------------------- | -------------------- | -------- |
| void setText(const QString &)                           | 设置主要消息文本     | ★★★★★    |
| void setInformativeText(const QString &)                | 设置补充说明文本     | ★★★★     |
| void setDetailedText(const QString &)                   | 设置可展开的详细文本 | ★★★      |
| void setIcon(Icon)                                      | 设置消息框图标       | ★★★★     |
| void setIconPixmap(const QPixmap &)                     | s设置自定义图标      | ★★       |
| void setStandardButtons(StandardButtons)                | 设置标准按钮组合     | ★★★★★    |
| void setDefaultButton(StandardButton)                   | 设置默认按钮         | ★★★★     |
| QAbstractButton *addButton(const QString &, ButtonRole) | 添加自定义按钮       | ★★★      |
| QPushButton *addButton(StandardButton)                  | 添加标准按钮         | ★★★      |
| QPushButton *button(StandardButton) const               | 获取标准按钮指针     | ★★★      |
| QAbstractButton *clickedButton() const                  | 获取被点击的按钮     | ★★★      |
| QString buttonText(int) const                           | 获取指定按钮文本     | ★★       |
| void setButtonText(int, const QString &)                | 设置按钮文本         | ★★       |

▨ 重写方法

| 方法                            | 触发场景       | 必须调用基类 |
| ------------------------------- | -------------- | ------------ |
| bool event(QEvent*)             | 任何事件发生时 | 通常需要     |
| void resizeEvent(QResizeEvent*) | 尺寸变化时     | 是           |
| void showEvent(QShowEvent*)     | 显示消息框前   | 是           |
| void closeEvent(QCloseEvent*)   | 关闭消息框时   | 是           |

3️⃣【信号与槽】 ◇ 输出信号

- buttonClicked(QAbstractButton *button) → 当任意按钮被点击时发射

◇ 输入槽

- 继承自QDialog的槽：accept(), reject(), done(int)

4️⃣【枚举属性】 ◇ 相关属性

| 属性名          | 类型            | 默认值       | 描述             |
| --------------- | --------------- | ------------ | ---------------- |
| text            | QString         | 空字符串     | 主消息文本       |
| icon            | Icon            | NoIcon       | 消息框图标       |
| iconPixmap      | QPixmap         | 空像素图     | 自定义图标       |
| textFormat      | Qt::TextFormat  | Qt::AutoText | 文本格式         |
| standardButtons | StandardButtons | NoButton     | 标准按钮组合     |
| defaultButton   | QPushButton*    | nullptr      | 默认按钮         |
| informativeText | QString         | 空字符串     | 补充说明文本     |
| detailedText    | QString         | 空字符串     | 可展开的详细文本 |

◇ 相关枚举

- **QMessageBox::Icon**

- 作用：定义消息框显示的图标类型
- 涉及函数：setIcon(), icon()

| 成员        | 值   | 说明          |
| ----------- | ---- | ------------- |
| NoIcon      | 0    | 无图标        |
| Information | 1    | 信息图标（i） |
| Warning     | 2    | 警告图标（!） |
| Critical    | 3    | 错误图标（x） |
| Question    | 4    | 问题图标（?） |

- **QMessageBox::StandardButton**

- 作用：定义标准按钮类型
- 涉及函数：setStandardButtons(), standardButtons()

| 成员    | 值         | 说明     |
| ------- | ---------- | -------- |
| Ok      | 0x00000400 | 确定按钮 |
| Open    | 0x00002000 | 打开按钮 |
| Save    | 0x00000800 | 保存按钮 |
| Cancel  | 0x00400000 | 取消按钮 |
| Close   | 0x00200000 | 关闭按钮 |
| Discard | 0x00800000 | 丢弃按钮 |
| Apply   | 0x02000000 | 应用按钮 |
| Reset   | 0x04000000 | 重置按钮 |
| Yes     | 0x00004000 | 是按钮   |
| No      | 0x00010000 | 否按钮   |
| Abort   | 0x00040000 | 中止按钮 |
| Retry   | 0x00080000 | 重试按钮 |
| Ignore  | 0x00100000 | 忽略按钮 |

5️⃣【版本适配】 △ Qt 5.12特性

- 高DPI支持改进
- 按钮内容换行显示能力增强
- 改进RTL语言支持

△ 跨平台差异

- Windows：按钮顺序遵循Windows设计指南（确定/取消在右侧）
- macOS：按钮顺序遵循macOS HIG（取消/确定顺序）
- Linux：按钮顺序取决于桌面环境（KDE/GNOME）

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 使用静态便捷方法（最常用）
int ret = QMessageBox::question(this, "确认", "是否保存更改?",
                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
if (ret == QMessageBox::Save) {
    saveFile();
} else if (ret == QMessageBox::Discard) {
    loadNewFile();
}

// 使用实例方法（高度自定义）
QMessageBox msgBox(this);
msgBox.setWindowTitle("操作完成");
msgBox.setText("处理已完成。");
msgBox.setInformativeText("共处理42个文件。");
msgBox.setDetailedText("处理的文件列表：\nfile1.txt\nfile2.txt\n...");
msgBox.setIcon(QMessageBox::Information);
msgBox.setStandardButtons(QMessageBox::Ok);
msgBox.setDefaultButton(QMessageBox::Ok);
msgBox.exec();
```

◇ 危险操作：

```cpp
// 错误：在非GUI线程中显示消息框
std::thread worker([]{
    // 错误：UI操作必须在主线程中进行
    QMessageBox::warning(nullptr, "错误", "在线程中调用");
});

// 正确：使用信号槽在主线程中显示
connect(workerObject, &Worker::errorOccurred, this, [this](const QString &msg){
    QMessageBox::critical(this, "错误", msg);
});

// 错误：忽略返回值可能导致逻辑错误
QMessageBox::question(this, "确认", "是否继续?",
                QMessageBox::Yes | QMessageBox::No);
// 正确：捕获并处理返回值
if (QMessageBox::question(this, "确认", "是否继续?", 
      QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
    proceedWithOperation();
}
```