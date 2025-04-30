# QButtonGroup API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QButtonGroup(QObject *parent = nullptr) ▸ 典型用途：创建按钮组管理器实例 ▸ 内存策略：父子对象自动回收 ▸ 特点：非可视组件，仅提供逻辑管理功能

◆ 工厂方法

- 无专用工厂方法

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                      | 功能描述           | 使用频率 |
| --------------------------------------------- | ------------------ | -------- |
| void addButton(QAbstractButton*, int id = -1) | 添加按钮并指定ID   | ★★★★★    |
| void removeButton(QAbstractButton*)           | 从组中移除按钮     | ★★★      |
| QList<QAbstractButton*> buttons() const       | 获取所有按钮列表   | ★★★      |
| QAbstractButton* button(int id) const         | 根据ID获取按钮指针 | ★★★★     |
| QAbstractButton* checkedButton() const        | 获取选中的按钮     | ★★★★     |
| int checkedId() const                         | 获取选中按钮的ID   | ★★★★     |
| int id(QAbstractButton*) const                | 获取按钮的ID       | ★★★      |
| void setId(QAbstractButton*, int id)          | 设置按钮的ID       | ★★★      |
| void setExclusive(bool)                       | 设置是否互斥       | ★★★★     |
| bool exclusive() const                        | 获取是否互斥       | ★★       |

▨ 重写方法

| 方法                | 触发场景     | 必须调用基类 |
| ------------------- | ------------ | ------------ |
| bool event(QEvent*) | 接收所有事件 | 是           |

3️⃣【信号与槽】 ◇ 输出信号

- buttonClicked(QAbstractButton*) → 按钮被点击时发射
- buttonClicked(int) → 按钮被点击时发射，参数为按钮ID
- buttonPressed(QAbstractButton*) → 按钮被按下时发射
- buttonPressed(int) → 按钮被按下时发射，参数为按钮ID
- buttonReleased(QAbstractButton*) → 按钮被释放时发射
- buttonReleased(int) → 按钮被释放时发射，参数为按钮ID
- buttonToggled(QAbstractButton*, bool) → 按钮状态改变时发射
- buttonToggled(int, bool) → 按钮状态改变时发射，参数为按钮ID和状态
- idClicked(int) → 按钮被点击时发射，参数为按钮ID
- idPressed(int) → 按钮被按下时发射，参数为按钮ID
- idReleased(int) → 按钮被释放时发射，参数为按钮ID
- idToggled(int, bool) → 按钮状态改变时发射，参数为按钮ID和状态

◇ 输入槽

- QButtonGroup没有专门定义的公共槽

4️⃣【版本适配】 △ Qt 5.12特性

- 保留了在Qt 5.2中引入的独立ID系统
- idClicked(int)、idPressed(int)、idReleased(int)和idToggled(int, bool)信号在Qt 5.8中引入

△ 跨平台差异

- QButtonGroup作为逻辑组件，在各平台表现一致
- 注意：管理的按钮在不同平台上可能存在视觉差异

5️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 创建单选按钮组
QButtonGroup *radioGroup = new QButtonGroup(this);
QRadioButton *option1 = new QRadioButton("选项1");
QRadioButton *option2 = new QRadioButton("选项2");
QRadioButton *option3 = new QRadioButton("选项3");

// 添加按钮到组并指定ID
radioGroup->addButton(option1, 1);
radioGroup->addButton(option2, 2);
radioGroup->addButton(option3, 3);
radioGroup->setExclusive(true);  // 设置互斥(默认已是true)

// 连接信号
connect(radioGroup, QOverload<int>::of(&QButtonGroup::buttonClicked),
        [=](int id) { 
            qDebug() << "选中了ID:" << id; 
        });

// 创建非互斥的按钮组(复选框)
QButtonGroup *checkGroup = new QButtonGroup(this);
checkGroup->setExclusive(false);  // 设置非互斥
```

◇ 危险操作：

```cpp
// 错误：未设置唯一ID导致冲突
QButtonGroup *group = new QButtonGroup(this);
group->addButton(new QRadioButton("按钮1"));  // 默认ID为-1
group->addButton(new QRadioButton("按钮2"));  // 默认ID为-1
// 问题：无法通过ID区分按钮

// 正确：为每个按钮设置唯一ID
group->addButton(new QRadioButton("按钮1"), 1);
group->addButton(new QRadioButton("按钮2"), 2);

// 错误：将按钮同时添加到多个按钮组
QRadioButton *radio = new QRadioButton("选项");
QButtonGroup *group1 = new QButtonGroup(this);
QButtonGroup *group2 = new QButtonGroup(this);
group1->addButton(radio);
group2->addButton(radio);  // 错误：一个按钮只应属于一个组
```







# QDialogButtonBox API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QDialogButtonBox(QWidget *parent = nullptr) ▸ 典型用途：创建空按钮盒，后续添加按钮 ▸ 内存策略：父子对象自动回收
- QDialogButtonBox(QDialogButtonBox::StandardButtons buttons, Qt::Orientation orientation = Qt::Horizontal, QWidget *parent = nullptr) ▸ 典型用途：一步创建包含标准按钮的按钮盒 ▸ 示例：QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel)
- QDialogButtonBox(Qt::Orientation orientation, QWidget *parent = nullptr) ▸ 典型用途：指定方向创建空按钮盒 ▸ 默认值：Qt::Horizontal（水平布局）

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                              | 功能描述                   | 使用频率 |
| ----------------------------------------------------- | -------------------------- | -------- |
| void addButton(QAbstractButton*, ButtonRole)          | 添加自定义按钮并指定角色   | ★★★★     |
| QPushButton* addButton(const QString&, ButtonRole)    | 创建并添加文本按钮         | ★★★      |
| QPushButton* addButton(StandardButton)                | 添加标准按钮               | ★★★★★    |
| QList<QAbstractButton*> buttons() const               | 获取所有按钮列表           | ★★       |
| QPushButton* button(StandardButton) const             | 获取标准按钮指针           | ★★★★     |
| ButtonRole buttonRole(QAbstractButton*) const         | 获取按钮角色               | ★★       |
| void clear()                                          | 移除所有按钮               | ★        |
| void setOrientation(Qt::Orientation)                  | 设置按钮排列方向           | ★★       |
| Qt::Orientation orientation() const                   | 获取当前排列方向           | ★        |
| void setStandardButtons(StandardButtons)              | 设置标准按钮组合           | ★★★★★    |
| StandardButtons standardButtons() const               | 获取当前标准按钮组合       | ★★       |
| StandardButton standardButton(QAbstractButton*) const | 获取按钮对应的标准按钮枚举 | ★★       |
| void setCenterButtons(bool)                           | 设置按钮是否居中显示       | ★★       |
| bool centerButtons() const                            | 获取按钮是否居中显示       | ★        |

▨ 重写方法

| 方法                      | 触发场景         | 必须调用基类 |
| ------------------------- | ---------------- | ------------ |
| void changeEvent(QEvent*) | 样式、语言变更时 | 是           |
| bool event(QEvent*)       | 接收所有事件     | 视情况而定   |

3️⃣【信号与槽】 ◇ 输出信号

- accepted() → 点击接受类按钮时发射（如Ok、Save、Yes）
- rejected() → 点击拒绝类按钮时发射（如Cancel、Close、No）
- clicked(QAbstractButton*) → 任何按钮被点击时发射，参数为被点击按钮
- helpRequested() → 点击帮助按钮时发射

◇ 输入槽

- QDialogButtonBox没有专门定义的公共槽，但可使用继承自QWidget的槽

4️⃣【版本适配】 △ Qt 5.12特性

- 继承了Qt 5.2引入的setCenterButtons()功能
- 在不同平台上更好地保持本地化按钮顺序

△ 跨平台差异

- Windows：标准按钮通常按照"确定-取消"的顺序排列在右侧
- macOS：标准按钮通常按照"取消-确定"的顺序排列在右侧
- Linux(KDE/GNOME)：按钮排列遵循平台HIG设计规范

5️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 创建包含"确定"和"取消"按钮的对话框
QDialog *dialog = new QDialog(this);
QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | 
                                                 QDialogButtonBox::Cancel);
connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

// 添加到布局
QVBoxLayout *layout = new QVBoxLayout;
layout->addWidget(contentWidget);
layout->addWidget(buttonBox);
dialog->setLayout(layout);
```

◇ 危险操作：

```cpp
// 错误：忽略按钮角色导致信号不触发
QPushButton *okButton = new QPushButton("确定");
buttonBox->addButton(okButton, QDialogButtonBox::InvalidRole); // 错误：使用无效角色

// 正确：使用适当的按钮角色
buttonBox->addButton(okButton, QDialogButtonBox::AcceptRole);

// 错误：动态创建后没有连接信号
auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
// 缺少连接信号到槽的步骤

// 正确：添加按钮后连接信号
connect(buttons, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
connect(buttons, &QDialogButtonBox::rejected, dialog, &QDialog::reject);
```



# QGroupBox API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QGroupBox(QWidget *parent = nullptr) ▸ 典型用途：创建无标题分组框 ▸ 内存策略：父子对象自动回收
- QGroupBox(const QString &title, QWidget *parent = nullptr) ▸ 典型用途：创建带标题的分组框 ▸ 示例：QGroupBox("选项设置", this)

◆ 工厂方法

- 无专用工厂方法

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                            | 功能描述               | 使用频率 |
| ----------------------------------- | ---------------------- | -------- |
| void setTitle(const QString &title) | 设置分组框标题         | ★★★★★    |
| QString title() const               | 获取分组框标题         | ★★★      |
| void setAlignment(int alignment)    | 设置标题对齐方式       | ★★★      |
| int alignment() const               | 获取标题对齐方式       | ★★       |
| void setCheckable(bool checkable)   | 设置分组框是否可选中   | ★★★★     |
| bool isCheckable() const            | 获取分组框是否可选中   | ★★       |
| void setChecked(bool checked)       | 设置分组框选中状态     | ★★★★     |
| bool isChecked() const              | 获取分组框选中状态     | ★★★      |
| void setFlat(bool flat)             | 设置分组框是否扁平显示 | ★★       |
| bool isFlat() const                 | 获取分组框是否扁平显示 | ★        |

▨ 重写方法

| 方法                                 | 触发场景     | 必须调用基类 |
| ------------------------------------ | ------------ | ------------ |
| void paintEvent(QPaintEvent*)        | 绘制分组框   | 视情况而定   |
| void focusInEvent(QFocusEvent*)      | 获取焦点时   | 是           |
| void changeEvent(QEvent*)            | 状态变更时   | 是           |
| bool event(QEvent*)                  | 接收所有事件 | 视情况而定   |
| void childEvent(QChildEvent*)        | 子控件事件   | 是           |
| void resizeEvent(QResizeEvent*)      | 大小改变时   | 是           |
| void mousePressEvent(QMouseEvent*)   | 鼠标按下时   | 是           |
| void mouseReleaseEvent(QMouseEvent*) | 鼠标释放时   | 是           |

3️⃣【信号与槽】 ◇ 输出信号

- clicked(bool checked = false) → 分组框被点击时发射
- toggled(bool on) → 分组框选中状态改变时发射

◇ 输入槽

- setChecked(bool) → 设置分组框选中状态

4️⃣【版本适配】 △ Qt 5.12特性

- 从Qt 5.2开始支持传入nullptr作为parent参数
- flat属性在所有平台上提供一致的扁平样式

△ 跨平台差异

- Windows：标题可能有不同的默认字体样式
- macOS：可能使用圆角矩形而非方角
- Linux：根据桌面环境，分组框样式可能略有不同

5️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 创建基本分组框
QGroupBox *groupBox = new QGroupBox("用户设置", this);
QVBoxLayout *vbox = new QVBoxLayout;
vbox->addWidget(new QCheckBox("自动登录"));
vbox->addWidget(new QCheckBox("记住密码"));
groupBox->setLayout(vbox);

// 创建可选中的分组框
QGroupBox *optionalGroup = new QGroupBox("高级选项", this);
optionalGroup->setCheckable(true);
optionalGroup->setChecked(false);
QVBoxLayout *optLayout = new QVBoxLayout;
optLayout->addWidget(new QRadioButton("选项1"));
optLayout->addWidget(new QRadioButton("选项2"));
optionalGroup->setLayout(optLayout);

// 连接信号
connect(optionalGroup, &QGroupBox::toggled, [=](bool checked) {
    qDebug() << "高级选项已" << (checked ? "启用" : "禁用");
});
```

◇ 危险操作：

```cpp
// 错误：没有为GroupBox设置布局
QGroupBox *group = new QGroupBox("选项", this);
new QCheckBox("选项1", group);  // 错误：没有设置布局
new QCheckBox("选项2", group);  // 子部件位置不确定

// 正确：设置布局管理器
QGroupBox *group = new QGroupBox("选项", this);
QVBoxLayout *layout = new QVBoxLayout(group);
layout->addWidget(new QCheckBox("选项1"));
layout->addWidget(new QCheckBox("选项2"));

// 错误：在可选中GroupBox上使用clicked信号
QGroupBox *group = new QGroupBox("选项");
group->setCheckable(true);
// 错误：clicked信号在点击时触发，不管是否改变了选中状态
connect(group, &QGroupBox::clicked, [](){ doSomething(); });

// 正确：使用toggled信号监听选中状态变化
connect(group, &QGroupBox::toggled, [](bool checked){ 
    if(checked) doSomething(); 
});
```







# QGroupBox API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QGroupBox(QWidget *parent = nullptr) ▸ 典型用途：创建无标题分组框 ▸ 内存策略：父子对象自动回收
- QGroupBox(const QString &title, QWidget *parent = nullptr) ▸ 典型用途：创建带标题的分组框 ▸ 示例：QGroupBox("选项设置", this)

◆ 工厂方法

- 无专用工厂方法

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                            | 功能描述               | 使用频率 |
| ----------------------------------- | ---------------------- | -------- |
| void setTitle(const QString &title) | 设置分组框标题         | ★★★★★    |
| QString title() const               | 获取分组框标题         | ★★★      |
| void setAlignment(int alignment)    | 设置标题对齐方式       | ★★★      |
| int alignment() const               | 获取标题对齐方式       | ★★       |
| void setCheckable(bool checkable)   | 设置分组框是否可选中   | ★★★★     |
| bool isCheckable() const            | 获取分组框是否可选中   | ★★       |
| void setChecked(bool checked)       | 设置分组框选中状态     | ★★★★     |
| bool isChecked() const              | 获取分组框选中状态     | ★★★      |
| void setFlat(bool flat)             | 设置分组框是否扁平显示 | ★★       |
| bool isFlat() const                 | 获取分组框是否扁平显示 | ★        |

▨ 重写方法

| 方法                                 | 触发场景     | 必须调用基类 |
| ------------------------------------ | ------------ | ------------ |
| void paintEvent(QPaintEvent*)        | 绘制分组框   | 视情况而定   |
| void focusInEvent(QFocusEvent*)      | 获取焦点时   | 是           |
| void changeEvent(QEvent*)            | 状态变更时   | 是           |
| bool event(QEvent*)                  | 接收所有事件 | 视情况而定   |
| void childEvent(QChildEvent*)        | 子控件事件   | 是           |
| void resizeEvent(QResizeEvent*)      | 大小改变时   | 是           |
| void mousePressEvent(QMouseEvent*)   | 鼠标按下时   | 是           |
| void mouseReleaseEvent(QMouseEvent*) | 鼠标释放时   | 是           |

3️⃣【信号与槽】 ◇ 输出信号

- clicked(bool checked = false) → 分组框被点击时发射
- toggled(bool on) → 分组框选中状态改变时发射

◇ 输入槽

- setChecked(bool) → 设置分组框选中状态

4️⃣【版本适配】 △ Qt 5.12特性

- 从Qt 5.2开始支持传入nullptr作为parent参数
- flat属性在所有平台上提供一致的扁平样式

△ 跨平台差异

- Windows：标题可能有不同的默认字体样式
- macOS：可能使用圆角矩形而非方角
- Linux：根据桌面环境，分组框样式可能略有不同

5️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 创建基本分组框
QGroupBox *groupBox = new QGroupBox("用户设置", this);
QVBoxLayout *vbox = new QVBoxLayout;
vbox->addWidget(new QCheckBox("自动登录"));
vbox->addWidget(new QCheckBox("记住密码"));
groupBox->setLayout(vbox);

// 创建可选中的分组框
QGroupBox *optionalGroup = new QGroupBox("高级选项", this);
optionalGroup->setCheckable(true);
optionalGroup->setChecked(false);
QVBoxLayout *optLayout = new QVBoxLayout;
optLayout->addWidget(new QRadioButton("选项1"));
optLayout->addWidget(new QRadioButton("选项2"));
optionalGroup->setLayout(optLayout);

// 连接信号
connect(optionalGroup, &QGroupBox::toggled, [=](bool checked) {
    qDebug() << "高级选项已" << (checked ? "启用" : "禁用");
});
```

◇ 危险操作：

```cpp
// 错误：没有为GroupBox设置布局
QGroupBox *group = new QGroupBox("选项", this);
new QCheckBox("选项1", group);  // 错误：没有设置布局
new QCheckBox("选项2", group);  // 子部件位置不确定

// 正确：设置布局管理器
QGroupBox *group = new QGroupBox("选项", this);
QVBoxLayout *layout = new QVBoxLayout(group);
layout->addWidget(new QCheckBox("选项1"));
layout->addWidget(new QCheckBox("选项2"));

// 错误：在可选中GroupBox上使用clicked信号
QGroupBox *group = new QGroupBox("选项");
group->setCheckable(true);
// 错误：clicked信号在点击时触发，不管是否改变了选中状态
connect(group, &QGroupBox::clicked, [](){ doSomething(); });

// 正确：使用toggled信号监听选中状态变化
connect(group, &QGroupBox::toggled, [](bool checked){ 
    if(checked) doSomething(); 
});
```





# QFrame API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QFrame(QWidget *parent = nullptr) ▸ 典型用途：创建基础框架控件 ▸ 内存策略：父子对象自动回收

◆ 枚举

- Shadow：`Plain`、`Raised`、`Sunken` → 控制边框阴影效果
- Shape：`NoFrame`、`Box`、`Panel`、`StyledPanel`、`HLine`、`VLine` → 控制框架形状
- StyleMask：用于提取阴影和形状值的掩码

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                         | 功能描述                | 使用频率 |
| -------------------------------- | ----------------------- | -------- |
| void setFrameStyle(int style)    | 设置框架样式(形状+阴影) | ★★★★     |
| int frameStyle() const           | 获取当前框架样式        | ★★★      |
| void setFrameShape(Shape)        | 设置框架形状            | ★★★★     |
| Shape frameShape() const         | 获取框架形状            | ★★★      |
| void setFrameShadow(Shadow)      | 设置框架阴影            | ★★★★     |
| Shadow frameShadow() const       | 获取框架阴影            | ★★★      |
| void setLineWidth(int)           | 设置线宽                | ★★★      |
| int lineWidth() const            | 获取线宽                | ★★       |
| void setMidLineWidth(int)        | 设置中线宽度            | ★★       |
| int midLineWidth() const         | 获取中线宽度            | ★★       |
| int frameWidth() const           | 获取总框架宽度          | ★★★      |
| QRect frameRect() const          | 获取框架矩形            | ★★★      |
| void setFrameRect(const QRect &) | 设置框架矩形            | ★★       |

▨ 重写方法

| 方法                          | 触发场景       | 必须调用基类 |
| ----------------------------- | -------------- | ------------ |
| void paintEvent(QPaintEvent*) | 框架绘制       | 是           |
| QSize sizeHint() const        | 尺寸提示计算   | 推荐         |
| void changeEvent(QEvent*)     | 样式或语言变更 | 是           |

3️⃣【信号与槽】 ◇ 输出信号

- QFrame本身没有定义特定信号，继承QWidget的信号

◇ 输入槽

- QFrame没有定义特定槽，但可以响应QWidget的槽

4️⃣【版本适配】 △ Qt 5.12特性

- 框架样式与QStyle系统更好地集成

△ 跨平台差异

- Windows：框架边距与系统DPI设置相关
- macOS：风格更偏向扁平化设计
- 样式表：可使用QSS覆盖原生绘制 (`QFrame { border: 1px solid gray; }`)

5️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 创建水平分隔线
QFrame *line = new QFrame(parent);
line->setFrameShape(QFrame::HLine);
line->setFrameShadow(QFrame::Sunken);

// 创建面板框架
QFrame *panel = new QFrame(parent);
panel->setFrameStyle(QFrame::Panel | QFrame::Raised);
panel->setLineWidth(2);
```

◇ 危险操作：

```cpp
// 错误：直接在paintEvent中重绘而不调用基类
void CustomFrame::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawRect(rect()); // 会丢失原生QFrame绘制
}

// 正确：先调用基类绘制再自定义
void CustomFrame::paintEvent(QPaintEvent *event) {
    QFrame::paintEvent(event); // 先调用基类
    QPainter painter(this);
    // 自定义绘制...
}
```





# QLabel API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QLabel(QWidget *parent = nullptr) ▸ 典型用途：创建空白标签 ▸ 内存策略：父子对象自动回收
- QLabel(const QString &text, QWidget *parent = nullptr) ▸ 典型用途：创建带初始文本的标签 ▸ 使用场景：快速创建静态描述文本

◆ 工厂方法

- 无专用工厂方法

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                               | 功能描述             | 使用频率 |
| ------------------------------------------------------ | -------------------- | -------- |
| void setText(const QString &)                          | 设置标签文本         | ★★★★★    |
| QString text() const                                   | 获取当前文本         | ★★★★     |
| void setPixmap(const QPixmap &)                        | 设置图像显示         | ★★★★     |
| const QPixmap *pixmap() const                          | 获取当前图像         | ★★★      |
| void setAlignment(Qt::Alignment)                       | 设置文本对齐方式     | ★★★★     |
| Qt::Alignment alignment() const                        | 获取当前对齐方式     | ★★       |
| void setWordWrap(bool)                                 | 设置是否自动换行     | ★★★      |
| void setOpenExternalLinks(bool)                        | 设置是否打开外部链接 | ★★       |
| void setTextFormat(Qt::TextFormat)                     | 设置文本格式解析方式 | ★★★      |
| void setTextInteractionFlags(Qt::TextInteractionFlags) | 设置文本交互标志     | ★★       |
| void setMargin(int)                                    | 设置边距大小         | ★★       |
| void clear()                                           | 清除所有内容         | ★★★      |

▨ 重写方法

| 方法                          | 触发场景         | 必须调用基类 |
| ----------------------------- | ---------------- | ------------ |
| void paintEvent(QPaintEvent*) | 文本/图像渲染    | 否           |
| QSize sizeHint() const        | 布局计算首选大小 | 否           |
| QSize minimumSizeHint() const | 布局计算最小大小 | 否           |
| bool event(QEvent *)          | 处理各类事件     | 视情况       |

3️⃣【信号与槽】 ◇ 输出信号

- linkActivated(const QString &link) → 用户点击超链接时发射
- linkHovered(const QString &link) → 鼠标悬停在超链接时发射

◇ 输入槽

- clear() → 清除文本或图像
- setNum(int num) → 显示整数
- setNum(double num) → 显示浮点数

4️⃣【枚举属性】 ◇ 相关属性

| 属性名            | 类型           | 默认值                          | 描述                     |
| ----------------- | -------------- | ------------------------------- | ------------------------ |
| text              | QString        | 空字符串                        | 显示文本                 |
| textFormat        | Qt::TextFormat | Qt::AutoText                    | 文本解析方式             |
| alignment         | Qt::Alignment  | Qt::AlignLeft\|Qt::AlignVCenter | 文本对齐方式             |
| wordWrap          | bool           | false                           | 是否自动换行             |
| margin            | int            | 0                               | 内容边距                 |
| indent            | int            | -1                              | 文本缩进距离             |
| openExternalLinks | bool           | false                           | 是否打开外部链接         |
| scaledContents    | bool           | false                           | 是否拉伸内容填充整个标签 |

◇ 相关枚举

- **Qt::TextFormat**

- 作用：控制文本如何被解析
- 涉及函数：setTextFormat(), textFormat()

| 成员             | 值   | 说明               |
| ---------------- | ---- | ------------------ |
| Qt::PlainText    | 0    | 纯文本，不解析HTML |
| Qt::RichText     | 1    | HTML富文本         |
| Qt::AutoText     | 2    | 自动检测格式       |
| Qt::MarkdownText | 3    | Markdown格式       |

- **Qt::TextInteractionFlag**

- 作用：控制用户如何与标签文本交互
- 涉及函数：setTextInteractionFlags(), textInteractionFlags()

| 成员                          | 说明             |
| ----------------------------- | ---------------- |
| Qt::NoTextInteraction         | 禁用所有文本交互 |
| Qt::TextSelectableByMouse     | 允许鼠标选择文本 |
| Qt::TextSelectableByKeyboard  | 允许键盘选择文本 |
| Qt::LinksAccessibleByMouse    | 允许鼠标点击链接 |
| Qt::LinksAccessibleByKeyboard | 允许键盘激活链接 |

5️⃣【版本适配】 △ Qt 5.12特性

- 保持了与之前版本的API兼容性
- 改进了高DPI显示下的渲染质量

△ 跨平台差异

- Windows：默认字体和渲染与系统风格匹配
- macOS：文本渲染遵循Apple的字体平滑算法
- Linux：依赖于系统配置的字体渲染引擎

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 创建标签并设置富文本
QLabel *label = new QLabel(parent);
label->setText("<b>Bold text</b> and <a href='https://qt.io'>link</a>");
label->setTextFormat(Qt::RichText);
label->setOpenExternalLinks(true);
label->setAlignment(Qt::AlignCenter);

// 创建图像标签
QLabel *imageLabel = new QLabel(parent);
imageLabel->setPixmap(QPixmap(":/images/logo.png"));
imageLabel->setScaledContents(true);

// 连接信号
connect(label, &QLabel::linkActivated, 
        this, &MyWidget::handleLink);
```

◇ 危险操作：

```cpp
// 错误：通过widget接口修改标签内容
QWidget *widget = label;
widget->setWindowTitle("Wrong usage");  // 无效，windowTitle不会影响显示文本

// 错误：将复杂布局交给单个QLabel
label->setText("<table>...</table>");   // 考虑使用QTextBrowser或HTML引擎
label->setWordWrap(true);               // 复杂布局可能导致意外换行

// 正确：区分用途
if (needInteraction)
    useQTextBrowser();  // 交互性文本使用QTextBrowser
else
    useQLabel();        // 静态显示使用QLabel
```





# QLCDNumber API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QLCDNumber(QWidget *parent = nullptr) ▸ 典型用途：创建默认的LCD数字显示组件 ▸ 内存策略：父子对象自动回收
- QLCDNumber(uint numDigits, QWidget *parent = nullptr) ▸ 典型用途：创建指定位数的LCD显示器 ▸ 示例：QLCDNumber(8) 创建8位数显示器

◆ 工厂方法（若有）

- 无专用工厂方法

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                             | 功能描述             | 使用频率 |
| ------------------------------------ | -------------------- | -------- |
| void display(int num)                | 显示整数值           | ★★★★★    |
| void display(double num)             | 显示浮点数           | ★★★★     |
| void display(const QString &str)     | 显示字符串           | ★★★      |
| void setDigitCount(int numDigits)    | 设置数字位数         | ★★★★     |
| int digitCount() const               | 获取当前位数         | ★★       |
| void setMode(Mode mode)              | 设置数字显示模式     | ★★★      |
| Mode mode() const                    | 获取当前模式         | ★★       |
| void setSegmentStyle(SegmentStyle)   | 设置显示段样式       | ★★★      |
| SegmentStyle segmentStyle() const    | 获取当前段样式       | ★★       |
| void setSmallDecimalPoint(bool)      | 设置小数点显示方式   | ★★       |
| bool checkOverflow(int num) const    | 检查整数是否会溢出   | ★★       |
| bool checkOverflow(double num) const | 检查浮点数是否会溢出 | ★★       |

▨ 重写方法

| 方法                          | 触发场景         | 必须调用基类 |
| ----------------------------- | ---------------- | ------------ |
| QSize sizeHint() const        | 布局计算首选大小 | 否           |
| void paintEvent(QPaintEvent*) | 显示渲染         | 否           |
| bool event(QEvent *)          | 事件处理         | 视情况       |

3️⃣【信号与槽】 ◇ 输出信号

- overflow() → 当显示的数字超出当前位数所能表示的范围时发射

◇ 输入槽

- display(int) → 显示整数
- display(double) → 显示浮点数
- display(const QString &) → 显示字符串
- setDigitCount(int) → 设置数字位数
- setMode(Mode) → 设置显示模式
- setSegmentStyle(SegmentStyle) → 设置段样式
- setSmallDecimalPoint(bool) → 设置小数点显示方式

4️⃣【枚举属性】 ◇ 相关属性

| 属性名            | 类型         | 默认值  | 描述                     |
| ----------------- | ------------ | ------- | ------------------------ |
| digitCount        | int          | 5       | 显示的数字位数           |
| mode              | Mode         | Dec     | 数字显示基数模式         |
| segmentStyle      | SegmentStyle | Outline | 显示段的样式             |
| value             | double       | 0       | 当前显示的数值           |
| smallDecimalPoint | bool         | false   | 小数点是否使用小样式显示 |
| intValue          | int          | 0       | 当前显示的整数值（只读） |

◇ 相关枚举

- **QLCDNumber::Mode**

- 作用：定义显示数字的进制模式
- 涉及函数：setMode(), mode()

| 成员 | 值   | 说明                 |
| ---- | ---- | -------------------- |
| Hex  | 0    | 十六进制（0-9, A-F） |
| Dec  | 1    | 十进制（0-9），默认  |
| Oct  | 2    | 八进制（0-7）        |
| Bin  | 3    | 二进制（0-1）        |

- **QLCDNumber::SegmentStyle**

- 作用：定义LCD数字段的显示样式
- 涉及函数：setSegmentStyle(), segmentStyle()

| 成员    | 值   | 说明                     |
| ------- | ---- | ------------------------ |
| Outline | 0    | 轮廓风格，只显示段的边框 |
| Filled  | 1    | 填充风格，完全填充段区域 |
| Flat    | 2    | 扁平风格，无3D效果       |

5️⃣【版本适配】 △ Qt 5.12特性

- 保持了与早期版本的API兼容性
- 优化了高DPI显示下的渲染效果

△ 跨平台差异

- Windows：渲染使用原生绘图API
- macOS：段样式在Retina显示器上更为平滑
- Linux：根据系统主题可能有细微外观差异

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 创建一个8位LCD数字显示器
QLCDNumber *lcd = new QLCDNumber(8, parent);
lcd->setSegmentStyle(QLCDNumber::Filled);  // 填充风格
lcd->setMode(QLCDNumber::Dec);  // 十进制模式
lcd->display(42.5);  // 显示数值

// 连接信号
connect(lcd, &QLCDNumber::overflow,
        this, &MyWidget::handleOverflow);

// 定时器更新显示
QTimer *timer = new QTimer(this);
connect(timer, &QTimer::timeout, this, [this, lcd]() {
    lcd->display(QTime::currentTime().toString("hh:mm:ss"));
});
timer->start(1000);  // 每秒更新一次
```

◇ 危险操作：

```cpp
// 错误：设置过多位数可能导致视觉问题
lcd->setDigitCount(20);  // 过多位数可能导致显示不完整

// 错误：显示超出位数范围的数值而不检查溢出
if (value > 999999 && lcd->digitCount() < 7) {
    // 应该先检查并增加位数
    lcd->display(value);  // 可能触发overflow()信号
}

// 正确：先检查溢出
if (lcd->checkOverflow(value)) {
    lcd->setDigitCount(lcd->digitCount() + 1);
}
lcd->display(value);

// 错误：尝试显示非法字符
lcd->display("HELLO!");  // 只能显示0-9,A-F,空格,小数点和减号
// 正确：仅使用可显示字符
lcd->display("12.3-4");
```



# QComboBox API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QComboBox(QWidget *parent = nullptr) ▸ 典型用途：创建下拉选择框 ▸ 内存策略：父子对象自动回收

◆ 工厂方法

- 无专用工厂方法

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                                     | 功能描述         | 使用频率 |
| ------------------------------------------------------------ | ---------------- | -------- |
| void addItem(const QString &text, const QVariant &userData = QVariant()) | 添加选项项       | ★★★★★    |
| void addItems(const QStringList &texts)                      | 批量添加文本项   | ★★★★     |
| int count() const                                            | 获取项目总数     | ★★★★     |
| QString currentText() const                                  | 获取当前文本     | ★★★★★    |
| int currentIndex() const                                     | 获取当前索引     | ★★★★★    |
| void setCurrentIndex(int index)                              | 设置当前索引     | ★★★★★    |
| void setCurrentText(const QString &text)                     | 设置当前文本     | ★★★★     |
| void setEditable(bool editable)                              | 设置是否可编辑   | ★★★★     |
| bool isEditable() const                                      | 获取是否可编辑   | ★★★      |
| void clear()                                                 | 清空所有项       | ★★★★     |
| QVariant itemData(int index, int role = Qt::UserRole) const  | 获取项数据       | ★★★      |
| void setItemData(int index, const QVariant &value, int role = Qt::UserRole) | 设置项数据       | ★★★      |
| QString itemText(int index) const                            | 获取项文本       | ★★★      |
| void setItemText(int index, const QString &text)             | 设置项文本       | ★★★      |
| QLineEdit *lineEdit() const                                  | 获取编辑器控件   | ★★★      |
| void setMaxVisibleItems(int maxItems)                        | 设置最大可见项数 | ★★       |
| void setInsertPolicy(InsertPolicy policy)                    | 设置插入策略     | ★★       |
| void setMaxCount(int max)                                    | 设置最大项数     | ★★       |
| void setModel(QAbstractItemModel *model)                     | 设置数据模型     | ★★★      |
| void setModelColumn(int visibleColumn)                       | 设置显示的模型列 | ★★       |
| void setCompleter(QCompleter *completer)                     | 设置自动完成器   | ★★       |
| void setSizeAdjustPolicy(SizeAdjustPolicy policy)            | 设置大小调整策略 | ★★       |

▨ 重写方法

| 方法                           | 触发场景       | 必须调用基类 |
| ------------------------------ | -------------- | ------------ |
| void showPopup()               | 显示下拉列表时 | 建议         |
| void hidePopup()               | 隐藏下拉列表时 | 建议         |
| void paintEvent(QPaintEvent*)  | 控件渲染       | 否           |
| void wheelEvent(QWheelEvent*)  | 鼠标滚轮事件   | 视情况       |
| void keyPressEvent(QKeyEvent*) | 键盘事件       | 视情况       |
| bool event(QEvent *)           | 通用事件处理   | 视情况       |

3️⃣【信号与槽】 ◇ 输出信号

- currentIndexChanged(int index) → 当前选中项索引变化时发射
- currentIndexChanged(const QString &text) → 当前选中项文本变化时发射
- currentTextChanged(const QString &text) → 当前文本变化时发射
- activated(int index) → 用户选择项目时发射（键盘或鼠标）
- activated(const QString &text) → 同上，带文本参数
- highlighted(int index) → 项目被高亮时发射
- highlighted(const QString &text) → 同上，带文本参数
- editTextChanged(const QString &text) → 可编辑模式下输入文本变化时发射

◇ 输入槽

- clear() → 清空所有项目
- clearEditText() → 清空编辑区文本
- setCurrentIndex(int index) → 设置当前索引
- setCurrentText(const QString &text) → 设置当前文本
- showPopup() → 显示下拉列表
- hidePopup() → 隐藏下拉列表

4️⃣【枚举属性】 ◇ 相关属性

| 属性名                | 类型             | 默认值                      | 描述                 |
| --------------------- | ---------------- | --------------------------- | -------------------- |
| count                 | int              | 0                           | 项目数量（只读）     |
| currentIndex          | int              | -1                          | 当前选中项索引       |
| currentText           | QString          | 空字符串                    | 当前选中项文本       |
| duplicatesEnabled     | bool             | false                       | 是否允许重复项       |
| editable              | bool             | false                       | 是否可编辑           |
| frame                 | bool             | true                        | 是否显示边框         |
| iconSize              | QSize            | 依赖样式                    | 图标大小             |
| insertPolicy          | InsertPolicy     | InsertAtBottom              | 可编辑模式下插入策略 |
| maxCount              | int              | 2147483647                  | 最大项目数           |
| maxVisibleItems       | int              | 10                          | 下拉列表最大可见项数 |
| minimumContentsLength | int              | 0                           | 最小内容长度         |
| modelColumn           | int              | 0                           | 使用模型时显示的列   |
| sizeAdjustPolicy      | SizeAdjustPolicy | AdjustToContentsOnFirstShow | 大小调整策略         |

◇ 相关枚举

- **QComboBox::InsertPolicy**

- 作用：控制可编辑下拉框中如何插入用户输入的新项
- 涉及函数：setInsertPolicy(), insertPolicy()

| 成员                 | 值   | 说明               |
| -------------------- | ---- | ------------------ |
| NoInsert             | 0    | 不插入             |
| InsertAtTop          | 1    | 插入到顶部         |
| InsertAtCurrent      | 2    | 替换当前项         |
| InsertAtBottom       | 3    | 插入到底部（默认） |
| InsertAfterCurrent   | 4    | 插入到当前项之后   |
| InsertBeforeCurrent  | 5    | 插入到当前项之前   |
| InsertAlphabetically | 6    | 按字母顺序插入     |

- **QComboBox::SizeAdjustPolicy**

- 作用：控制下拉框如何调整其大小
- 涉及函数：setSizeAdjustPolicy(), sizeAdjustPolicy()

| 成员                                  | 值   | 说明                                    |
| ------------------------------------- | ---- | --------------------------------------- |
| AdjustToContents                      | 0    | 始终根据内容调整大小                    |
| AdjustToContentsOnFirstShow           | 1    | 首次显示时根据内容调整大小（默认）      |
| AdjustToMinimumContentsLengthWithIcon | 2    | 根据minimumContentsLength和图标调整大小 |

5️⃣【版本适配】 △ Qt 5.12特性

- 保持了与Qt 5.11版本的API兼容性
- 改进了高DPI支持和下拉列表的渲染效果
- 修复了可编辑模式下的几个焦点和导航问题

△ 跨平台差异

- Windows：下拉列表样式遵循Windows原生控件样式
- macOS：下拉按钮和列表行为符合macOS界面设计规范
- Linux：外观取决于当前Qt样式或GTK主题集成

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 基本用法
QComboBox *comboBox = new QComboBox(parent);
comboBox->addItem("选项一");
comboBox->addItem("选项二", QVariant(123));  // 带用户数据
comboBox->addItems(QStringList() << "选项三" << "选项四" << "选项五");
comboBox->setCurrentIndex(0);  // 选择第一个选项

// 响应选择变化
connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=](int index) {
            QVariant itemData = comboBox->itemData(index);
            qDebug() << "选择了:" << comboBox->itemText(index);
        });

// 可编辑下拉框
QComboBox *editableCombo = new QComboBox(parent);
editableCombo->setEditable(true);
editableCombo->setInsertPolicy(QComboBox::InsertAtBottom);
editableCombo->setCompleter(new QCompleter(editableCombo));

// 使用模型
QStandardItemModel *model = new QStandardItemModel(0, 2, parent);
for (int i = 0; i < 10; ++i) {
    model->insertRow(i);
    model->setData(model->index(i, 0), QString("项目 %1").arg(i));
    model->setData(model->index(i, 1), QVariant(i * 10));
}
QComboBox *modelCombo = new QComboBox(parent);
modelCombo->setModel(model);
modelCombo->setModelColumn(0);  // 显示第一列
```

◇ 危险操作：

```cpp
// 错误：不检查索引有效性
comboBox->setCurrentIndex(999);  // 超出范围索引
QVariant data = comboBox->itemData(comboBox->count());  // 无效索引

// 正确：检查索引
int index = 5;
if (index >= 0 && index < comboBox->count()) {
    comboBox->setCurrentIndex(index);
}

// 错误：可编辑模式下直接访问编辑器而不检查
comboBox->lineEdit()->setText("新文本");  // 如果不可编辑会崩溃

// 正确：先检查是否可编辑
if (comboBox->isEditable()) {
    comboBox->lineEdit()->setText("新文本");
}
// 或使用更安全的方法
comboBox->setEditable(true);
comboBox->setCurrentText("新文本");

// 错误：在项目变化信号处理函数中修改项目，可能导致递归
connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=](int) { comboBox->clear(); });  // 可能引起无限递归

// 正确：使用单次连接或延迟处理
auto connection = connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=, &connection](int) { 
            disconnect(connection);  // 断开连接防止递归
            comboBox->clear(); 
        });
```







# QLineEdit API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QLineEdit(QWidget *parent = nullptr) ▸ 典型用途：创建空白单行文本框 ▸ 内存策略：父子对象自动回收
- QLineEdit(const QString &contents, QWidget *parent = nullptr) ▸ 典型用途：创建带初始文本的单行文本框 ▸ 内容约束：仅适合单行文本（不支持多行）

◆ 工厂方法

- 无专用工厂方法

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                 | 功能描述             | 使用频率 |
| ---------------------------------------- | -------------------- | -------- |
| QString text() const                     | 获取当前文本         | ★★★★★    |
| void setText(const QString &)            | 设置文本内容         | ★★★★★    |
| void clear()                             | 清空文本             | ★★★★     |
| void selectAll()                         | 选择全部文本         | ★★★★     |
| void setMaxLength(int)                   | 设置最大文本长度     | ★★★★     |
| int maxLength() const                    | 获取最大长度限制     | ★★★      |
| void setReadOnly(bool)                   | 设置只读状态         | ★★★★     |
| bool isReadOnly() const                  | 检查是否只读         | ★★★      |
| void setPlaceholderText(const QString &) | 设置占位提示文本     | ★★★★     |
| QString placeholderText() const          | 获取占位文本         | ★★★      |
| void setInputMask(const QString &)       | 设置输入掩码         | ★★★      |
| QString inputMask() const                | 获取输入掩码         | ★★       |
| void setEchoMode(EchoMode)               | 设置文本显示模式     | ★★★★     |
| EchoMode echoMode() const                | 获取显示模式         | ★★★      |
| void setClearButtonEnabled(bool)         | 设置是否显示清空按钮 | ★★★      |
| void setValidator(const QValidator *)    | 设置输入验证器       | ★★★      |
| const QValidator *validator() const      | 获取验证器           | ★★       |
| void setCompleter(QCompleter *)          | 设置自动完成器       | ★★★      |
| QCompleter *completer() const            | 获取自动完成器       | ★★       |
| QSize sizeHint() const                   | 获取建议大小         | ★★       |
| void setAlignment(Qt::Alignment)         | 设置文本对齐方式     | ★★★      |
| Qt::Alignment alignment() const          | 获取对齐方式         | ★★       |
| void setCursorPosition(int)              | 设置光标位置         | ★★★      |
| int cursorPosition() const               | 获取光标位置         | ★★★      |
| void setSelection(int start, int length) | 设置文本选择范围     | ★★★      |
| bool hasSelectedText() const             | 检查是否有选中文本   | ★★★      |
| QString selectedText() const             | 获取选中的文本       | ★★★      |
| int selectionStart() const               | 获取选择起始位置     | ★★       |
| void setModified(bool)                   | 设置修改标志         | ★★       |
| bool isModified() const                  | 检查是否被修改       | ★★★      |
| void cut()                               | 剪切选中文本         | ★★★      |
| void copy()                              | 复制选中文本         | ★★★      |
| void paste()                             | 粘贴剪贴板内容       | ★★★      |
| void undo()                              | 撤销操作             | ★★★      |
| void redo()                              | 重做操作             | ★★★      |

▨ 重写方法

| 方法                                       | 触发场景         | 必须调用基类 |
| ------------------------------------------ | ---------------- | ------------ |
| void keyPressEvent(QKeyEvent *)            | 键盘输入时       | 视情况       |
| void focusInEvent(QFocusEvent *)           | 获得焦点时       | 是           |
| void focusOutEvent(QFocusEvent *)          | 失去焦点时       | 是           |
| void paintEvent(QPaintEvent *)             | 绘制控件时       | 否           |
| void mousePressEvent(QMouseEvent *)        | 鼠标按下时       | 视情况       |
| void mouseMoveEvent(QMouseEvent *)         | 鼠标移动时       | 视情况       |
| void mouseReleaseEvent(QMouseEvent *)      | 鼠标释放时       | 视情况       |
| void mouseDoubleClickEvent(QMouseEvent *)  | 鼠标双击时       | 视情况       |
| void inputMethodEvent(QInputMethodEvent *) | 输入法事件       | 是           |
| void contextMenuEvent(QContextMenuEvent *) | 上下文菜单请求时 | 视情况       |

3️⃣【信号与槽】 ◇ 输出信号

- textChanged(const QString &text) → 文本内容变化时发射
- textEdited(const QString &text) → 用户编辑文本时发射（不包括程序修改）
- returnPressed() → 用户按下回车/换行键时发射
- editingFinished() → 编辑完成时发射（失去焦点或按下回车）
- cursorPositionChanged(int old, int new) → 光标位置变化时发射
- selectionChanged() → 选择范围变化时发射

◇ 输入槽

- clear() → 清空文本内容
- copy() → 复制选中文本到剪贴板
- cut() → 剪切选中文本到剪贴板
- paste() → 从剪贴板粘贴文本
- undo() → 撤销操作
- redo() → 重做操作
- selectAll() → 选择全部文本
- setText(const QString &) → 设置文本内容

4️⃣【枚举属性】 ◇ 相关属性

| 属性名             | 类型          | 默认值        | 描述                               |
| ------------------ | ------------- | ------------- | ---------------------------------- |
| text               | QString       | 空字符串      | 当前文本内容                       |
| maxLength          | int           | 32767         | 最大允许文本长度                   |
| frame              | bool          | true          | 是否显示边框                       |
| readOnly           | bool          | false         | 是否为只读模式                     |
| echoMode           | EchoMode      | Normal        | 文本显示模式                       |
| placeholderText    | QString       | 空字符串      | 占位提示文本                       |
| cursorPosition     | int           | 0             | 光标位置                           |
| modified           | bool          | false         | 内容是否已修改                     |
| hasSelectedText    | bool          | false         | 是否有选中文本（只读）             |
| selectedText       | QString       | 空字符串      | 当前选中的文本（只读）             |
| alignment          | Qt::Alignment | Qt::AlignLeft | 文本对齐方式                       |
| acceptableInput    | bool          | true          | 当前输入是否符合验证器要求（只读） |
| clearButtonEnabled | bool          | false         | 是否显示清空按钮                   |
| dragEnabled        | bool          | false         | 是否启用拖拽                       |

◇ 相关枚举

- **QLineEdit::EchoMode**

- 作用：定义文本的显示模式，特别用于密码输入
- 涉及函数：setEchoMode(), echoMode()

| 成员               | 值   | 说明                                   |
| ------------------ | ---- | -------------------------------------- |
| Normal             | 0    | 正常显示输入的字符                     |
| NoEcho             | 1    | 不显示任何内容（包括光标）             |
| Password           | 2    | 显示密码掩码字符代替实际输入           |
| PasswordEchoOnEdit | 3    | 编辑时显示实际字符，失去焦点后显示掩码 |

- **QLineEdit::ActionPosition**

- 作用：定义自定义动作按钮的位置
- 涉及函数：addAction()

| 成员             | 值   | 说明           |
| ---------------- | ---- | -------------- |
| LeadingPosition  | 0    | 文本框开始位置 |
| TrailingPosition | 1    | 文本框结束位置 |

5️⃣【版本适配】 △ Qt 5.12特性

- setClearButtonEnabled() 方法已经稳定，用于添加清除按钮
- 支持自定义操作按钮通过addAction()方法
- 改进了高DPI显示支持
- 增强了触摸屏输入体验

△ 跨平台差异

- Windows：按照Windows原生文本框风格渲染
- macOS：遵循Apple人机界面指南的输入框样式
- Linux：依赖于所使用的桌面环境和主题

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 基本用法：创建文本输入框
QLineEdit *nameInput = new QLineEdit(parent);
nameInput->setPlaceholderText("请输入姓名");
nameInput->setMaxLength(50);
nameInput->setText("初始文本");

// 获取输入内容
connect(nameInput, &QLineEdit::returnPressed, [=]() {
    QString inputText = nameInput->text();
    qDebug() << "用户输入:" << inputText;
});

// 创建密码输入框
QLineEdit *passwordInput = new QLineEdit(parent);
passwordInput->setEchoMode(QLineEdit::Password);
passwordInput->setPlaceholderText("请输入密码");
passwordInput->setClearButtonEnabled(true);

// 使用验证器限制输入
QLineEdit *ageInput = new QLineEdit(parent);
QIntValidator *validator = new QIntValidator(0, 120, ageInput);
ageInput->setValidator(validator);
ageInput->setPlaceholderText("年龄 (0-120)");

// 使用输入掩码
QLineEdit *phoneInput = new QLineEdit(parent);
phoneInput->setInputMask("+99 999-999-9999");
phoneInput->setPlaceholderText("电话号码");

// 添加自定义动作按钮
QLineEdit *searchInput = new QLineEdit(parent);
QAction *searchAction = searchInput->addAction(
    QIcon(":/icons/search.png"), 
    QLineEdit::TrailingPosition
);
connect(searchAction, &QAction::triggered, [=]() {
    performSearch(searchInput->text());
});
```

◇ 危险操作：

```cpp
// 错误：不检查就假设用户输入的是有效数字
QLineEdit *numberInput = new QLineEdit(parent);
// 没有设置验证器
double value = numberInput->text().toDouble();  // 可能转换失败

// 正确：使用验证器并检查输入有效性
QLineEdit *safeNumberInput = new QLineEdit(parent);
safeNumberInput->setValidator(new QDoubleValidator(safeNumberInput));
if (safeNumberInput->hasAcceptableInput()) {
    double value = safeNumberInput->text().toDouble();
}

// 错误：在textChanged信号处理中修改文本，可能导致递归
connect(lineEdit, &QLineEdit::textChanged, [=](const QString &text) {
    lineEdit->setText(text.toUpper());  // 会再次触发textChanged
});

// 正确：避免递归
connect(lineEdit, &QLineEdit::textChanged, [=](const QString &text) {
    QString upper = text.toUpper();
    if (upper != text) {
        lineEdit->blockSignals(true);
        lineEdit->setText(upper);
        lineEdit->blockSignals(false);
    }
});

// 错误：未保存用户的输入位置就修改文本
int len = lineEdit->text().length();
lineEdit->setText(newText);  // 光标会跳到末尾

// 正确：保存和恢复光标位置
int pos = lineEdit->cursorPosition();
lineEdit->setText(newText);
if (pos <= newText.length()) {
    lineEdit->setCursorPosition(pos);
}
```





# QValidator API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QValidator(QObject *parent = nullptr) ▸ 典型用途：创建验证器基类实例 ▸ 内存策略：父子对象自动回收

◆ 工厂方法

- 无直接工厂方法（抽象基类） ▸ 注意：通常使用子类实例化，如QIntValidator、QDoubleValidator等

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                                   | 功能描述                 | 使用频率 |
| ---------------------------------------------------------- | ------------------------ | -------- |
| virtual State validate(QString &input, int &pos) const = 0 | 核心验证方法（纯虚函数） | ★★★★★    |
| virtual void fixup(QString &input) const                   | 修复不完全有效的输入     | ★★★      |
| void setLocale(const QLocale &locale)                      | 设置区域设置             | ★★       |
| QLocale locale() const                                     | 获取当前区域设置         | ★★       |

▨ 重写方法

| 方法                       | 触发场景           | 必须调用基类   |
| -------------------------- | ------------------ | -------------- |
| validate(QString &, int &) | 输入验证时         | 否（纯虚函数） |
| fixup(QString &)           | 完成编辑时修复输入 | 否             |

3️⃣【信号与槽】 ◇ 输出信号

- changed() → 验证器状态变化时发射（如规则变更）

◇ 输入槽

- 无专用槽方法

4️⃣【枚举属性】 ◇ 相关属性

| 属性名 | 类型    | 默认值   | 描述                 |
| ------ | ------- | -------- | -------------------- |
| locale | QLocale | 系统默认 | 影响数字格式处理规则 |

◇ 相关枚举

- **QValidator::State**

- 作用：描述验证结果状态
- 涉及函数：validate()

| 成员         | 值   | 说明                         |
| ------------ | ---- | ---------------------------- |
| Invalid      | 0    | 输入无效，不应被接受         |
| Intermediate | 1    | 输入不完全有效，但可继续编辑 |
| Acceptable   | 2    | 输入完全有效，可被接受       |

5️⃣【版本适配】 △ Qt 5.12特性

- 无主要变化

△ 跨平台差异

- 默认locale()行为取决于系统区域设置
- 数字格式处理（小数点、千位分隔符等）在不同语言环境下表现不同

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 使用整数验证器
QIntValidator *validator = new QIntValidator(0, 100, this);
ui->lineEdit->setValidator(validator);

// 使用正则表达式验证器
QRegExpValidator *emailValidator = new QRegExpValidator(
    QRegExp("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}"), this);
ui->emailEdit->setValidator(emailValidator);
```

◇ 自定义验证器：

```cpp
// 创建自定义验证器
class MyValidator : public QValidator {
public:
    MyValidator(QObject *parent = nullptr) : QValidator(parent) {}
    
    State validate(QString &input, int &pos) const override {
        if (input.isEmpty())
            return Intermediate;
        if (input.length() > 10)
            return Invalid;
        if (input.startsWith("Qt"))
            return Acceptable;
        return Intermediate;
    }
    
    void fixup(QString &input) const override {
        if (!input.startsWith("Qt"))
            input.prepend("Qt");
    }
};
```

◇ 危险操作：

```cpp
// 错误：从非GUI线程调用validate
QThread *workerThread = new QThread();
connect(workerThread, &QThread::started, [=]() {
    QString text = "123";
    int pos = 0;
    validator->validate(text, pos); // 不应在非GUI线程调用
});

// 正确：将验证逻辑移至GUI线程
connect(workerThread, &QThread::finished, this, [=]() {
    QString text = "123";
    int pos = 0;
    validator->validate(text, pos); // 在主线程中调用
});
```







# QAbstractSpinBox API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QAbstractSpinBox(QWidget *parent = nullptr) ▸ 典型用途：创建抽象基类实例（通常使用子类） ▸ 内存策略：父子对象自动回收

◆ 工厂方法

- 无专用工厂方法（抽象基类） ▸ 注意：通常使用子类实例化，如QSpinBox、QDoubleSpinBox、QDateTimeEdit

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                   | 功能描述           | 使用频率 |
| ------------------------------------------ | ------------------ | -------- |
| QString text() const                       | 获取当前显示文本   | ★★★★     |
| bool hasAcceptableInput() const            | 检查输入是否被接受 | ★★★★     |
| void setReadOnly(bool r)                   | 设置只读状态       | ★★★      |
| void setAlignment(Qt::Alignment alignment) | 设置文本对齐方式   | ★★★      |
| void setFrame(bool)                        | 设置是否显示边框   | ★★       |
| void setWrapping(bool w)                   | 设置值是否循环包装 | ★★       |
| void setButtonSymbols(ButtonSymbols bs)    | 设置上下按钮样式   | ★★★      |
| void setAccelerated(bool on)               | 设置是否启用加速   | ★★       |
| void setCorrectionMode(CorrectionMode cm)  | 设置修正模式       | ★★       |
| void setKeyboardTracking(bool kt)          | 设置是否即时更新值 | ★★★      |
| void stepBy(int steps)                     | 按指定步数增减     | ★★★★     |

▨ 重写方法

| 方法                                                 | 触发场景             | 必须调用基类 |
| ---------------------------------------------------- | -------------------- | ------------ |
| void keyPressEvent(QKeyEvent*)                       | 键盘事件处理         | 否           |
| void wheelEvent(QWheelEvent*)                        | 鼠标滚轮事件处理     | 否           |
| void focusInEvent(QFocusEvent*)                      | 获得焦点时           | 建议         |
| void focusOutEvent(QFocusEvent*)                     | 失去焦点时           | 建议         |
| virtual StepEnabled stepEnabled() const              | 决定步进按钮启用状态 | 否           |
| virtual void validate(QString &text, int &pos) const | 验证输入文本         | 否           |
| virtual void fixup(QString &input) const             | 修复不完整的输入     | 否           |

3️⃣【信号与槽】 ◇ 输出信号

- editingFinished() → 编辑完成时发射

◇ 输入槽

- clear() → 清空内容
- selectAll() → 全选文本
- stepUp() → 增加一步
- stepDown() → 减少一步

4️⃣【枚举属性】 ◇ 相关属性

| 属性名           | 类型           | 默认值                                   | 描述                     |
| ---------------- | -------------- | ---------------------------------------- | ------------------------ |
| alignment        | Qt::Alignment  | Qt::AlignLeft                            | 文本对齐方式             |
| buttonSymbols    | ButtonSymbols  | UpDownArrows                             | 按钮样式                 |
| correctionMode   | CorrectionMode | QAbstractSpinBox::CorrectToPreviousValue | 修正模式                 |
| frame            | bool           | true                                     | 是否显示边框             |
| keyboardTracking | bool           | true                                     | 键盘输入时是否即时更新值 |
| readOnly         | bool           | false                                    | 是否只读                 |
| wrapping         | bool           | false                                    | 是否循环包装             |
| accelerated      | bool           | false                                    | 是否加速增减             |

◇ 相关枚举

- **QAbstractSpinBox::ButtonSymbols**

- 作用：定义上下按钮的显示样式
- 涉及函数：setButtonSymbols()

| 成员         | 值   | 说明             |
| ------------ | ---- | ---------------- |
| UpDownArrows | 0    | 显示上下箭头按钮 |
| PlusMinus    | 1    | 显示加减号按钮   |
| NoButtons    | 2    | 不显示按钮       |

- **QAbstractSpinBox::CorrectionMode**

- 作用：定义输入错误时的修正方式
- 涉及函数：setCorrectionMode()

| 成员                   | 值   | 说明                 |
| ---------------------- | ---- | -------------------- |
| CorrectToPreviousValue | 0    | 恢复到之前的有效值   |
| CorrectToNearestValue  | 1    | 修正为最接近的有效值 |

- **QAbstractSpinBox::StepEnabledFlag**

- 作用：指示可用的步进方向
- 涉及函数：stepEnabled()

| 成员            | 值   | 说明         |
| --------------- | ---- | ------------ |
| StepNone        | 0x00 | 禁用所有步进 |
| StepUpEnabled   | 0x01 | 启用向上步进 |
| StepDownEnabled | 0x02 | 启用向下步进 |

5️⃣【版本适配】 △ Qt 5.12特性

- 稳定特性，无主要变化

△ 跨平台差异

- macOS：按钮风格与系统标准控件一致
- Windows/Linux：按钮外观根据当前样式而变化
- 触摸设备：按钮尺寸会自动调整以适应触摸操作

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 基本设置（通常使用子类）
QSpinBox *spinBox = new QSpinBox(this);
spinBox->setRange(0, 100);
spinBox->setValue(50);
spinBox->setPrefix("$");
spinBox->setSuffix(" units");

// 设置按钮样式
spinBox->setButtonSymbols(QAbstractSpinBox::PlusMinus);

// 设置只读状态
spinBox->setReadOnly(true);

// 连接信号
connect(spinBox, &QSpinBox::editingFinished, this, &MyWidget::handleEditingFinished);
```

◇ 自定义子类实现：

```cpp
class CustomSpinBox : public QAbstractSpinBox {
public:
    CustomSpinBox(QWidget *parent = nullptr) : QAbstractSpinBox(parent) {
        // 初始化
    }

protected:
    QAbstractSpinBox::StepEnabled stepEnabled() const override {
        // 自定义步进启用逻辑
        if (value() <= minValue)
            return QAbstractSpinBox::StepUpEnabled;
        else if (value() >= maxValue)
            return QAbstractSpinBox::StepDownEnabled;
        return QAbstractSpinBox::StepUpEnabled | QAbstractSpinBox::StepDownEnabled;
    }
    
    void stepBy(int steps) override {
        // 自定义步进逻辑
        setValue(value() + steps * singleStep());
        update();
    }
};
```

◇ 危险操作：

```cpp
// 错误：在非GUI线程中访问QAbstractSpinBox
QThread *workerThread = new QThread();
connect(workerThread, &QThread::started, [=]() {
    spinBox->setValue(100); // 不应在非GUI线程中访问
});

// 正确：使用信号槽机制
connect(workerThread, &WorkerThread::valueReady,
        spinBox, &QSpinBox::setValue, Qt::QueuedConnection);
```





# QSpinBox API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QSpinBox(QWidget *parent = nullptr) ▸ 典型用途：创建基础整数数值输入框 ▸ 内存策略：继承自QAbstractSpinBox，父子对象自动回收

◆ 工厂方法（若有）

- 无特殊工厂方法，标准构造即可满足需求

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                        | 功能描述         | 使用频率 |
| ------------------------------- | ---------------- | -------- |
| int value() const               | 获取当前数值     | ★★★★★    |
| void setValue(int val)          | 设置当前数值     | ★★★★★    |
| void setRange(int min, int max) | 设置数值范围     | ★★★★     |
| void setMinimum(int min)        | 设置最小值       | ★★★      |
| void setMaximum(int max)        | 设置最大值       | ★★★      |
| int minimum() const             | 获取最小值       | ★★★      |
| int maximum() const             | 获取最大值       | ★★★      |
| void setSingleStep(int step)    | 设置单步增减值   | ★★★      |
| int singleStep() const          | 获取单步增减值   | ★★       |
| void setPrefix(const QString &) | 设置前缀文本     | ★★★      |
| void setSuffix(const QString &) | 设置后缀文本     | ★★★      |
| QString prefix() const          | 获取前缀文本     | ★★       |
| QString suffix() const          | 获取后缀文本     | ★★       |
| void setDisplayIntegerBase(int) | 设置显示整数进制 | ★★       |
| int displayIntegerBase() const  | 获取显示整数进制 | ★        |

▨ 重写方法

| 方法                                               | 触发场景     | 必须调用基类 |
| -------------------------------------------------- | ------------ | ------------ |
| void stepBy(int steps)                             | 步进操作时   | 否           |
| QValidator::State validate(QString &, int &) const | 输入验证     | 否           |
| int valueFromText(const QString &) const           | 文本转数值   | 否           |
| QString textFromValue(int) const                   | 数值转文本   | 否           |
| void fixup(QString &) const                        | 修正无效输入 | 否           |

3️⃣【信号与槽】 ◇ 输出信号

- valueChanged(int) → 数值变化时发射
- valueChanged(const QString &) → 带显示文本的变化信号

◇ 输入槽

- setValue(int) → 设置数值
- stepUp() → 增加一个步进值
- stepDown() → 减少一个步进值
- clear() → 清空当前值（继承自QAbstractSpinBox）

4️⃣【枚举属性】 ◇ 相关属性

| 属性名             | 类型    | 默认值 | 描述                   |
| ------------------ | ------- | ------ | ---------------------- |
| value              | int     | 0      | 当前数值               |
| minimum            | int     | 0      | 最小值                 |
| maximum            | int     | 99     | 最大值                 |
| singleStep         | int     | 1      | 单步增减值             |
| prefix             | QString | ""     | 显示前缀               |
| suffix             | QString | ""     | 显示后缀               |
| cleanText          | QString | 只读   | 不含前后缀的纯数值文本 |
| displayIntegerBase | int     | 10     | 显示进制（2-36）       |
| wrapping           | bool    | false  | 是否循环包裹           |

◇ 相关枚举

- 无特定枚举，使用标准Qt::AlignmentFlag控制对齐方式

5️⃣【版本适配】 △ Qt 5.12特性

- 继承自QAbstractSpinBox的StepType支持自定义步进类型

△ 跨平台差异

- Windows：支持滚轮操作默认开启
- macOS：遵循Apple HIG设计规范，按钮显示风格不同

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
QSpinBox *spinBox = new QSpinBox(parent);
spinBox->setRange(0, 1000);
spinBox->setValue(100);
spinBox->setSuffix(" px");
spinBox->setPrefix("width: ");
connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
        this, &MyWidget::onValueChanged);
```

◇ 危险操作：

```cpp
// 错误：设置无效范围
spinBox->setMinimum(100);
spinBox->setMaximum(50);  // 会导致最大值被设为100

// 正确：一次性设置范围
spinBox->setRange(0, 100);

// 错误：直接修改text而非value
spinBox->lineEdit()->setText("200"); // 不会触发valueChanged信号
// 正确：使用setValue
spinBox->setValue(200);  // 会触发valueChanged信号
```
